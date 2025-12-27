#include <iostream>
using namespace std;
#include <list>
#include <vector>
namespace pzh
{
	template <class T>
	struct list_node // 双向链表的节点结构
	{
		T _data;                    // 存储的数据，类型为T
		list_node<T> *_next;        // 指向下一个节点的指针，类型为list_node<T>*
		list_node<T> *_prev;
		// T不一定是int，需要给个匿名对象
		// 匿名对象是指不具名的临时对象，它的生命周期通常仅限于创建它的表达式
		list_node(const T &x = T()) // 构造函数，参数为const T&，默认值为T类型的默认构造值
			: _data(x)
			, _next(nullptr)
			, _prev(nullptr)
		{}
	};

	template <class T, class Ref, class Ptr> // 模板参数：T=类型，Ref=T&(引用类型)，Ptr=T*(指针类型)
	struct __list_iterator	// 迭代器类，实现链表元素的遍历
	{
		typedef list_node<T> Node;   // 节点类型别名
		typedef __list_iterator<T, Ref, Ptr> self;  // 自身类型别名
		Node *_node;  // 当前指向的节点指针

		__list_iterator(Node *node)  // 构造函数，接受一个节点指针
			: _node(node)
		{}

		self &operator++()   // 前置++运算符重载，没有形参是前置++
		{
			_node = _node->_next;
			return *this;
		}

		self &operator--()
		{
			_node = _node->_prev;
			return *this;
		}

		self operator++(int)  // 后置++运算符重载，int参数用于区分前置++
		{
			self tmp(*this);
			_node = _node->_next;
			return tmp;
		}

		self operator--(int)
		{
			self tmp(*this);
			_node = _node->_prev;
			return tmp;
		}

		// 解引用操作符
		Ref operator*()
		{
			return _node->_data;
		}

		Ptr operator->()
		{
			return &_node->_data;   // 返回节点数据的地址
		}

		bool operator!=(const self &s)
		{
			return _node != s._node;
		}

		bool operator==(const self &s)
		{
			return _node == s._node;
		}
	};

	template <class T>
	struct __list_const_iterator // const迭代器
	{
		typedef list_node<T> Node;
		typedef __list_const_iterator<T> self;
		Node *_node;

		__list_const_iterator(Node *node) // 构造函数
			: _node(node)
		{}

		self &operator++()
		{
			_node = _node->_next;
			return *this;
		}

		self &operator--()
		{
			_node = _node->_prev;
			return *this;
		}

		self operator++(int)
		{
			self tmp(*this);
			_node = _node->_next;
			return tmp;
		}

		self operator--(int)
		{
			self tmp(*this);
			_node = _node->_prev;
			return tmp;
		}

		const T &operator*()
		{
			return _node->_data;
		}

		const T *operator->()
		{
			return &_node->_data;
		}

		bool operator!=(const self &s)
		{
			return _node != s._node;
		}

		bool operator==(const self &s)
		{
			return _node == s._node;
		}
	};

	template <class T>
	class list	 // 双向链表类
	{
		typedef list_node<T> Node;
	public:
		typedef __list_iterator<T, T&, T*> iterator;     // 普通迭代器
		typedef __list_iterator<T, const T&, const T*> const_iterator;  // const迭代器
		// typedef __list_const_iterator<T> const_iterator;

		iterator begin()
		{
			return _head->_next;
		}

		iterator end()
		{
			return _head;
		}

		const_iterator begin() const
		{
			return _head->_next;
		}

		const_iterator end() const
		{
			return _head;
		}

		void empty_init() // 置空初始化
		{
			_head = new Node;
			_head->_next = _head;
			_head->_prev = _head;
			_size = 0;    // 大小初始化为0
		}

		list()  // 默认构造函数
		{
			empty_init();
		}

		list(const list<T> &It)  // 拷贝构造函数
		{
			empty_init();
			for (auto e : It)
			{
				push_back(e);
			}
		}

		void swap(list<T> &It)
		{
			std::swap(_head, It._head);
			std::swap(_size, It._size);
		}

		// It3(this = It3) = It1 (It1 = It)
		list<int> &operator=(list<int> &It) // 赋值运算符重载
		{
			swap(It);
			return *this;
		}

		~list()
		{
			clear();
			delete _head;
			_head = nullptr;
		}

		void clear()    // 清空链表（除头节点外）
		{
			iterator it = begin();
			while (it != end())
			{
				it = erase(it);
			}
		}

		void push_back(const T &x)  // 在链表末尾添加元素
		{
			insert(end(), x);
		}

