#pragma once
#include <iostream>
#include <vector>
#include <cassert>

using namespace std;

// 颜色枚举：红黑树的节点颜色
enum Colour
{
    RED,
    BLACK
};

// 红黑树节点定义 (泛型T)
template<class T>
struct RBTreeNode
{
    RBTreeNode<T>* _left;
    RBTreeNode<T>* _right;
    RBTreeNode<T>* _parent;
    T _data;                // 节点数据，T可能是Key，也可能是pair<K, V>
    Colour _col;

    // 构造函数
    RBTreeNode(const T& data)
        :_left(nullptr)
        , _right(nullptr)
        , _parent(nullptr)
        , _data(data)      // 初始化节点数据
        , _col(RED)        // 新插入节点默认为红色（红黑树规则）
    {}
};

// 红黑树迭代器
template<class T>
struct __TreeIterator
{
    typedef RBTreeNode<T> Node;   // 节点类型别名
    typedef __TreeIterator<T> Self; // 迭代器自身类型别名
    Node* _node;                 // 当前迭代器指向的节点

    // 构造函数：用节点指针初始化迭代器
    __TreeIterator(Node* node)
        : _node(node)
    {}

    // 解引用操作符：返回节点数据的引用
    T& operator*()
    {
        return _node->_data;
    }

    // 箭头操作符：返回节点数据的指针
    T* operator->()
    {
        return &_node->_data;
    }

    // 不等于操作符：比较两个迭代器是否指向不同节点
    bool operator!=(const Self& s)
    {
        return _node != s._node;
    }

    // 等于操作符：比较两个迭代器是否指向相同节点
    bool operator==(const Self& s)
    {
        return _node == s._node;
    }

    // 前置++操作符：迭代器移动到中序后继节点
    Self& operator++()
    {
        if (_node->_right) // 如果当前节点有右子树
        {
            // 下一个节点是右子树的最左节点（右子树中的最小节点）
            Node* cur = _node->_right;
            while (cur->_left)
            {
                cur = cur->_left;
            }
            _node = cur;
        }
        else // 如果当前节点没有右子树
        {
            // 向上查找，直到当前节点是父节点的左子节点
            Node* cur = _node;
            Node* parent = cur->_parent;
            while (parent && cur == parent->_right)
            {
                cur = parent;
                parent = parent->_parent;
            }
            _node = parent; // 父节点就是下一个节点
        }
        return *this;
    }

    // 前置--操作符：迭代器移动到中序前驱节点
    Self& operator--()
    {
        if (_node == nullptr) // 特殊情况：end()迭代器
        {
            // 这里假设外部能保证不是空树
            // 实际标准库通常通过header节点处理，这里简化处理
            return *this;
        }

        if (_node->_left) // 如果当前节点有左子树
        {
            // 前一个节点是左子树的最右节点（左子树中的最大节点）
            Node* cur = _node->_left;
            while (cur->_right)
            {
                cur = cur->_right;
            }
            _node = cur;
        }
        else // 如果当前节点没有左子树
        {
            // 向上查找，直到当前节点是父节点的右子节点
            Node* cur = _node;
            Node* parent = cur->_parent;
            while (parent && cur == parent->_left)
            {
                cur = parent;
                parent = parent->_parent;
            }
            _node = parent; // 父节点就是前一个节点
        }
        return *this;
    }
};

// 红黑树类模板
// K: 键值类型
// T: 存储的数据类型 (Set是K, Map是pair<K,V>)
// KeyOfT: 仿函数，用于从T中提取键值K
template<class K, class T, class KeyOfT>
class RBTree
{
    typedef RBTreeNode<T> Node; // 节点类型别名
public:
    typedef __TreeIterator<T> iterator; // 迭代器类型别名

    // 返回指向最小元素的迭代器
    iterator begin()
    {
        Node* cur = _root;
        // 一直向左走找到最小节点
        while (cur && cur->_left)
        {
            cur = cur->_left;
        }
        return iterator(cur);
    }

