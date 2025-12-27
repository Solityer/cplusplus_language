#include<assert.h>

template<class K, class V>
struct AVLTreeNode
{
	AVLTreeNode<K, V>* _left;
	AVLTreeNode<K, V>* _right;
	AVLTreeNode<K, V>* _parent;
	// std::pair<K, V>是C++标准库提供的一种模板类，用于将两个值组合成一个单一的对象
	// 它的主要作用是方便地同时操作或传递两个相关联的数据项
	pair<K, V> _kv;
	int _bf; // 平衡因子

	AVLTreeNode(const pair<K, V>& kv)
		:_left(nullptr)
		,_right(nullptr)
		,_parent(nullptr)
		,_kv(kv)
		,_bf(0)
	{}
};

template<class K, class V>
class AVLTree
{
	typedef AVLTreeNode<K, V> Node;
public:
	bool Insert(const pair<K, V>& kv)
	{
		if (_root == nullptr)
		{
			_root = new Node(kv);
			return true;
		}
		Node* parent = nullptr;
		Node* cur = _root;
		while (cur)
		{
			// 当前节点的值大于要插入节点的值
			if (cur->_kv.first < kv.first)
			{
				parent = cur;
				cur = cur->_right;
			}
			// 小于
			else if (cur->_kv.first > kv.first)
			{
				parent = cur;
				cur = cur->_left;
			}
			// 等于
			else
			{
				return false;
			}
		}

		// 为新插入的值创建节点
		cur = new Node(kv);
		if (parent->_kv.first < kv.first)
		{
			parent->_right = cur;
			cur->_parent = parent;
		}
		else
		{
			parent->_left = cur;
			cur->_parent = parent;
		}

		// 调整节点的平衡因子
		while (parent)
		{
			if (cur == parent->_left)
			{
				parent->_bf--;
			}
			else
			{
				parent->_bf++;
			}

			if (parent->_bf == 0)
			{
				break;
			}
			// 当前节点构成平衡，继续向上调整
			else if(parent->_bf == 1 || parent->_bf == -1)
			{
				cur = parent;
				parent = parent->_parent;
			}
			// 无法构成平衡
			else if (parent->_bf == 2 || parent->_bf == -2)
			{
				// 父节点右子树 = 左子树 + 2，子节点右子树 = 左子树 + 1
				if (parent->_bf == 2 && cur->_bf == 1)
				{
					// 右子树太深，左单旋
					RotateL(parent);
				}
				// 父节点左子树 = 右子树 + 2，子节点左子树 = 右子树 + 1
				else if (parent->_bf == -2 && cur->_bf == -1)
				{
					// 左子树太深，右单旋
					RotateR(parent);
				}
				// 父节点右子树 = 左子树 + 2，子节点左子树 = 右子树 + 1
				else if (parent->_bf == 2 && cur->_bf == -1)
				{
					RotateRL(parent);
				}
				// 父节点左子树 = 右子树 + 2，子节点右子树 = 左子树 + 1
				else if (parent->_bf == -2 && cur->_bf == 1)
				{
					RotateLR(parent);
				}
				// 1、旋转让这颗子树平衡了
				// 2、旋转降低了这颗子树的高度，恢复到跟插入前一样的高度，所以对上一层没有影响，不用继续更新
				break;
			}
			else
			{
				assert(false);
			}
		}
		return true;
	}

	// 左单旋
	void RotateL(Node* parent)
	{
		// 核心流程
		Node* subR = parent->_right;
		Node* subRL = subR->_left;
		parent->_right = subRL;
		subR->_left = parent;

		// 处理节点依赖关系
		// 当前子树的根的父节点
		Node* parentParent = parent->_parent;
		parent->_parent = subR;
		if(subRL != nullptr)
			subRL->_parent = parent;

		// 当前旋转的子树的根是整个树的根
		if (_root == parent)
		{
			_root = subR;
			subR->_parent = nullptr;
		}
		else
		{
			if (parentParent->_left == parent)
			{
				parentParent->_left = subR;
			}
			else
			{
				parentParent->_right = subR;
			}
			subR->_parent = parentParent;
		}
		parent->_bf = subR->_bf = 0;
	}

	// 右单旋
	void RotateR(Node* parent)
	{
		Node* subL = parent->_left;
		Node* subLR = subL->_right;

		parent->_left = subLR;
		if (subLR != nullptr)
			subLR->_parent = parent;

		Node* parentParent = parent->_parent;
		subL->_right = parent;
		parent->_parent = subL;
		if (_root == parent)
		{
			_root = subL;
			subL->_parent = nullptr;
		}
		else
		{
			if (parentParent->_left == parent)
			{
				parentParent->_left = subL;
			}
			else
			{
				parentParent->_right = subL;
			}
			subL->_parent = parentParent;
		}
		subL->_bf = parent->_bf = 0;
	}

	// 先右单旋(小子树)再左单旋(大子树)
	void RotateRL(Node* parent)
	{
		Node* subR = parent->_right;
		Node* subRL = subR->_left;
		int bf = subRL->_bf;

		RotateR(parent->_right);
		RotateL(parent);

		if (bf == 0)
		{
			// subRL自己就是新增
			parent->_bf = subR->_bf = subRL->_bf = 0;
		}
		else if (bf == -1)
		{
			// subRL的左子树新增
			parent->_bf = 0;
			subRL->_bf = 0;
			subR->_bf = 1;
		}
		else if (bf == 1)
		{
			// subRL的右子树新增
			parent->_bf = -1;
			subRL->_bf = 0;
			subR->_bf = 0;
		}
		else
		{
			assert(false);
		}
	}

	// 先左单旋再右单旋
	void RotateLR(Node* parent)
	{
		RotateL(parent->_left);
		RotateR(parent);
	}


	// 中序遍历
	void InOrder()
	{
		_InOrder(_root);
		cout << endl;
	}

	void _InOrder(Node* root)
	{
		if (root == nullptr)
			return;

		_InOrder(root->_left);
		cout << root->_kv.first << " ";
		_InOrder(root->_right);
	}

	int _Height(Node* root)
	{
		if (root == nullptr)
			return 0;

		int leftHeight = _Height(root->_left);
		int rightHeight = _Height(root->_right);
		return leftHeight > rightHeight ? leftHeight + 1 : rightHeight + 1;
	}

	bool IsBalance()
	{
		return _IsBalance(_root);
	}

	bool _IsBalance(Node* root)
	{
		if (root == nullptr)
			return true;

		int leftHeight = _Height(root->_left);
		int rightHeight = _Height(root->_right);
		if (rightHeight - leftHeight != root->_bf)
		{
			cout << root->_kv.first << "平衡因子异常" << endl;
			return false;
		}
		return abs(rightHeight - leftHeight) < 2
			&& _IsBalance(root->_left)
			&& _IsBalance(root->_right);
	}

private:
	Node* _root = nullptr;
};
