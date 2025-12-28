#include<iostream>
using namespace std;

namespace pzh
{
    template <class K>
    struct BSTreeNode
    {
        BSTreeNode<K>* _left;
        BSTreeNode<K>* _right;
        K _key;

        // 构造函数，初始化节点
        BSTreeNode(const K& key)
            : _left(nullptr)
              , _right(nullptr)
              , _key(key)
        {}
    };

    template <class K>
    class BSTree
    {
        typedef BSTreeNode<K> Node;

    public:
        bool Insert(const K& key) //插入函数
        {
            if (_root == nullptr)
            {
                _root = new Node(key);
                return true;
            }
            Node* parent = nullptr;
            Node* cur = _root;  // 当前节点从根开始
            while (cur != nullptr)
            {
                parent = cur;
                if (cur->_key < key)  // 插入的值大于当前值
                {
                    cur = cur->_right;  // 向右子树移动
                }
                else if (cur->_key > key)
                {
                    cur = cur->_left;
                }
                else
                {
                    return false;  // 关键值已存在，插入失败
                }
            }
            cur = new Node(key);
            if (parent->_key < key)  // 将新节点连接到父节点
            {
                parent->_right = cur;
            }
            else
            {
                parent->_left = cur;
            }
            return true;
        }

        bool Find(const K& key) //查找函数
        {
            Node* cur = _root;
            while (cur)
            {
                if (cur->_key < key)
                {
                    cur = cur->_right;
                }
                else if (cur->_key > key)
                {
                    cur = cur->_left;
                }
                else
                {
                    return true;
                }
            }
            return false;
        }

        // 删除函数（非递归版本）
        bool Erase(const K& key)
        {
            Node* parent = nullptr;
            Node* cur = _root;
            while (cur)  // 寻找要删除的节点
            {
                if (cur->_key < key)
                {
                    parent = cur;
                    cur = cur->_right;
                }
                else if (cur->_key > key)
                {
                    parent = cur;
                    cur = cur->_left;
                }
                else  // 找到了要删除的节点，分三种情况处理
                {
                    if (cur->_left == nullptr)  // 情况1：删除的节点左子节点为空
                    {
                        if (cur == _root)
                        {
                            _root = cur->_right;
                        }
                        else
                        {
                            // 将父节点的相应指针指向当前节点的右子节点
                            if (cur == parent->_left)
                            {
                                parent->_left = cur->_right;
                            }
                            else
                            {
                                parent->_right = cur->_right;
                            }
                        }
                        delete cur;
                    }
                    else if (cur->_right == nullptr)  // 情况2：删除的节点右子节点为空
                    {
                        if (cur == _root)
                        {
                            _root = cur->_left;
                        }
                        else
                        {
                            if (cur == parent->_left)
                            {
                                parent->_left = cur->_left;
                            }
                            else
                            {
                                parent->_right = cur->_left;
                            }
                        }
                        delete cur;
                    }
                    else  // 情况3：节点左右子节点都不为空
                    {
                        Node* parent = cur;
                        Node* subLeft = cur->_right;  // 从当前节点的右子树开始，subLeft初始指向右子树的根节点
                        // 循环找到右子树中的最小节点（最左节点）
                        while (subLeft->_left)
                        {
                            parent = subLeft; // parent始终记录subLeft的父节点
                            subLeft = subLeft->_left;
                        }
                        swap(cur->_key, subLeft->_key);  // 把后继节点的值给cur，相当于“间接删除cur”
                        // 删除原本subLeft处的节点本身
                        /* subLeft是cur右子树的最左节点，但当cur的右子树本身没有左分支时
                         * 即cur->right自己就是右子树的最小节点，while循环不会执行，此时就会出现subLeft是父节点右孩子的情况。*/
                        if (subLeft == parent->_left)
                            parent->_left = subLeft->_right;
                        else
                            parent->_right = subLeft->_right;
                        delete subLeft;
                    }
                    return true;
                }
            }
            return false;
        }

        //类外面无法传根，类里面可以（c++写递归函数的逻辑）
        void InOrder()
        {
            _InOrder(_root);
            cout << endl;
        }

        bool FindR(const K& key) // R代表递归版本
        {
            return _FindR(_root, key);
        }

        bool InsertR(const K& key)
        {
            return _InsertR(_root, key);
        }

        bool EraseR(const K& key)
        {
            return _EraseR(_root, key);
        }

        ///////////////////////////////////////////////////////////////
        // 特殊成员函数
        BSTree() = default;  // 强制生成默认构造函数

        ~BSTree() //析构
        {
            Destroy(_root);
        }

        // 拷贝构造函数（深拷贝）
        BSTree(const BSTree<K>& t)
        {
            _root = Copy(t._root);  // 复制整棵树
        }

        // t1 = t3
        // 赋值运算符重载（采用拷贝交换）
        BSTree<K>& operator=(BSTree<K> t)
        {
            swap(_root, t._root);
            return *this;
        }

    private:
        // 删除函数（递归实现）
        bool _EraseR(Node*& root, const K& key)
        {
            if (root == nullptr)
                return false;
            // 递归查找要删除的节点
            if (root->_key < key)
            {
                return _EraseR(root->_right, key);
            }
            else if (root->_key > key)
            {
                return _EraseR(root->_left, key);
            }
            else   // 找到要删除的节点，分三种情况处理
            {
                if (root->_left == nullptr)  // 情况1：节点左子节点为空
                {
                    Node* del = root;
                    root = root->_right;
                    delete del;
                    return true;
                }
                else if (root->_right == nullptr)  // 情况2：节点右子节点为空
                {
                    Node* del = root;
                    root = root->_left;
                    delete del;
                    return true;
                }
                else  // 情况3：节点左右子节点都不为空
                {
                    Node* subLeft = root->_right;
                    while (subLeft->_left)
                    {
                        subLeft = subLeft->_left;
                    }
                    swap(root->_key, subLeft->_key);
                    // 递归删除右子树中的后继节点
                    return _EraseR(root->_right, key);
                }
            }
        }

