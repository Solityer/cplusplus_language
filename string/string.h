#include <iostream>
using namespace std;
#include <assert.h>
#include <cstring>

namespace pzh
{
	class string
	{
	public:
		typedef char* iterator;
		typedef const char* const_iterator;

		iterator begin()
		{
			return _str;
		}
		iterator end()
		{
			return _str + _size;
		}

		const_iterator begin() const
		{
			return _str;
		}
		const_iterator end() const
		{
			return _str + _size;
		}

		/*string()  //无参构造
			: _str(new char[1])
			, _size(0)
			, _capacity(0)
		{
			_str[0] = '\0';
		}*/

		string(const char* str = "")  //构造函数
			: _size(strlen(str))
		{
			_capacity = _size;
			_str = new char[_capacity + 1];  //+1是要给‘\0’留一个空间
			strcpy(_str, str);
		}

		string(const string& s)  //拷贝构造
			: _size(s._size)
			, _capacity(s._capacity)
		{
			_str = new char[s._capacity + 1];
			strcpy(_str, s._str);
		}

		~string()
		{
			delete[] _str;
			_str = nullptr;
			_size = _capacity = 0;
		}

		const char* c_str()
		{
			return _str;
		}

		size_t size() const
		{
			return _size;
		}

		char& operator[](size_t pos)
		{
			assert(pos < _size);
			return _str[pos];
		}

		const char& operator[](size_t pos) const
		{
			assert(pos < _size);
			return _str[pos];
		}

		string& operator=(const string& s)
		{
			if (this != &s)
			{
				char* tmp = new char[s._capacity + 1];
				strcpy(tmp, s._str);
				delete[] _str;
				_str = tmp;
				_size = s._size;
				_capacity = s._capacity;
			}
			return *this;
		}

		bool operator>(const string& s) const
		{
			return strcmp(_str, s._str) > 0;
		}

		bool operator==(const string& s) const
		{
			return strcmp(_str, s._str) == 0;
		}

		bool operator>=(const string& s) const
		{
			return *this > s || *this == s;
		}

		bool operator<(const string& s) const
		{
			return !(*this >= s);
		}

		bool operator<=(const string& s) const
		{
			return !(*this > s);
		}

		bool operator!=(const string& s) const
		{
			return !(*this == s);
		}

		string& operator+=(const char* str)
		{
			append(str);
			return *this;
		}

		void push_back(char ch)
		{
			if (_size + 1 > _capacity)
			{
				reserve(_capacity * 2);
			}
			_str[_size] = ch;
			++_size;
			_str[_size] = '\0';
		}

		void append(const char* str)
		{
			size_t len = strlen(str);
			if (_size + len > _capacity)
			{
				reserve(len + _size);
			}
			strcpy(_str + _size, str);
			_size += len;
		}

		void reserve(size_t n)
		{
			char* tmp = new char[n + 1];
			strcpy(tmp, _str);
			delete[] _str;
			_str = tmp;
			_capacity = n;
		}

		void insert(size_t pos, char ch)
		{
			assert(pos <= _size);
			if (_size + 1 > _capacity)
			{
				reserve(2 * _capacity);
			}
			size_t end = _size;
			while (end >= pos)
			{
				_str[end + 1] = _str[end];
				--end;
			}
			_str[pos] = ch;
			++_size;
		}

	private:
		char* _str;
		size_t _size;
		size_t _capacity;
	};

	void print(const string& s)
	{
		for (size_t i = 0; i < s.size(); ++i)
		{
			cout << s[i] << " ";
		}
		cout << endl;

		string::const_iterator it = s.begin();
		while (it != s.end())
		{
			++it;
		}
		cout << endl;

		for (auto ch : s)
		{
			cout << ch << " ";
		}
	}

	void func_str1()
	{
		string s1;
		string s2("hello solity");
		string s3(s2);  //this
		cout << s1.c_str() << endl;
		cout << s2.c_str() << endl;
		cout << s3.c_str() << endl;

		s1 = s3;
		cout << s1.c_str() << endl;
	}

	void func_str2()
	{
		string s1("hello solity");
		for (size_t i = 0; i < s1.size(); ++i)
		{
			s1[i]++;
		}
		for (size_t i = 0; i < s1.size(); ++i)
		{
			cout << s1[i] << " ";
		}
		cout << endl;

		print(s1);
		cout << endl;

		string::iterator it = s1.begin();
		while (it != s1.end())
		{
			(*it)--;
			++it;
		}
		cout << endl;
		it = s1.begin();
		while (it != s1.end())
		{
			cout << *it << " ";
			++it;
		}
		cout << endl;

		for (auto ch : s1)
		{
			cout << ch << " ";
		}
	}

	void func_str3()
	{
		string s1("hello solity");
		string s2("hello pzh");
		string s3("abc");
		cout << (s1 < s2) << endl;
		cout << (s1 == s2) << endl;
		cout << (s1 >= s2) << endl;

		for (auto e : s1)
		{
			cout << e;
		}
	}
}