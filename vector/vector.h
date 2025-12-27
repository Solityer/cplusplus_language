#include <iostream>
using namespace std;
#include <vector>  //就是一个顺序表
#include <algorithm> // 用于 std::find
#include <assert.h>

namespace pzh
{
	/*我们可以将template<class T>理解为：定义一个模板，其中T是一个类型参数
	 *在后续代码中T可以被替换为任何具体的类型，从而生成该类型的特定代码。*/
	template<class T>
	class vector
	{
	public:
		typedef T* iterator;
		typedef const T* const_iterator;

		vector()
			: _start(nullptr)
			, _finish(nullptr)
			, _end_of_storage(nullptr)  // 初始化容量结束指针为空
		{}

		/* 匿名函数的意义
		*  vector(size_t n, const T& val = T());};
		   vector<int> v1(5);  // 等效于 vector<int> v1(5, int());
						       // 创建5个int，每个初始化为0
		   vector<int> v2(5, 42);   // 创建5个int，每个初始化为42
		   vector<string> v3(3);   // 等效于 vector<string> v3(3, string());
						           // 创建3个空字符串*/
		vector(size_t n, const T& val = T())
			: _start(nullptr)
			, _finish(nullptr)
			, _end_of_storage(nullptr)
		{
				reserve(n);  // 预留n个元素的空间
				for (size_t i = 0; i < n; i++)
				{
					push_back(val);
				}
		}

		template <class InputIterator>
		vector(InputIterator first, InputIterator last)  // first指向首元素，last指向尾后元素

			: _start(nullptr)
			, _finish(nullptr)
			, _end_of_storage(nullptr)
		{
			while (first != last)
			{
				push_back(*first);  // 将迭代器指向的元素添加到vector
				++first;
			}
		}

		~vector()
		{
			delete[] _start;
			_start = _finish = _end_of_storage = nullptr;
		}

		iterator begin()
		{
			return _start;
		}
		iterator end()
		{
			return _finish;  // 返回指向最后一个元素之后位置的指针
		}

		const_iterator begin() const
		{
			return _start;
		}
		const_iterator end() const
		{
			return _finish;
		}

		size_t capacity() const
		{
			return _end_of_storage - _start;  // 容量 = 存储结束位置 - 开始位置
		}

		size_t size() const
		{
			return _finish - _start;  // 元素数量 = 结束位置 - 开始位置
		}

		/*T val = T()：表示当需要增加元素时，新添加的元素的初始值。
		 *这里使用默认参数，即如果调用者没有提供第二个参数，则使用T类型的默认构造函数生成的值
		 *（例如，对于int，就是0；对于类类型，就是调用默认构造函数）*/
		void resize(size_t n,T val = T())  // 调整大小：n为新大小，val为新增元素值
		{
			if(n < size())
			{
				//删除数据
				_finish = _start + n;  // 直接将结束指针前移，丢弃多余元素
			}
			else
			{
				if (n > capacity())
				{
					reserve(n);
				}
				while (_finish != _start + n)  // 循环直到元素数量达到n
				{
					*_finish = val;
					++_finish;
				}
			}
		}

		void reserve(size_t n)  // 预留容量：n为期望的最小容量
		{
			if (n > capacity())
			{
				size_t sz = size();
				T* tmp = new T[n];  // 分配新的内存空间
				if (_start)  // 如果原空间不为空
				{
					memcpy(tmp, _start, sizeof(T) * size());  // 复制原数据到新空间
					delete _start;
				}
				_start = tmp;
				_finish = tmp + sz;
				_end_of_storage = _start + n;  // 更新容量结束指针
			}
		}

		void push_back(const T& x)  //插入
		{
			if (_finish == _end_of_storage)  //检查容量
			{
				reserve(capacity() == 0 ? 4 : capacity() * 2);;
			}
			*_finish = x;
			++_finish;
		}

		bool empty()
		{
			return _start == _finish;
		}

		void pop_back()  //删除
		{
			assert(!empty());
			--_finish;
		}

		void insert(iterator pos, const T& val)  //在pos位置插入val值
		{
			assert(pos >= _start);
			assert(pos <= _finish);
			if (_finish == _end_of_storage)  //检查容量
			{
				size_t newCapacity = (0 == capacity()) ? 1 : capacity() * 2;  // 计算新容量
				reserve(newCapacity);
				pos = _start + size(); // 如果发生了增容，需要重置pos
			}
			iterator end = _finish - 1;
			while (end >= pos)
			{
				*(end + 1) = *end;
				--end;
			}
			*pos = val;
			++_finish;
		}

		iterator erase(iterator pos)
		{
			//挪动数据进行删除
			iterator begin = pos + 1;
			while (begin != _finish)
			{
				*(begin - 1) = *begin;
				++begin;
			}
			--_finish;
			return pos;
		}

		const T& operator[](size_t pos) const
		{
			assert(pos < size());
			return _start[pos];
		}

	private:
		iterator _start;  //空间开始位置
		iterator _finish;  //空间内字符的最后位置
		iterator _end_of_storage;  //空间的最后位置
	};

	void print(const vector<int>& v)
	{
		for (size_t i = 0; i < v.size(); i++)
		{
			cout << v[i] << " ";
		}
		cout << endl;
	}

	void test_vector1()
	{
		vector<int> v1;
		v1.push_back(1);
		v1.push_back(2);
		v1.push_back(3);
		v1.push_back(4);
		v1.push_back(5);
		for (size_t i = 0; i < v1.size(); ++i)
		{
			cout << v1[i] << " ";
		}
		cout << endl;

		vector<int>::iterator it = v1.begin();
		while (it != v1.end())
		{
			cout << *it << " ";
			++it;
		}
		cout << endl;

		for (auto e : v1)
		{
			cout << e << " ";
		}
		cout << endl;

		print(v1);
	}

	void test_vector2()
	{
		vector<int> v1;
		v1.push_back(1);
		v1.push_back(2);
		v1.push_back(3);
		v1.push_back(4);
		v1.push_back(5);
		cout << v1.size() << endl;
		cout << v1.capacity() << endl;

		v1.resize(10);
		cout << v1.size() << endl;
		cout << v1.capacity() << endl;

		for (auto e : v1)
		{
			cout << e << " ";
		}
		cout << endl;
	}

	void test_vector3()
	{
		vector<int> v1;
		v1.push_back(1);
		v1.push_back(2);
		v1.push_back(3);
		v1.push_back(4);
		v1.push_back(5);
		for (auto e : v1)
		{
			cout << e << " ";
		}
		cout << endl;

		v1.insert(v1.begin(), 0);
		for (auto e : v1)
		{
			cout << e << " ";
		}
		cout << endl;

		auto pos = find(v1.begin(), v1.end(), 3);
		if (pos != v1.end())
		{
			v1.insert(pos, 30);
		}
		for (auto e : v1)
		{
			cout << e << " ";
		}
		cout << endl;

		v1.erase(v1.begin() + 1);
		for (auto e : v1)
		{
			cout << e << " ";
		}
		cout << endl;
	}

	void test_vector4()
	{
		std::vector<int> v1;
		v1.push_back(1);
		v1.push_back(2);
		v1.push_back(3);
		v1.push_back(4);
		v1.push_back(5);
		for (auto e : v1)
		{
			cout << e << " ";
		}
		cout << endl;

		std::vector<int>::iterator it = v1.begin();
		while (it != v1.end())
		{
			if (*it % 2 == 0)
				it = v1.erase(it);  //erase返回删除数据的下一个位置
			else
				++it;
		}
		for (auto e : v1)
		{
			cout << e << " ";
		}
		cout << endl;
	}
}