		void push_front(const T &x)   // 在链表头部添加元素
		{
			insert(begin(), x);
		}

		void pop_front()
		{
			erase(begin());
		}

		void pop_back()
		{
			erase(--end());
		}

		iterator insert(iterator pos, const T &x)  // 在pos位置插入新节点x
		{
			Node *node = pos._node;
			Node *newnode = new Node(x);
			Node *prev = node->_prev;
			prev->_next = newnode;
			newnode->_prev = prev;
			newnode->_next = node;
			node->_prev = newnode;
			++_size;
			return iterator(newnode);
		}

		iterator erase(iterator pos) // 删除节点
		{
			Node *node = pos._node;
			Node *prev = node->_prev;
			Node *next = node->_next;
			delete node;
			prev->_next = next;
			next->_prev = prev;
			--_size;
			return iterator(next);
		}

		size_t size()
		{
			return _size;
		}

	private:
		Node *_head;
		size_t _size;
	};

	void test_list1()
	{
		list<int> It;
		It.push_back(1);
		It.push_back(2);
		It.push_back(3);
		It.push_back(4);
		It.push_back(5);
		list<int>::iterator it = It.begin();
		while (it != It.end())
		{
			cout << *it << endl;
			++it;
		}
		cout << endl;
		for (auto e : It)
		{
			cout << e << " ";
		}
		cout << endl;
	}

	void test_list2()
	{
		list<int> It1;
		It1.push_back(1);
		It1.push_back(2);
		It1.push_back(3);
		It1.push_back(4);
		It1.push_back(5);

		cout << "It1: ";
		for (auto e : It1)
		{
			cout << e << " ";
		}
		cout << endl;

		list<int> It2;
		cout << "拷贝前It2: ";
		for (auto e : It2)
		{
			cout << e << " ";
		}
		cout << endl;
		It2 = It1; // 拷贝构造
		cout << "拷贝后It2: ";
		for (auto e : It2)
		{
			cout << e << " ";
		}
		cout << endl;
	}

	struct AA
	{
		AA(int a1 = 0, int a2 = 0)
			: _a1(a1),_a2(a2)
		{}
		int _a1;
		int _a2;
	};
	void test_list3()
	{
		list<AA> It;
		It.push_back(AA(1, 1));
		It.push_back(AA(2, 2));
		It.push_back(AA(3, 3));
		It.push_back(AA(4, 4));
		list<AA>::iterator it = It.begin();
		while (it != It.end())
		{
			// cout << (*it)._a1 << " " << (*it)._a2 << endl;
			cout << it->_a1 << " " << it->_a2 << endl; // 迭代器不支持箭头，需要重载
			++it;
		}
	}

	////////////////////  const  ////////////////////

	// 只能输出int
	void print_list(const list<int> &It)
	{
		list<int>::const_iterator it = It.begin();
		while (it != It.end())
		{
			cout << *it << " ";
			++it;
		}
		cout << endl;

		// for(auto e : It)
		// {
		// 	cout << e << " ";
		// }
		// cout << endl;
	}

	// 可以输出list的任意类型
	template <typename T> // T没有实例化  //先实例化
	// template <class T> // 直接编译不通过
	void print_list(const list<T> &It)
	{
		// list<T>::const_iterator it = It.begin();  //直接编译不通过

		/* list<T>未实例化的类模板，比那一其不能去它里面去找
		   编译器就无法list<T>::const_iterator是内嵌类型，还是静态成员变量
		   前面加一个typename就是告诉编译器，这里是一个类型，等list<T>实例化再去类里面去取*/
		typename list<T>::const_iterator it = It.begin(); // 不加typename，list<T>没有实例化
		while (it != It.end())
		{
			cout << *it << " ";
			++it;
		}
		cout << endl;
	}

	// 可以输出任意容器的类型
	template <typename Container>
	void print_Container(const Container &con)
	{
		typename Container::const_iterator it = con.begin();
		while (it != con.end())
		{
			cout << *it << " ";
			++it;
		}
		cout << endl;
	}

	void test_list4()
	{
		list<int> It;
		It.push_back(1);
		It.push_back(2);
		It.push_back(3);
		It.push_back(4);
		It.push_back(5);
		//print_list(It);
		print_Container(It);

		list<string> It1;
		It1.push_back("s");
		It1.push_back("o");
		It1.push_back("l");
		It1.push_back("i");
		It1.push_back("t");
		It1.push_back("y");
		//print_list(It1);
		print_Container(It1);

		vector<string> It2;
		It2.push_back("p");
		It2.push_back("z");
		It2.push_back("h");
		print_Container(It2);
	}
}