    // 返回尾后迭代器
    iterator end()
    {
        return iterator(nullptr);
    }

    // 插入操作：返回pair<迭代器, 是否插入成功>
    pair<iterator, bool> Insert(const T& data)
    {
        if (_root == nullptr) // 空树情况
        {
            _root = new Node(data);
            _root->_col = BLACK; // 根节点必须为黑色
            return make_pair(iterator(_root), true);
        }
        Node* parent = nullptr;
        Node* cur = _root;
        KeyOfT kot; // 仿函数对象，用于提取键值
        // 查找插入位置
        while (cur)
        {
            if (kot(cur->_data) < kot(data)) // 当前节点键值小于插入键值
            {
                parent = cur;
                cur = cur->_right;
            }
            else if (kot(cur->_data) > kot(data)) // 当前节点键值大于插入键值
            {
                parent = cur;
                cur = cur->_left;
            }
            else // 键值已存在，插入失败
            {
                return make_pair(iterator(cur), false);
            }
        }
        // 创建新节点
        cur = new Node(data);
        Node* newnode = cur; // 保存新节点指针用于返回
        cur->_col = RED;     // 新节点颜色为红色
        // 将新节点链接到父节点
        if (kot(parent->_data) < kot(data))
        {
            parent->_right = cur;
            cur->_parent = parent;
        }
        else
        {
            parent->_left = cur;
            cur->_parent = parent;
        }

        // 红黑树平衡调整：父节点为红色时需要调整
        // 因为红色节点不能连续出现(规则3)
        while (parent && parent->_col == RED)
        {
            Node* grandfather = parent->_parent; // 祖父节点
            if (parent == grandfather->_left) // 父节点是祖父节点的左孩子
            {
                Node* uncle = grandfather->_right; // 叔叔节点
                // 情况1: 叔叔节点存在且为红色
                /*
                 *      g(B)                    g(R)
                 *     /    \                  /    \
                 *   p(R)   u(R)   ====>     p(B)   u(B)
                 *   /                        /
                 * c(R)                     c(R)
                 * 操作：变色，继续向上调整
                 */
                if (uncle && uncle->_col == RED)
                {
                    // 颜色调整
                    parent->_col = uncle->_col = BLACK;
                    grandfather->_col = RED;
                    // 向上继续调整（祖父变为红色，可能破坏规则3）
                    cur = grandfather;
                    parent = cur->_parent;
                }
                else // 情况2 & 3: 叔叔节点不存在或为黑色
                {
                    if (cur == parent->_left) // 情况2: 当前节点是父节点的左孩子（直线型）
                    {
                        /*
                         *       g(B)                 p(B)
                         *      /                    /    \
                         *    p(R)     ====>       c(R)   g(R)
                         *    /
                         *  c(R)
                         * 操作：对g右单旋，变色p黑g红
                         */
                        RotateR(grandfather);
                        parent->_col = BLACK;
                        grandfather->_col = RED;
                    }
                    else // 情况3: 当前节点是父节点的右孩子（折线型）
                    {
                        /*
                         *     g(B)                 g(B)                c(B)
                         *    /                    /                   /    \
                         *  p(R)     ====>       c(R)    ====>       p(R)   g(R)
                         *    \                  /
                         *    c(R)             p(R)
                         * 操作：先对p左单旋，再对g右单旋，变色c黑g红
                         */
                        RotateL(parent);
                        RotateR(grandfather);
                        cur->_col = BLACK;
                        grandfather->_col = RED;
                    }
                    break; // 调整完成
                }
            }
            else // parent == grandfather->_right 父节点是祖父节点的右孩子
            {
                Node* uncle = grandfather->_left;
                // 情况1: 叔叔节点存在且为红色（对称情况）
                /*
                 *    g(B)                   g(R)
                 *   /    \                 /    \
                 * u(R)   p(R)   ====>    u(B)   p(B)
                 *          \                      \
                 *          c(R)                   c(R)
                 */
                if (uncle && uncle->_col == RED)
                {
                    parent->_col = uncle->_col = BLACK;
                    grandfather->_col = RED;
                    cur = grandfather;
                    parent = cur->_parent;
                }
                else // 情况2 & 3: 叔叔节点不存在或为黑色（对称情况）
                {
                    if (cur == parent->_right) // 情况2: 当前节点是父节点的右孩子（直线型）
                    {
                        /*
                         *  g(B)                       p(B)
                         *     \                      /    \
                         *     p(R)     ====>       g(R)   c(R)
                         *       \
                         *       c(R)
                         * 操作：对g左单旋，变色p黑g红
                         */
                        RotateL(grandfather);
                        parent->_col = BLACK;
                        grandfather->_col = RED;
                    }
                    else // 情况3: 当前节点是父节点的左孩子（折线型）
                    {
                        /*
                         *  g(B)                   g(B)                 c(B)
                         *     \                     \                 /    \
                         *     p(R)    ====>         c(R)   ====>   g(R)   p(R)
                         *    /                       \
                         *  c(R)                     p(R)
                         * 操作：先对p右单旋，再对g左单旋，变色c黑g红
                         */
                        RotateR(parent);
                        RotateL(grandfather);
                        cur->_col = BLACK;
                        grandfather->_col = RED;
                    }
                    break; // 调整完成
                }
            }
        }
        _root->_col = BLACK; // 确保根节点为黑色（规则2）
        return make_pair(iterator(newnode), true);
    }

