#include <iostream>
using namespace std;

class Date
{
    //友元函数可以直接访问类的私有成员，它是定义在类外部的普通函数，不属于任何类，但需要在类的内部声明，声明时需要加friend关键字
    friend ostream& operator<<(ostream& _cout, const Date& d);
    friend istream& operator>>(istream& _cin, Date& d);
public:
    //初始化列表
    //以逗号分隔的数据成员列表，每个"成员变量"后面跟一个放在括号中的初始值或表达式
    //成员变量在类中声明次序就是其在初始化列表中的初始化顺序
    Date(int year = 1970, int month = 1, int day = 1)
        : _year(year)  //将-year初始化为year值
        , _month(month)
        , _day(day)
    {}

//    // 单参构造函数，没有使用explicit修饰，具有类型转换作用
//    // explicit修饰构造函数，禁止类型转换
//    explicit Date(int year)
//            :_year(year)
//    {
//        cout << "explicit Date(int year)" << endl;
//    }

//    // 虽然有多个参数，但是创建对象时,后两个参数可以不传递
//    // explicit修饰构造函数，禁止类型转换---explicit去掉之后，代码可以通过编译
//    explicit Date(int year, int month = 1, int day = 1)
//            : _year(year)
//            , _month(month)
//            , _day(day)
//    {
//        cout << "explicit Date(int year, int month = 1, int day = 1)" << endl;
//    }

    Date& operator=(const Date& d)
    {
        if(this != &d)
        {
            _year = d._year;
            _month = d._month;
            _day = d._day;
        }
        return *this;
    }

    void print()
    {
        cout << _year << "年" << _month << "月" << _day << "日" << endl;
    }
private:
    int _year;
    int _day;
    int _month;
};

class S
{
public:
    S(int a)
        :_a1(a)
    {}

    void Print()
    {
        cout << _a1 << endl;
    }
private:
    int _a1;
};

class A
{
public:
    A()
    {
        ++_scount;
    }

    A(const A& t)
    {
        ++_scount;
    }

    ~A()
    {
        --_scount;
    }

    static int GetACount()
    {
        return _scount;
    }

    class B  //内部类，天生就是A类的友元
    {
    public:
        void foo(const A& a)
        {
            cout << k << endl;
            cout << a.h << endl;
        }
    };

private:
    static int _scount;
    static int k;
    int h;
};
int A::_scount = 0;
int A::k = 1;

void TestA()
{
    cout << A::GetACount() << endl;
    A a1, a2;
    A a3(a1);
    cout << A::GetACount() << endl;
}

//友元函数
ostream& operator<<(ostream& _cout, const Date& d)
{
    _cout << d._year << "-" << d._month << "-" << d._day;
    return _cout;
}
istream& operator>>(istream& _cin, Date& d)
{
    _cin >> d._year;
    _cin >> d._month;
    _cin >> d._day;
    return _cin;
}


int main()
{
    Date d(2024,3,12);
    d.print();

    S aa1(2); //直接构造
    aa1.Print();
    // 单参数构造函数的隐式类型转换
    // 用2调用A构造函数生成一个临时对象，再用这个对象去拷贝构造aa2
    // 编译器会再优化，优化用2直接构造
    S aa2 = 2;
    aa2.Print();

    //explicit测试
    Date d1(2025);  //直接调用构造函数
    // Date d2 = 2024;  //隐式类型转换，直接用2024构造d2  编译报错，去掉explicit则可以正常运行

    TestA();

    Date d3;
    cin >> d3;
    cout << d3 << endl;

    A::B b;
    b.foo(A());

    return 0;
}