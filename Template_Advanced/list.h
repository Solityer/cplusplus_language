#include"reverse_iterator.h"
#include <vector>
#include <set>

namespace pzh
{
	template<class T>
	struct list_node
	{
		T _data;
		list_node<T>* _next;
		list_node<T>* _prev;

		list_node(const T& x = T())
			:_data(x)
			,_next(nullptr)
			,_prev(nullptr)
		{}
	};

	// 链表迭代器模板结构体，使用三个模板参数分别处理T、T&、T*的情况
	// 通过模板参数实现const和非const迭代器的统一实现
	template<class T, class Ref, class Ptr>
	struct __list_iterator
	{
		typedef list_node<T> Node;
		typedef __list_iterator<T, Ref, Ptr> self;
		Node* _node;  // 指向当前节点的指针

		__list_iterator(Node* node)  // 构造函数，用节点指针初始化迭代器
			:_node(node)
		{}

		self& operator++()   // 前置++运算符重载，移动到下一个节点
		{
			_node = _node->_next;
			return *this;
		}

		self& operator--()
		{
			_node = _node->_prev;
			return *this;
		}

		self operator++(int)
		{
			self tmp(*this);
			_node = _node->_next;
			return tmp;   // 返回自增前的迭代器副本
		}

		self operator--(int)
		{
			self tmp(*this);
			_node = _node->_prev;
			return tmp;
		}

		Ref operator*()  // 解引用运算符重载，返回节点数据的引用
		{
			return _node->_data;
		}

		// 箭头运算符重载，返回节点数据的指针
		// 用于通过迭代器直接访问成员，如it->member
		Ptr operator->()
		{
			return &_node->_data;
		}

		// 不等于运算符重载，比较两个迭代器是否指向不同节点
		bool operator!=(const self& s)
		{
			return _node != s._node;
		}

		bool operator==(const self& s)
		{
			return _node == s._node;
		}
	};

	// 双向链表模板类
	template<class T>
	class list
	{
		typedef list_node<T> Node;
	public:
		typedef __list_iterator<T, T&, T*> iterator;  // 迭代器类型别名
		typedef __list_iterator<T, const T&, const T*> const_iterator;   // 常量迭代器类型别名
		typedef ReverseIterator<iterator, T&, T*> reverse_iterator;  // 反向迭代器类型别名，使用ReverseIterator适配器
		typedef ReverseIterator<const_iterator, const T&, const T*> const_reverse_iterator;   // 常量反向迭代器类型别名

		// 返回反向迭代器起始位置（对应正向迭代器的结束位置）
		reverse_iterator rbegin()
		{
			return reverse_iterator(end());  // 用end()构造反向迭代器起始位置
		}

		// 返回反向迭代器结束位置（对应正向迭代器的起始位置）
		reverse_iterator rend()
		{
			return reverse_iterator(begin());  // 用begin()构造反向迭代器结束
		}

		const_reverse_iterator rbegin() const
		{
			return const_reverse_iterator(end());
		}

		const_reverse_iterator rend() const
		{
			return const_reverse_iterator(begin());
		}

		// 返回常量迭代器起始位置
		const_iterator begin() const
		{
			return const_iterator(_head->_next);
		}

		const_iterator end() const
		{
			return const_iterator(_head);
		}

		iterator begin()
		{
			return iterator(_head->_next);
			//return _head->_next;
		}

		iterator end()
		{
			return iterator(_head);
			//return _head;
		}

		void empty_init()
		{
			_head = new Node;
			_head->_next = _head;
			_head->_prev = _head;
			_size = 0;
		}
		// 默认构造函数
		list()
		{
			empty_init();  // 调用空链表初始化
		}

		// 拷贝构造函数，用另一个list初始化当前list
		list(const list<T>& lt)
		{
			empty_init();
			for (auto e : lt)
			{
				push_back(e);
			}
		}

		// lt3 = lt1
		/*list<int>& operator=(const list<int>& lt)
		{
			if (this != &lt)
			{
				clear();
				for (auto e : lt)
				{
					push_back(e);
				}
			}

			return *this;
		}*/

		void swap(list<T>& lt)
		{
			std::swap(_head, lt._head);
			std::swap(_size, lt._size);
		}

		// 赋值运算符重载（使用拷贝交换）
		list<T>& operator=(list<T> lt)
		{
			swap(lt);
			return *this;
		}

		~list()
		{
			clear();

			delete _head;
			_head = nullptr;
		}

		// 清空链表中的所有元素节点（保留头节点）
		void clear()
		{
			iterator it = begin();
			while (it != end())
			{
				it = erase(it);
			}
		}

