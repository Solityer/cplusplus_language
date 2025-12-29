#include<assert.h>

template<class K, class V>
struct AVLTreeNode
{
	AVLTreeNode<K, V>* _left;
	AVLTreeNode<K, V>* _right;
	AVLTreeNode<K, V>* _parent;
	// std::pair<K, V>是C++标准库提供的一种模板类，用于将两个值组合成一个单一的对象
	// 它的主要作用是方便地同时操作或传递两个相关联的数据项
	pair<K, V> _kv;   // 键值对，存储节点的数据
	int _bf; // 平衡因子

	// 构造函数
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
			// 当前节点的键值小于要插入的键值，向右子树查找
			if (cur->_kv.first < kv.first)
			{
				parent = cur;
				cur = cur->_right;
			}
			// 当前节点的键值大于要插入的键值，向左子树查找
			else if (cur->_kv.first > kv.first)
			{
				parent = cur;
				cur = cur->_left;
			}
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

		// 调整节点的平衡因子，从插入点的父节点开始向上调整
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
			// 检查父节点的平衡因子状态
			if (parent->_bf == 0)      // 平衡因子为0，高度未变，调整结束
			{
				break;
			}
			// 平衡因子为1或-1，子树高度增加，需要继续向上调整
			else if(parent->_bf == 1 || parent->_bf == -1)
			{
				cur = parent;
				parent = parent->_parent;
			}
			// 平衡因子为2或-2，需要进行旋转调整
			else if (parent->_bf == 2 || parent->_bf == -2)
			{
				// 情况1：右子树比左子树高2，且右子树的右子树比左子树高1（右右情况）
				// 父节点右子树 = 左子树 + 2，子节点右子树 = 左子树 + 1
				if (parent->_bf == 2 && cur->_bf == 1)
				{
					// 右子树太深，左单旋
					RotateL(parent);
				}
				// 情况2：左子树比右子树高2，且左子树的左子树比右子树高1（左左情况）
				// 父节点左子树 = 右子树 + 2，子节点左子树 = 右子树 + 1
				else if (parent->_bf == -2 && cur->_bf == -1)
				{
					// 左子树太深，右单旋
					RotateR(parent);
				}
				// 情况3：右子树比左子树高2，且右子树的左子树比右子树高1（右左情况）
				// 父节点右子树 = 左子树 + 2，子节点左子树 = 右子树 + 1
				else if (parent->_bf == 2 && cur->_bf == -1)
				{
					// 先右旋再左旋（右左双旋）
					RotateRL(parent);
				}
				// 情况4：左子树比右子树高2，且左子树的右子树比左子树高1（左右情况）
				// 父节点左子树 = 右子树 + 2，子节点右子树 = 左子树 + 1
				else if (parent->_bf == -2 && cur->_bf == 1)
				{
					// 先左旋再右旋（左右双旋）
					RotateLR(parent);
				}
				// 旋转完成后：
				// 1、旋转让这颗子树平衡了
				// 2、旋转降低了这颗子树的高度，恢复到跟插入前一样的高度，所以对上一层没有影响，不用继续更
				break;
			}
			else  // 平衡因子出现非法值（不是-2,-1,0,1,2）
			{
				assert(false);  // 触发断言，程序终止
			}
		}
		return true;
	}

	// 左单旋（处理右右不平衡情况）
	void RotateL(Node* parent)
	{
	    /* ================================
	       旋转前结构示意图：
	        parent (bf = 2)
	        /     \
	       A     subR (bf = 1)
	            /   \
	        subRL    C
	    条件：parent->_bf == 2 && subR->_bf == 1
	    ================================ */

	    Node* subR = parent->_right;    // 父节点的右子节点（将成为新的根）
	    Node* subRL = subR->_left;      // 右子节点的左子节点

	    /* ================================
	       步骤1：核心旋转操作
	       将parent的右指针指向subRL：
	            parent
	            /    \
	           A     subRL
	                subR
	                /   \
	            [subRL]  C

	       将subR的左指针指向parent：
	            subR
	            /   \
	        parent   C
	         /  \
	        A   [subRL]
	    ================================ */
	    parent->_right = subRL;         // 父节点的右子节点指向subRL
	    subR->_left = parent;           // subR的左子节点指向父节点

	    /* ================================
	       步骤2：更新父节点关系
	       旋转后临时结构：

	            subR
	            /   \
	        parent   C
	         /  \
	        A   subRL
	    ================================ */
	    Node* parentParent = parent->_parent;  // 保存父节点的父节点
	    parent->_parent = subR;         // parent的父节点指向subR
	    if(subRL != nullptr)           // 如果subRL存在
	        subRL->_parent = parent;   // subRL的父节点指向parent

	    /* ================================
	       步骤3：更新树的连接
	       最终旋转后结构：

	            subR (新根)
	            /     \
	        parent     C
	         /  \
	        A   subRL
	    ================================ */
	    if (_root == parent)           // parent是整棵树的根节点
	    {
	        _root = subR;              // 更新根节点为subR
	        subR->_parent = nullptr;   // 根节点的父节点为空
	    }
	    else  // parent不是根节点
	    {
	        if (parentParent->_left == parent)  // parent是左子节点
	        {
	            parentParent->_left = subR;     // 祖父节点的左指针指向subR
	        }
	        else                                // parent是右子节点
	        {
	            parentParent->_right = subR;    // 祖父节点的右指针指向subR
	        }
	        subR->_parent = parentParent;       // 更新subR的父节点
	    }

	    /* ================================
	       步骤4：更新平衡因子
	       旋转后所有节点都平衡：

	            subR (bf = 0)
	            /     \
	        parent (bf = 0) C
	         /  \
	        A   subRL
	    ================================ */
	    parent->_bf = subR->_bf = 0;    // 旋转后两个节点的平衡因子都为0
	}

    // 右单旋（处理左左不平衡情况）
	void RotateR(Node* parent)
	{
	    /* ================================
	       旋转前结构示意图：
	            parent (bf = -2)
	            /     \
	        subL (bf = -1)  A
	         /   \
	        B     subLR
	    条件：parent->_bf == -2 && subL->_bf == -1
	    ================================ */
	    Node* subL = parent->_left;     // 父节点的左子节点（将成为新的根）
	    Node* subLR = subL->_right;     // 左子节点的右子节点

	    /* ================================
	       步骤1：处理subLR的连接
	       将parent的左指针指向subLR：
	            parent
	            /    \
	        subLR     A
	        subL
	         /   \
	        B    [subLR]
	    ================================ */
	    parent->_left = subLR;          // 父节点的左子节点指向subLR
	    if (subLR != nullptr)           // 如果subLR存在
	        subLR->_parent = parent;    // 更新subLR的父节点

	    /* ================================
	       步骤2：处理parent的连接
	       将subL的右指针指向parent：
	            subL
	            /   \
	           B   parent
	               /    \
	            [subLR]  A
	    ================================ */
	    Node* parentParent = parent->_parent;  // 保存父节点的父节点
	    subL->_right = parent;          // subL的右子节点指向父节点
	    parent->_parent = subL;         // 更新父节点的父节点

	    /* ================================
	       步骤3：更新树的连接
	       旋转后结构：
	            subL (新根)
	            /     \
	           B     parent
	                 /    \
	             subLR     A
	    ================================ */
	    if (_root == parent)            // 父节点是根节点
	    {
	        _root = subL;               // 更新根节点为subL
	        subL->_parent = nullptr;    // 根节点的父节点为空
	    }
	    else  // 不是根节点
	    {
	        if (parentParent->_left == parent)  // 父节点原来是左子节点
	        {
	            parentParent->_left = subL;     // 祖父节点的左子节点指向subL
	        }
	        else                                // 父节点原来是右子节点
	        {
	            parentParent->_right = subL;    // 祖父节点的右子节点指向subL
	        }
	        subL->_parent = parentParent;       // 更新subL的父节点
	    }

	    /* ================================
	       步骤4：更新平衡因子
	       旋转后所有节点都平衡：
	            subL (bf = 0)
	            /     \
	           B     parent (bf = 0)
	                 /    \
	             subLR     A
	    ================================ */
	    subL->_bf = parent->_bf = 0;    // 旋转后两个节点的平衡因子都为0
	}

	// 先右单旋再左单旋（处理右左不平衡情况）
	void RotateRL(Node* parent)
	{
	    Node* subR = parent->_right;    // 父节点的右子节点
	    Node* subRL = subR->_left;      // 右子节点的左子节点（将成为新的根）
	    int bf = subRL->_bf;            // 保存subRL的原始平衡因子
	    /* ============================================
	       旋转前的结构示意图（右左不平衡）：
	                parent (bf = 2)
	                /     \
	              A      subR (bf = -1)
	                    /   \
	               subRL     C
	               /   \
	              B     D
	    ============================================ */

	    // 第一步：对subR进行右单旋
	    RotateR(parent->_right);
	    /* ============================================
	       第一次旋转后的结构示意图：
	                parent (bf = 2)
	                /     \
	              A      subRL (bf = 0)
	                    /     \
	                   B      subR
	                         /   \
	                        D     C
	    ============================================ */

	    // 第二步：对parent进行左单旋
	    RotateL(parent);
	    /* ============================================
	       第二次旋转后的结构示意图：
	                subRL (bf = 0)
	                /     \
	           parent      subR
	           /    \     /   \
	          A      B   D     C
	    ============================================ */

	    // 根据subRL的原始平衡因子更新平衡因子
	    if (bf == 0)
	    {
	        /* ============================================
	           情况1：subRL自身是新增节点
	           旋转前结构（数字示例：插入3, 1, 5, 4）：

	                   3 (bf = 2)
	                  / \
	                 1   5 (bf = -1)
	                    /
	                   4 (新增，bf = 0)

	           旋转后结构：

	                   4 (bf = 0)
	                  / \
	                 3   5 (bf = 0)
	                /
	               1 (bf = 0)

	           平衡因子更新：
	           parent(3)->_bf = 0
	           subR(5)->_bf = 0
	           subRL(4)->_bf = 0
	        ============================================ */
	        parent->_bf = subR->_bf = subRL->_bf = 0;
	    }
	    else if (bf == -1)
	    {
	        /* ============================================
	           情况2：在subRL的左子树新增节点
	           旋转前结构：

	                   3 (bf = 2)
	                  / \
	                 1   5 (bf = -1)
	                    /
	                   4 (bf = -1)
	                  /
	                 3.5 (新增)

	           旋转后结构：

	                   4 (bf = 0)
	                  / \
	                 3   5 (bf = 1)
	                / \   \
	               1 3.5   null
	        ============================================ */
	        parent->_bf = 0;    // 父节点平衡
	        subRL->_bf = 0;     // subRL平衡
	        subR->_bf = 1;      // subR的右子树更高（因为左子树D的高度为0）
	    }
	    else if (bf == 1)
	    {
	        /* ============================================
	           情况3：在subRL的右子树新增节点
	           示例：插入3, 1, 5, 4, 4.5（或任何大于4小于5的值）
	           旋转前结构：

	                   3 (bf = 2)
	                  / \
	                 1   5 (bf = -1)
	                    /
	                   4 (bf = 1)
	                    \
	                     4.5 (新增)

	           旋转后结构：

	                   4 (bf = 0)
	                  / \
	                 3   5 (bf = 0)
	                /   / \
	               1   4.5 null
	        ============================================ */
	        parent->_bf = -1;   // 父节点的左子树更高（因为右子树B的高度较低）
	        subRL->_bf = 0;     // subRL平衡
	        subR->_bf = 0;      // subR平衡
	    }
	    else  // 非法平衡因子
	    {
	        // AVL树中平衡因子只能是-1、0、1
	        // 出现其他值说明代码逻辑有错误
	        assert(false);
	    }
	}

	// 先左单旋再右单旋（处理左右不平衡情况）
	void RotateLR(Node* parent)
	{
		Node* subL = parent->_left;     // parent的左子节点
		Node* subLR = subL->_right;     // subL的右子节点（将成为新的根）
		// 保存subLR的原始平衡因子，用于后续判断插入位置
		int bf = subLR->_bf;
		// 第一步：对subL进行左单旋
		RotateL(parent->_left);
		// 第二步：对parent进行右单旋
		RotateR(parent);
		// 根据subLR的原始平衡因子更新相关节点的平衡因子
		if (bf == 0)
		{
			// 情况1：subLR自己是新增节点
			// 旋转后所有相关节点都平衡
			parent->_bf = 0;    // parent的左右子树高度相等
			subL->_bf = 0;      // subL的左右子树高度相等
			subLR->_bf = 0;     // subLR的左右子树高度相等
		}
		else if (bf == -1)
		{
			// 情况2：在subLR的左子树新增节点
			// 示例：在subLR的左子节点C插入
			/*
					 parent
					/      \
				  subL      A
				 /    \
				B    subLR
					/
				   C (新增)
			*/
			parent->_bf = 1;    // parent的右子树比左子树高1（D在右子树）
			subL->_bf = 0;      // subL的左右子树高度相等
			subLR->_bf = 0;     // subLR的左右子树高度相等
		}
		else if (bf == 1)
		{
			// 情况3：在subLR的右子树新增节点
			// 示例：在subLR的右子节点D插入
			/*
					 parent
					/      \
				  subL      A
				 /    \
				B    subLR
					  \
					   D (新增)
			*/
			parent->_bf = 0;    // parent的左右子树高度相等
			subL->_bf = -1;     // subL的左子树比右子树高1（B在左子树）
			subLR->_bf = 0;     // subLR的左右子树高度相等
		}
		else
		{
			// 平衡因子只能是-1、0、1，其他值是非法的
			assert(false);
		}
	}

    // 中序遍历（外部接口）
    void InOrder()
    {
        _InOrder(_root);        // 调用内部递归函数
        cout << endl;
    }

	// 中序遍历（内部递归实现）
	void _InOrder(Node* root)
	{
		if (root == nullptr)
			return;
		_InOrder(root->_left);
		cout << root->_kv.first << " ";
		_InOrder(root->_right);
	}

	// 计算树的高度
	int _Height(Node* root)
	{
		if (root == nullptr)
			return 0;
		int leftHeight = _Height(root->_left);
		int rightHeight = _Height(root->_right);
		return leftHeight > rightHeight ? leftHeight + 1 : rightHeight + 1;
	}

	// 检查AVL树是否平衡（外部接口）
	bool IsBalance()
	{
		return _IsBalance(_root);
	}

	// 检查AVL树是否平衡（内部递归实现）
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