        bool _InsertR(Node*& root, const K& key)
        {
            if (root == nullptr)
            {
                root = new Node(key);
                return true;
            }
            // 递归寻找插入位置
            if (root->_key < key)
                return _InsertR(root->_right, key);
            else if (root->_key > key)
                return _InsertR(root->_left, key);
            else
                return false;
        }

        bool _FindR(Node* root, const K& key)
        {
            if (root == nullptr)
            {
                return false;
            }
            if (root->_key < key)
            {
                return _FindR(root->_right, key);
            }
            else if (root->_key > key)
            {
                return _FindR(root->_left, key);
            }
            else
            {
                return true;
            }
        }

        void _InOrder(Node* root) //中序遍历函数
        {
            if (root == nullptr)
                return;
            _InOrder(root->_left);
            cout << root->_key << " ";
            _InOrder(root->_right);
        }

        ///////////////////////////////////////////////////////////////
        // 复制树（深拷贝）
        Node* Copy(Node* root)
        {
            if (root == nullptr)
                return nullptr;
            Node* newRoot = new Node(root->_key);
            newRoot->_left = Copy(root->_left);
            newRoot->_right = Copy(root->_right);
            return newRoot;
        }

        // 销毁树（后序遍历释放内存）
        void Destroy(Node*& root)
        {
            if (root == nullptr)
                return;
            Destroy(root->_left);
            Destroy(root->_right);
            delete root;
            root = nullptr;
        }

    private:
        Node* _root = nullptr;
    };
}

// 键值对版本的二叉搜索树命名空间
namespace kv
{
    template <class K, class V>
    struct BSTreeNode
    {
        BSTreeNode<K, V>* _left;   // 左子节点指针
        BSTreeNode<K, V>* _right;  // 右子节点指针
        K _key;                    // 键
        V _value;                  // 值

        BSTreeNode(const K& key, const V& value)
            : _left(nullptr)
            , _right(nullptr)
            , _key(key)
            , _value(value)
        {}
    };

    // 键值对二叉搜索树模板类
    template <class K, class V>
    class BSTree
    {
        typedef BSTreeNode<K, V> Node;

    public:
        bool Insert(const K& key, const V& value)
        {
            if (_root == nullptr)
            {
                _root = new Node(key, value);
                return true;
            }
            Node* parent = nullptr;
            Node* cur = _root;
            while (cur != nullptr)
            {
                parent = cur;
                if (cur->_key < key)
                {
                    cur = cur->_right;
                }
                else if (cur->_key > key)
                {
                    cur = cur->_left;
                }
                else
                {
                    return false;
                }
            }
            // 创建新节点并连接到父节点
            cur = new Node(key, value);
            if (parent->_key < key)
            {
                parent->_right = cur;
            }
            else
            {
                parent->_left = cur;
            }
            return true;
        }

        // 查找键对应的节点
        Node* Find(const K& key)
        {
            Node* cur = _root;
            while (cur != nullptr)
            {
                if (cur->_key < key)
                {
                    cur = cur->_right;
                }
                else if (cur->_key > key)
                {
                    cur = cur->_left;
                }
                else
                {
                    return cur;
                }
            }
            return nullptr;
        }

        // 删除键值对
        bool Erase(const K& key)
        {
            Node* parent = nullptr;
            Node* cur = _root;
            while (cur)
            {
                if (cur->_key < key)
                {
                    parent = cur;
                    cur = cur->_right;
                }
                else if (cur->_key > key)
                {
                    parent = cur;
                    cur = cur->_left;
                }
                else  // 找到要删除的节点，分三种情况处理
                {
                    if (cur->_left == nullptr)  // 情况1：左子节点为空
                    {
                        if (cur == _root)
                        {
                            _root = cur->_right;
                        }
                        else
                        {
                            if (cur == parent->_left)
                            {
                                parent->_left = cur->_right;
                            }
                            else
                            {
                                parent->_right = cur->_right;
                            }
                        }
                    }
                    else if (cur->_right == nullptr)  // 情况2：右子节点为空
                    {
                        if (cur == _root)
                        {
                            _root = cur->_left;
                        }
                        else
                        {
                            if (cur == parent->_left)
                            {
                                parent->_left = cur->_left;
                            }
                            else
                            {
                                parent->_right = cur->_left;
                            }
                        }
                    }
                    else    // 情况3：左右子节点都不为空
                    {
                        // 找到右子树的最小节点(后继节点)
                        Node* parent = cur;
                        Node* subLeft = cur->_right;
                        while (subLeft->_left)  // 寻找最左节点
                        {
                            parent = subLeft;
                            subLeft = subLeft->_left;
                        }
                        swap(cur->_key, subLeft->_key);
                        if (subLeft == parent->_left)
                            parent->_left = subLeft->_right;
                        else
                            parent->_right = subLeft->_right;
                    }
                    return true;
                }
            }
            return false;   // 未找到要删除的键
        }

        void InOrder()
        {
            _InOrder(_root);
            cout << endl;
        }

    private:
        void _InOrder(Node* root)
        {
            if (root == nullptr)
                return;
            _InOrder(root->_left);
            cout << root->_key << ":" << root->_value << endl;
            _InOrder(root->_right);
        }

    private:
        Node* _root = nullptr;
    };
}
