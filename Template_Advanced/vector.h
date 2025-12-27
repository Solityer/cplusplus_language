#include <assert.h>

namespace pzh
{
	template<class T>
	class vector
	{
	public:
		// 迭代器类型定义，vector的迭代器本质是指针
		typedef T* iterator;
		typedef const T* const_iterator;
		// 反向迭代器类型定义，使用ReverseIterator适配器
		typedef ReverseIterator<iterator, T&, T*> reverse_iterator;
		typedef ReverseIterator<const_iterator, const T&, const T*> const_reverse_iterator;

		reverse_iterator rbegin()
		{
			return reverse_iterator(end());
		}

		reverse_iterator rend()
		{
			return reverse_iterator(begin());
		}

		const_reverse_iterator rbegin() const
		{
			return const_reverse_iterator(end());
		}

		const_reverse_iterator rend() const
		{
			return const_reverse_iterator(begin());
		}

		iterator begin()
		{
			return _start;
		}

		iterator end()
		{
			return _finish;
		}

		const_iterator begin() const
		{
			return _start;
		}

		const_iterator end() const
		{
			return _finish;
		}

		vector()  // 默认构造函数，不分配任何内存
		{}

		// 范围构造函数，用迭代器范围[first, last)初始化vector
		template <class InputIterator>
		vector(InputIterator first, InputIterator last)
		{
			// 遍历输入迭代器范围
			while (first != last)
			{
				push_back(*first);
				++first;
			}
		}

		// 构造函数，创建包含n个val的vector（size_t版本）
		vector(size_t n, const T& val = T())
		{
			reserve(n);
			for (size_t i = 0; i < n; i++)
			{
				push_back(val);
			}
		}

		// 构造函数，创建包含n个val的vector（int版本）
		// 避免与范围构造函数产生歧义
		vector(int n, const T& val = T())
		{
			reserve(n);
			for (int i = 0; i < n; i++)
			{
				push_back(val);
			}
		}

		// 拷贝构造函数，深拷贝另一个vector
		// v2(v1)
		vector(const vector<T>& v)
		{
			reserve(v.capacity());
			for (auto& e : v)
			{
				push_back(e);
			}
		}

		void swap(vector<T>& v)
		{
			std::swap(_start, v._start);
			std::swap(_finish, v._finish);
			std::swap(_endofstorage, v._endofstorage);
		}

		// v1 = v3
		vector<T>& operator=(vector<T> tmp)
		{
			swap(tmp);
			return *this;
		}

		~vector()
		{
			delete[] _start;   // 释放动态分配的数组内存
			// 所有指针置为空，防止悬空指针
			_start = _finish = _endofstorage = nullptr;
		}

		// 预留至少能容纳n个元素的内存空间
		void reserve(size_t n)
		{
			if (n > capacity())
			{
				T* tmp = new T[n];
				size_t sz = size();
				if (_start)
				{
					// 不能使用memcpy，因为对于非平凡类型（如string）需要调用拷贝构造函数
					//memcpy(tmp, _start, sizeof(T) * sz);
					// 使用循环深拷贝每个元素
					//memcpy(tmp, _start, sizeof(T) * sz);
					for (size_t i = 0; i < sz; i++)
					{
						tmp[i] = _start[i];   // 调用T的赋值运算符（深拷贝）
					}
					delete[] _start;
				}
				_start = tmp;
				_finish = _start + sz;
				_endofstorage = _start + n;
			}
		}

		// 调整vector的大小为n
		// 如果n小于当前大小，截断尾部元素
		// 如果n大于当前大小，用val填充新增位置
		// T int
		// T string
		// T vector<int>
		//void resize(size_t n, T val = T())
		void resize(size_t n, const T& val = T())  // 参数改为const引用，避免不必要的拷贝
		{
			if (n <= size())
			{
				_finish = _start + n;
			}
			else
			{
				reserve(n);
				while (_finish < _start+n)
				{
					*_finish = val;
					++_finish;
				}
			}
		}

		void push_back(const T& x)
		{
			insert(end(), x);  // 复用insert函数在末尾插入
			/*
			if (_finish == _endofstorage)
			{
				reserve(capacity() == 0 ? 4 : capacity() * 2);
			}
			
			*_finish = x;
			++_finish;*/
		}

		void insert(iterator pos, const T& x)
		{
			assert(pos >= _start);
			assert(pos <= _finish);

			// 如果容量已满，需要扩容
			if (_finish == _endofstorage)
			{
				// 保存pos相对于_start的偏移量（扩容后pos会失效）
				size_t len = pos - _start;
				// 扩容策略：如果容量为0，扩容到4；否则扩容到2倍
				reserve(capacity() == 0 ? 4 : capacity() * 2);
				pos = _start + len;   // 重新计算pos的位置
			}
			// 从后向前移动元素，为插入腾出空间
			iterator end = _finish - 1;
			while (end >= pos)
			{
				*(end + 1) = *end;
				--end;
			}
			*pos = x;
			++_finish;
		}

		iterator erase(iterator pos)
		{
			assert(pos >= _start);
			assert(pos < _finish);

			// 从pos+1开始向前移动元素，覆盖被删除的元素
			iterator it = pos + 1;
			while (it < _finish)
			{
				*(it - 1) = *it;
				++it;
			}
			--_finish;
			return pos;
		}