    // 查找操作：根据键值查找节点
    Node* Find(const K& key)
    {
        Node* cur = _root;
        KeyOfT kot;

        while (cur)
        {
            if (kot(cur->_data) < key) // 当前节点键值小于查找键值
            {
                cur = cur->_right;
            }
            else if (kot(cur->_data) > key) // 当前节点键值大于查找键值
            {
                cur = cur->_left;
            }
            else // 找到键值相等的节点
            {
                return cur;
            }
        }
        return nullptr; // 未找到
    }

    // 左单旋操作
    /*
     * 左单旋场景：当节点右子树比左子树高时使用
     *
     * 旋转前：
     *        parent (p)               parent的高度可能不平衡
     *       /       \
     *     subL      subR (r)          以subR为轴进行左旋
     *             /     \
     *          subRL   subRR
     *
     * 旋转后：
     *         subR (r)
     *        /      \
     *      parent   subRR
     *     /     \
     *   subL   subRL
     *
     * 旋转步骤：
     * 1. 保存subR和subRL
     * 2. parent的右孩子指向subRL
     * 3. subR的左孩子指向parent
     * 4. 更新父节点指针
     */
    void RotateL(Node* parent)
    {
        Node* subR = parent->_right;    // 父节点的右孩子
        Node* subRL = subR->_left;      // 右孩子的左孩子
        // 处理subRL
        parent->_right = subRL;
        if (subRL != nullptr)
            subRL->_parent = parent;
        // 处理parent和subR的关系
        Node* parentParent = parent->_parent;
        subR->_left = parent;
        parent->_parent = subR;
        // 处理subR和原parent父节点的关系
        if (_root == parent) // parent是根节点
        {
            _root = subR;
            subR->_parent = nullptr;
        }
        else
        {
            if (parentParent->_left == parent) // parent是左孩子
            {
                parentParent->_left = subR;
            }
            else // parent是右孩子
            {
                parentParent->_right = subR;
            }
            subR->_parent = parentParent;
        }
    }