		// 在链表末尾插入元素
		void push_back(const T& x)
		{
			insert(end(), x);
		}

		// 在链表头部插入元素
		void push_front(const T& x)
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

		// 在指定位置前插入元素
		iterator insert(iterator pos, const T& x)
		{
			Node* cur = pos._node;
			Node* newnode = new Node(x);
			Node* prev = cur->_prev;
			// 将新节点插入到prev和cur之间
			// 连接prev和newnode
			prev->_next = newnode;
			newnode->_prev = prev;
			// 连接newnode和cur
			newnode->_next = cur;
			cur->_prev = newnode;
			++_size;
			return iterator(newnode);
		}

		// 删除指定位置的元素
		iterator erase(iterator pos)
		{
			Node* cur = pos._node;
			Node* prev = cur->_prev;
			Node* next = cur->_next;
			delete cur;
			prev->_next = next;
			next->_prev = prev;
			--_size;
			return iterator(next);
		}

		// 获取链表大小
		size_t size()
		{
			return _size;
		}

	private:
		Node* _head;
		size_t _size;
	};

	void test_list1()
	{
		list<int> lt;
		lt.push_back(1);
		lt.push_back(2);
		lt.push_back(3);
		lt.push_back(4);
		lt.push_back(5);

		list<int>::iterator it = lt.begin();
		while (it != lt.end())
		{
			*it += 20;
			cout << *it << " ";
			++it;
		}
		cout << endl;
		for (auto e : lt)
		{
			cout << e << " ";
		}
		cout << endl;

		set<int> s;
		s.insert(1);
		s.insert(3);
		s.insert(2);
		s.insert(4);

		set<int>::iterator sit = s.begin();
		while (sit != s.end())
		{
			cout << *sit << " ";
			++sit;
		}
		cout << endl;
	}

	void test_list2()
	{
		list<int> lt;
		lt.push_back(1);
		lt.push_back(2);
		lt.push_back(3);
		lt.push_back(4);
		lt.push_back(5);

		list<int> lt1(lt);

		for (auto e : lt)
		{
			cout << e << " ";
		}
		cout << endl;

		for (auto e : lt1)
		{
			cout << e << " ";
		}
		cout << endl;

		list<int> lt2;
		lt2.push_back(10);
		lt2.push_back(200);
		lt2.push_back(30);
		lt2.push_back(40);
		lt2.push_back(50);

		lt1 = lt2;
		for (auto e : lt1)
		{
			cout << e << " ";
		}
		cout << endl;

		for (auto e : lt2)
		{
			cout << e << " ";
		}
		cout << endl;
	}

	struct AA
	{
		AA(int a1 = 0, int a2 = 0)
			:_a1(a1)
			,_a2(a2)
		{}

		int _a1;
		int _a2;
	};

	void test_list3()
	{
		list<AA> lt;
		lt.push_back(AA(1, 1));
		lt.push_back(AA(2, 2));
		lt.push_back(AA(3, 3));

		list<AA>::iterator it = lt.begin();
		while (it != lt.end())
		{
			//cout << (*it)._a1<<" "<<(*it)._a2 << endl;
			cout << it->_a1 << " " << it->_a2 << endl;
			cout << it.operator->()->_a1 << " " << it.operator->()->_a1 << endl;
			++it;
		}
		cout << endl;
	}

	// 专用函数：只能用于 pzh::list 容器
	template<typename T>
	void print_list(const list<T>& lt)
	{
		typename list<T>::const_iterator it = lt.begin();
		while (it != lt.end())
		{
			cout << *it << " ";
			++it;
		}
		cout << endl;
	}

	// 通用函数：可用于任何支持标准迭代器接口的容器
	template<typename Container>
	void print_container(const Container& con)
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
		list<int> lt;
		lt.push_back(1);
		lt.push_back(2);
		lt.push_back(3);
		lt.push_back(4);
		lt.push_back(5);
		// print_list(lt);
		print_container(lt);

		list<string> lt1;
		lt1.push_back("1111111111111");
		lt1.push_back("1111111111111");
		lt1.push_back("1111111111111");
		lt1.push_back("1111111111111");
		lt1.push_back("1111111111111");
		// print_list(lt1);
		print_container(lt1);

		vector<string> v;
		v.push_back("222222222222222222222");
		v.push_back("222222222222222222222");
		v.push_back("222222222222222222222");
		v.push_back("222222222222222222222");
		// print_list(v);
		print_container(v);
	}
}