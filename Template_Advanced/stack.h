#include<iostream>
using namespace std;

template <class T>
T Add(const T& left, const T& right)
{
    cout << "T Add(const T& left, const T& right)" << endl;
    return left + right;
}

void func()
{
    cout << "void func()" << endl;
}

// 显示实例化部分
// 为Add函数模板生成int类型的实例化版本
// 语法：template 返回类型 函数名<具体类型>(参数列表)
// 这会在编译时生成int版本的Add函数
template
int Add<int>(const int& left, const int& right);

// 为Add函数模板生成double类型的实例化版本
template
double Add<double>(const double& left, const double& right);

// 类模板定义：通用的栈类
// template <class T> 表示这是一个类模板，T是类型参数
template <class T>
class Stack
{
public:
    // 成员函数：向栈中压入元素
    // 参数使用const T&，避免拷贝，同时保护实参
    void Push(const T& x)
    {
        cout << "void Stack<T>::Push(const T& x)" << endl;
    }

    void Pop()
    {
        cout << "void Pop()" << endl;
    }

private:
    T* _a = nullptr;
    int _top = 0;
    int _capacity = 0;
};