		T& operator[](size_t pos)
		{
			assert(pos < size());
			return _start[pos];
		}

		const T& operator[](size_t pos) const
		{
			assert(pos < size());
			return _start[pos];
		}

		// 获取vector的容量（当前分配的内存最多能容纳的元素个数）
		size_t capacity() const
		{
			return _endofstorage - _start;
		}

		// 获取vector当前的大小（实际元素个数）
		size_t size() const
		{
			return _finish - _start;
		}

	private:
		iterator _start = nullptr;
		iterator _finish = nullptr;   // 指向最后一个元素之后的位置（size位置）
		iterator _endofstorage = nullptr;  // 指向分配内存的结束位置（capacity位置）
	};

	void test_vector1()
	{
		vector<int> v;
		v.push_back(1);
		v.push_back(2);
		v.push_back(3);
		v.push_back(4);
		v.push_back(5);

		for (size_t i = 0; i < v.size(); i++)
		{
			cout << v[i] << " ";
		}
		cout << endl;

		vector<int>::iterator it = v.begin();
		while (it != v.end())
		{
			*it *= 10;
			cout << *it << " ";
			++it;
		}
		cout << endl;

		for (auto e : v)
		{
			cout << e << " ";
		}
		cout << endl;
	}

	void test_vector2()
	{
		int i = 0;
		int j(1);
		int k = int(2);

		vector<int*> v1;
		v1.resize(10);

		vector<string> v2;
		//v2.resize(10, string("xxx"));
		v2.resize(10, "xxx");

		for (auto e : v1)
		{
			cout << e << " ";
		}
		cout << endl;

		for (auto e : v2)
		{
			cout << e << " ";
		}
		cout << endl;
	}

	void test_vector3()
	{
		vector<int> v;
		v.push_back(1);
		v.push_back(2);
		v.push_back(3);
		v.push_back(4);
		v.push_back(5);
		v.push_back(6);
		v.push_back(7);

		for (auto e : v)
		{
			cout << e << " ";
		}
		cout << endl;

		vector<int>::iterator it = v.begin() + 2;
		v.insert(it, 30);
		for (auto e : v)
		{
			cout << e << " ";
		}
		cout << endl;

		//v.insert(v.begin(), 30);
		v.insert(v.begin()+3, 30);
		for (auto e : v)
		{
			cout << e << " ";
		}
		cout << endl;
	}

	void test_vector4()
	{
		vector<int> v;
		v.push_back(1);
		v.push_back(2);
		v.push_back(3);
		v.push_back(4);
		v.push_back(5);
		v.push_back(6);
		v.push_back(7);

		for (auto e : v)
		{
			cout << e << " ";
		}
		cout << endl;

		auto pos = v.begin();
		v.erase(pos);

		for (auto e : v)
		{
			cout << e << " ";
		}
		cout << endl;

		v.erase(v.begin()+3);
		for (auto e : v)
		{
			cout << e << " ";
		}
		cout << endl;
	}

	void test_vector5()
	{
		//std::vector<int> v;
		vector<int> v;
		v.push_back(1);
		v.push_back(2);
		v.push_back(3);
		v.push_back(4);
		v.push_back(4);
		v.push_back(5);
		v.push_back(6);

		for (auto e : v)
		{
			cout << e << " ";
		}
		cout << endl;

		auto it = v.begin();
		while (it != v.end())
		{
			if (*it % 2 == 0)
			{
				it = v.erase(it);
			}
			else
			{
				++it;
			}
		}

		for (auto e : v)
		{
			cout << e << " ";
		}
		cout << endl;
	}

	void test_vector6()
	{
		vector<string> v;
		v.push_back("111111111111111111111");
		v.push_back("111111111111111111111");
		v.push_back("111111111111111111111");
		v.push_back("111111111111111111111");
		v.push_back("111111111111111111111");
		for (auto e : v)
		{
			cout << e << " ";
		}
		cout << endl;
	}

	void test_vector7()
	{
		vector<int> v1;
		v1.push_back(1);
		v1.push_back(1);
		v1.push_back(1);
		v1.push_back(1);
		v1.push_back(1);

		vector<int> v2(v1);
		for (auto e : v1)
		{
			cout << e << " ";
		}
		cout << endl;

		for (auto e : v2)
		{
			cout << e << " ";
		}
		cout << endl;

		vector<int> v3;
		v3.push_back(10);
		v3.push_back(20);
		v3.push_back(30);
		v3.push_back(40);

		v1 = v3;
		for (auto e : v1)
		{
			cout << e << " ";
		}
		cout << endl;
	}

	void test_vector8()
	{
		//vector<int> v0(10, 0);
		vector<string> v1(10, "xxxx");
		for (auto e : v1)
		{
			cout << e << " ";
		}
		cout << endl;

		vector<int> v2;
		v2.push_back(10);
		v2.push_back(20);
		v2.push_back(30);
		v2.push_back(40);

		vector<int> v3(v2.begin(), v2.end());
		string str("hello world");
		vector<int> v4(str.begin(), str.end());
		for (auto e : v3)
		{
			cout << e << " ";
		}
		cout << endl;

		for (auto e : v4)
		{
			cout << e << " ";
		}
		cout << endl;
	}
}