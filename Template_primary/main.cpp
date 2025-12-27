#include <iostream>
using namespace std;

void Swap0(int& left, int& right)
{
	int temp = left;
	left = right;
	right = temp;
}
void Swap1(double& left, double& right)
{
	double temp = left;
	left = right;
	right = temp;
}
//泛型编程---模板
template<class P>  //也可以 template<typename P>
void Swap(P& x, P& y)
{
	P tmp = x;
	x = y;
	y = x;
}

//类模板
template<class T>
class Stack{
public:
    Stack(size_t capacity = 4)
    {
        cout << "Stack()" << endl;
        _array = new T[capacity];
        _capacity = capacity;
        _size = 0;
    }

    void Push(const T& data)
    {
        _array[_size] = data;
        _size++;
    }

    ~Stack()
    {
        cout << "~Stack()" << endl;
        delete[] _array;
        _array = nullptr;
        _size = _capacity = 0;
    }
private:
    T* _array;
    int _capacity;
    int _size;
};


/*模板参数的匹配原则*/
// 专门处理int的加法函数
int Add1(int left, int right)
{
	return left + right;
}
// 通用加法函数
template<class T>
T Add1(T left, T right)
{
	return left + right;
}

//函数模板
template<class P>
P Add(const P& left, const P& right)
{
    return left + right;
}

int main()
{
	int a = 1, b = 2;
	//Swap0(a, b);
	double c = 1.0, d = 2.0;
	//Swap1(c, d);

    //泛型编程---模板
	Swap(a, b);
	Swap(c, d);

    //类模板只能显式实例化
    Stack<int> st1; // int
    st1.Push(1);
    st1.Push(2);
    Stack<double> st2; // double
    st2.Push(1.2);
    st2.Push(1.2);

    //函数模板
	int a1 = 10, a2 = 20;
	double d1 = 10.11, d2 = 20.22;
    //实参传递给形参，自动推演模板类型
	cout << Add(a1, a2) << endl;
	cout << Add(d1, d2) << endl;
	cout << Add(a1, (int)d1) << endl;
	cout << Add((double)a1, d1) << endl;
	cout << endl;
    //显式实例化
	cout << Add<int>(a1, d1) << endl;
	cout << Add<double>(a1, d1) << endl;

    //模板参数的匹配原则
    Add1(1, 2); // 与非模板函数匹配，编译器不需要特化
    Add1<int>(1, 2); // 调用编译器特化的Add版本

	return 0;
}