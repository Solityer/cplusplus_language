#include <iostream>
using namespace std;

//c++兼容c语言，结构用法可以继续用，同时struct也升级成了类
struct Stack_example  //默认限定访问符为public
{
    //成员函数
    void Init(int n = 4)
	{
        a = (int*)malloc(sizeof(int) * n);
		if (nullptr == a)
		{
			perror("malloc failed to apply for space.");
			return;
		}
		capacity = n;
		size = 0;
	}

	void Push(int x)
	{
		a[size++] = x;
	}
private:
    //成员变量
	int* a;
	int size;
	int capacity;
};

//上述结构体，c++中更喜欢用class替代
class Date_example  //默认限定访问符为private
{
public:
	void Init(int year, int month, int day)  //前面也回补充Date* this
	{
		if (this == nullptr) {
			cout << "错误：空指针调用Init" << endl;
			return;
		}
		// this->_year = year;  //实际上会自动补充this指针
		_year = year;
		_month = month;
		_day = day;
	}

	void func()
	{
		cout << this << endl;
		cout << "func()" << endl;
	}

	void Print()
	{
		cout << _year << "-" << _month << "-" << _day << endl;
	}

//private:
	int _year = 1;
	int _month = 1;
	int _day = 1;
};

typedef int DataType;
class Stack
{
public:
	// Stack()  //构造函数，函数名与类名相同，无返回值不需要写void
	// {
	//     _array = nullptr;
	//     _size = _capacity = 0;
	// }

	Stack(size_t capacity = 4)  //有一个默认参数，所以它可以被无参调用
	{
		_array = (DataType*)malloc(sizeof(DataType) * capacity);
		if(nullptr == _array)
		{
			perror("malloc申请空间失败");
			return;
		}
		_capacity = capacity;
		_size = 0;
	}

	Stack(int n)
	{
	    _array = (int *)malloc(sizeof(int) * n);
	    if(nullptr == _array)
	    {
	        perror("malloc");
	        return;
	    }
	    _capacity = n;
	    _size = 0;
	}

	Stack(Stack& s)
	{
		cout << "Stack(Stack& s)" << endl;
		_array = (DataType*)malloc(sizeof(DataType) * s._capacity);
		if(nullptr == _array)
		{
			perror("malloc申请空间失败");
			return;
		}
		memcpy(_array,s._array,sizeof(DataType) * s._size);
		_size = s._size;
		_capacity = s._capacity;
	}

	void Init(int n)
	{
		_array = (int *)malloc(sizeof(int) * n);
		if(nullptr == _array)
		{
			perror("malloc");
			return;
		}
		_capacity = n;
		_size = 0;
	}

	~Stack()
	{
		cout << "~Stack()" << endl;
		free(_array);
		_array = nullptr;
		_size = _capacity = 0;
	}

	void Push(int x)
	{
		_array[_size++] = x;
	}

private:
	int* _array;
	int _size;
	int _capacity;
};

class Date
{
public:
	//    //定义了全参的构造函数就不能再定义无参构造函数，除非Date的形参中只声明未定义具体数值
	//    Date()
	//    {
	//        _year = 1;
	//        _month = 1;
	//        _day = 1;
	//    }
	//    Date(int year, int month, int day)
	//    {
	//        _year = year;
	//        _month = month;
	//        _day = day;
	//    }

	//上面两个合并的结果
	Date(int year = 2000, int month = 1, int day = 1)
	{
		_year = year;
		_month = month;
		_day = day;
	}

	//拷贝构造函数
	Date(Date& d)
	{
		_year = d._year;
		_month = d._month;
		_day = d._day;
	}

	void Print()
	{
		cout << _year << '/' << _month << '/' << _day << endl;
	}

private:
	int _year;
	int _month;
	int _day;
};

void fun1(Date d)
{
	d.Print();
}
void fun2(Stack s)
{
	s.Push(1);
	s.Push(2);
}

int main()
{
	Date_example d;
	d.Init(2024, 2, 3);
	d._year++;
	cout << d._year << endl;
	cout << sizeof(d) << endl;

	// Date_example* ptr = nullptr;  //空指针调用Init,结果报错
	Date_example* ptr = new Date_example;  // 动态分配
	ptr->func();
	ptr -> Init(2000,1,1);
	(*ptr).func();

    Stack_example st;
    st.Init();
    st.Push(1);
    st.Push(2);
    st.Push(3);

	////////////////////////////////////
	Date d3;
	d3.Print();
	Date d4(2023,3,9);
	d4.Print();
	Date d5(d4);  //将d2深拷贝给d3,等价于 Date d3 = d2
	d5.Print();

	fun1(d4);
	Stack s1;
	fun2(s1);
	Stack s2(s1);

	return 0;
}