#include <iostream>
using namespace std;
#include "list.h"
#include "vector.h"
#include "stack.h"

void func(const pzh::list<int>& lt)
{
    pzh::list<int>::const_reverse_iterator rit = lt.rbegin();
    while (rit != lt.rend())
    {
        cout << *rit << " ";
        ++rit;
    }
    cout << endl;
}

void func(const pzh::vector<int>& lt)
{
    pzh::vector<int>::const_reverse_iterator rit = lt.rbegin();
    while (rit != lt.rend())
    {
        cout << *rit << " ";
        ++rit;
    }
    cout << endl;
}

// 模板特化：针对某些类型进行特殊化处理
template <class T1, class T2>
class Data
{
public:
    Data() { cout << "Data<T1, T2>" << endl; }
private:
    T1 _d1;
    T2 _d2;
};

//这个类是上面类的一个特化--全特化
template <>
class Data<int, int>
{
public:
    Data() { cout << "Data<int, int>" << endl; }
private:
    int _d1;
    char _d2;
};

// 偏特化
template <class T1>
class Data<T1, int>
{
public:
    Data() { cout << "Data<T1, int>" << endl; }
private:
    T1 _d1;
    int _d2;
};

template <typename T1, typename T2>
class Data<T1*, T2*>
{
public:
    Data() { cout << "Data<T1*, T2*>" << endl; }
private:
    T1 _d1;
    T2 _d2;
};

template <typename T1, typename T2>
class Data<T1&, T2&>
{
public:
    Data() { cout << "Data<T1&, T2&>" << endl; }
};

// Date类定义
class Date
{
public:
    Date(int year = 1900, int month = 1, int day = 1)
        : _year(year)
          , _month(month)
          , _day(day)
    {}

    bool operator<(const Date& d) const
    {
        return (_year < d._year) ||
            (_year == d._year && _month < d._month) ||
            (_year == d._year && _month == d._month && _day < d._day);
    }

    bool operator>(const Date& d) const
    {
        return (_year > d._year) ||
            (_year == d._year && _month > d._month) ||
            (_year == d._year && _month == d._month && _day > d._day);
    }

    // 声明友元函数，允许operator<<访问私有成员
    friend ostream& operator<<(ostream& _cout, const Date& d);

private:
    int _year;
    int _month;
    int _day;
};

ostream& operator<<(ostream& _cout, const Date& d)
{
    _cout << d._year << "-" << d._month << "-" << d._day;
    return _cout;
}

// 类模板用特化，函数模板可以不用特化
template<class T>
bool Less(const T& left, const T& right)
{
	return left < right;
}

bool Less(Date* left, Date* right)
{
	return *left < *right;
}

int main()
{
    pzh::list<int> lt1;
    lt1.push_back(1);
    lt1.push_back(2);
    lt1.push_back(3);
    lt1.push_back(4);
    lt1.push_back(5);
    pzh::list<int>::iterator it1 = lt1.begin();
    while (it1 != lt1.end())
    {
    	cout << *it1 << " ";
    	++it1;
    }
    cout << endl;

    pzh::list<int>::reverse_iterator rit1 = lt1.rbegin();
    while (rit1 != lt1.rend())
    {
    	cout << *rit1 << " ";
    	++rit1;
    }
    cout << endl;
    func(lt1);

    pzh::vector<int> lt2;
    lt2.push_back(1);
    lt2.push_back(2);
    lt2.push_back(3);
    lt2.push_back(4);
    lt2.push_back(5);
    pzh::vector<int>::iterator it2 = lt2.begin();
    while (it2 != lt2.end())
    {
    	cout << *it2 << " ";
    	++it2;
    }
    cout << endl;
    pzh::vector<int>::reverse_iterator rit2 = lt2.rbegin();
    while (rit2 != lt2.rend())
    {
    	cout << *rit2 << " ";
    	++rit2;
    }
    cout << endl;
    func(lt2);

    Data<int, int> d1;
    Data<double, int> d2;
    Data<double*, int*> d3;
    Data<double&, int&> d4;

    Date* d5 = new Date(2025, 10, 8);
    Date* d6 = new Date(2025, 10, 9);
    cout << Less(d5, d6) << endl;

    Date* d7 = new Date(2025, 10, 8);
    Date* d8 = new Date(2025, 10, 9);
    cout << Less(d7, d8) << endl;

    Date* d9 = new Date(2025, 10, 8);
    Date* d0 = new Date(2025, 10, 9);
    cout << Less(d9, d0) << endl;
    cout << Less(1, 2) << endl;

    // 模板的分离编译问题
    // 在C++中，模板（函数模板和类模板）的定义通常需要放在头文件中
    // 因为模板的实例化是在编译时进行的。当编译器看到模板被使用时，它需要能够看到模板的定义才能生成特定类型的代码
    Add(1, 2);
    func();
    Add(1.1, 2.2);

    Stack<int> st;
    st.Push(1);
    st.Pop();

    Stack<double> stt;
    stt.Push(1);

    // 需要释放动态分配的Date对象，避免内存泄漏
    delete d5;
    delete d6;
    delete d7;
    delete d8;
    delete d9;
    delete d0;
    return 0;
}