    // 右单旋操作
    /*
     * 右单旋场景：当节点左子树比右子树高时使用
     *
     * 旋转前：
     *        parent (p)                 parent的高度可能不平衡
     *       /       \
     *     subL (l)   subR               以subL为轴进行右旋
     *    /     \
     * subLL   subLR
     *
     * 旋转后：
     *         subL (l)
     *        /      \
     *     subLL    parent
     *             /     \
     *          subLR   subR
     *
     * 旋转步骤：
     * 1. 保存subL和subLR
     * 2. parent的左孩子指向subLR
     * 3. subL的右孩子指向parent
     * 4. 更新父节点指针
     */
    void RotateR(Node* parent)
    {
        Node* subL = parent->_left;     // 父节点的左孩子
        Node* subLR = subL->_right;     // 左孩子的右孩子
        // 处理subLR
        parent->_left = subLR;
        if (subLR)
            subLR->_parent = parent;
        // 处理parent和subL的关系
        Node* parentParent = parent->_parent;
        subL->_right = parent;
        parent->_parent = subL;
        // 处理subL和原parent父节点的关系
        if (_root == parent) // parent是根节点
        {
            _root = subL;
            subL->_parent = nullptr;
        }
        else
        {
            if (parentParent->_left == parent) // parent是左孩子
            {
                parentParent->_left = subL;
            }
            else // parent是右孩子
            {
                parentParent->_right = subL;
            }
            subL->_parent = parentParent;
        }
    }

    // 中序遍历打印（需要根据T类型调整打印方式）
    void InOrder()
    {
        _InOrder(_root);
        cout << endl;
    }

    // 验证红黑树是否平衡
    bool IsBalance()
    {
        if (_root == nullptr) // 空树是平衡的
            return true;
        if (_root->_col == RED) // 根节点必须是黑色
        {
            cout << "错误：根节点是红色的" << endl;
            return false;
        }
        // 计算参考值（任意一条路径的黑色节点数）
        int refVal = 0;
        Node* cur = _root;
        while (cur)
        {
            if (cur->_col == BLACK)
            {
                ++refVal;
            }
            cur = cur->_left;  // 一直向左走，计算最左路径的黑色节点数
        }
        int blacknum = 0;
        return Check(_root, blacknum, refVal);
    }

    // 获取树的高度
    int Height()
    {
        return _Height(_root);
    }

    // 获取节点个数
    size_t Size()
    {
        return _Size(_root);
    }

private:
    Node* _root = nullptr; // 根节点指针

    // 递归中序遍历
    void _InOrder(Node* root)
    {
        if (root == nullptr)
            return;

        _InOrder(root->_left);
        // 注意：这里简单打印，如果T是pair，需要特殊处理
        // 实际使用推荐用iterator遍历
        _InOrder(root->_right);
    }

    // 递归检查红黑树性质
    bool Check(Node* root, int blacknum, const int refVal)
    {
        if (root == nullptr) // 到达空节点
        {
            // 规则4：检查每条路径黑色节点数是否相等
            if (blacknum != refVal)
            {
                cout << "错误：存在黑色节点数量不相等的路径" << endl;
                cout << "当前路径黑色节点数：" << blacknum
                     << "，参考值：" << refVal << endl;
                return false;
            }
            return true;
        }

        // 规则3：检查是否出现连续红色节点
        if (root->_col == RED && root->_parent && root->_parent->_col == RED)
        {
            cout << "错误：有连续的红色节点" << endl;
            return false;
        }

        // 统计黑色节点数
        if (root->_col == BLACK)
        {
            ++blacknum;
        }

        // 递归检查左右子树
        return Check(root->_left, blacknum, refVal)
            && Check(root->_right, blacknum, refVal);
    }

    // 递归计算树的高度
    int _Height(Node* root)
    {
        if (root == nullptr)
            return 0;
        int leftHeight = _Height(root->_left);
        int rightHeight = _Height(root->_right);
        return leftHeight > rightHeight ? leftHeight + 1 : rightHeight + 1;
    }

    // 递归计算节点个数
    size_t _Size(Node* root)
    {
        if (root == nullptr)
            return 0;

        return _Size(root->_left) + _Size(root->_right) + 1;
    }
};