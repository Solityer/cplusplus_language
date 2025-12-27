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

        BSTreeNode(const K& key)
            : _left(nullptr)
              , _right(nullptr)
              , _key(key)
        {
        }
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
            Node* cur = _root;
            while (cur)
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
            cur = new Node(key);
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

        bool Erase(const K& key) //删除函数
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
                else
                {
                    // 删除的节点左为空的情况
                    if (cur->_left == nullptr)
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
                        delete cur;
                    }
                    else if (cur->_right == nullptr)
                    {
                        // 删除的节点右为空的情况
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
                    else
                    {
                        // 删除的节点左右都不为空的情况
                        // 右树的最小节点(最左节点)
                        Node* parent = cur;
                        // 从当前节点的右子树开始，寻找中序遍历的后继节点
                        // subLeft初始指向右子树的根节点
                        Node* subLeft = cur->_right;
                        // 循环找到右子树中的最小节点（最左节点）
                        while (subLeft->_left)
                        {
                            parent = subLeft; // parent始终记录subLeft的父节点
                            subLeft = subLeft->_left;
                        }
                        swap(cur->_key, subLeft->_key);
                        // 如果后继节点是其父节点的左孩子，将父节点的左指针指向后继节点的右孩子
                        // 如果后继节点是其父节点的右孩子，将父节点的右指针指向后继节点的右孩子
                        // 注意：后继节点不可能有左孩子（因为它是子树中的最左节点）
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
        // 强制生成默认的构造函数
        BSTree() = default;

        ~BSTree() //析构
        {
            Destroy(_root);
        }

        BSTree(const BSTree<K>& t)
        {
            _root = Copy(t._root);
        }

        // t1 = t3
        // 赋值  t就是t3的拷贝
        BSTree<K>& operator=(BSTree<K> t)
        {
            swap(_root, t._root);
            return *this;
        }

    private:
        bool _EraseR(Node*& root, const K& key)
        {
            if (root == nullptr)
                return false;
            if (root->_key < key)
            {
                return _EraseR(root->_right, key);
            }
            else if (root->_key > key)
            {
                return _EraseR(root->_left, key);
            }
            else
            {
                // 删除
                if (root->_left == nullptr)
                {
                    Node* del = root;
                    root = root->_right;
                    delete del;
                    return true;
                }
                else if (root->_right == nullptr)
                {
                    Node* del = root;
                    root = root->_left;
                    delete del;
                    return true;
                }
                else
                {
                    Node* subLeft = root->_right;
                    while (subLeft->_left)
                    {
                        subLeft = subLeft->_left;
                    }
                    swap(root->_key, subLeft->_key);
                    // 转换成在子树去递归删除
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
        Node* Copy(Node* root) //拷贝
        {
            if (root == nullptr)
                return nullptr;
            Node* newRoot = new Node(root->_key);
            newRoot->_left = Copy(root->_left);
            newRoot->_right = Copy(root->_right);
            return newRoot;
        }

        void Destroy(Node*& root) //销毁
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

//namespace key_value 通过k查找v
namespace kv
{
    template <class K, class V>
    struct BSTreeNode
    {
        BSTreeNode<K, V>* _left;
        BSTreeNode<K, V>* _right;
        K _key;
        V _value;

        BSTreeNode(const K& key, const V& value)
            : _left(nullptr)
              , _right(nullptr)
              , _key(key)
              , _value(value)
        {}
    };

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
            while (cur)
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

        Node* Find(const K& key)
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
                    return cur;
                }
            }
            return nullptr;
        }

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
                else
                {
                    // 准备删除  20:15继续
                    if (cur->_left == nullptr)
                    {
                        //左为空
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
                    else if (cur->_right == nullptr)
                    {
                        //右为空
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
                    else
                    {
                        //左右都不为空
                        // 右树的最小节点(最左节点)
                        Node* parent = cur;
                        Node* subLeft = cur->_right;
                        while (subLeft->_left)
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
            return false;
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
