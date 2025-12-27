#include <iostream>
using namespace std;

class Person1
{
public:
    void Print()
    {
        cout << "name:" << _name << endl;
        cout << "age:" << _age << endl;
    }
protected:
    string _name = "peter"; // 姓名
    int _age = 18; // 年龄
};

// 继承后父类的Person的成员（成员函数+成员变量）都会变成子类的一部分。
// protected 类外面不能使用，类里面可以使用，还可以继承
/*
 * Student 是派生类(子类)
 * public 是继承方式
 * Person 是基类(父类)
 */
/* 不可见和私有的区别:
 * 不可见类里面类外面都不能用
 * 私有类外面不能使用，类里面可以使用
 */

class Student1 : public Person1
{
protected:
    int _stuid; // 学号
};
class Teacher1 : public Person1
{
protected:
    int _jobid; // 工号
};

//////////////基类和派生类对象赋值转换////////////////////
class Person2
{
protected:
	string _name; // 姓名
	string _sex;  // 性别
	int	_age;	 // 年龄
};

class Student2 : public Person2
{
public:
	int _No; // 学号
};

/////////////////继承中的作用域//////////////////////////
class Person3
{
protected:
	string _name = "小李子"; // 姓名
	int _num = 111; 	   // 身份证号
};

/*
 * 继承中的同名成员处理：
 * 1. 父类和子类可以有同名成员，它们位于不同的作用域
 * 2. 默认情况下直接访问子类的成员
 * 3. 子类同名成员会隐藏父类同名成员（名称隐藏）
 * 4. 对于同名成员函数，只要函数名相同就构成隐藏，不关心参数和返回值
 */
class Student3 : public Person3
{
public:
	void Print()
	{
		cout << " 姓名:" << _name << endl;
		cout << " 身份证号:" << Person3::_num << endl;
		cout << " 学号:" << _num << endl;
	}
protected:
	int _num = 999; // 学号
};

////////////////派生类的默认成员函数/////////////////////////
class Person4
{
public:
    Person4(const char* name = "peter")
        : _name(name)
    {
        cout << "Person4()" << endl;
    }

    Person4(const Person4& p)
        : _name(p._name)
    {
        cout << "Person4(const Person4& p)" << endl;
    }

    Person4& operator=(const Person4& p)
    {
        cout << "Person4 operator=(const Person4& p)" << endl;
        if (this != &p)
            _name = p._name;
        return *this;
    }

    ~Person4()
    {
        cout << "~Person4()" << endl;
    }

protected:
    string _name; // 姓名
};

class Student4 : public Person4
{
public:
    Student4(const char* name, int id)
        : _id(id)
        , Person4(name)
    {
        cout << "Student4(const char* name, int id)" << endl;
    }

    Student4(const Student4& s)
        : Person4(s)
        , _id(s._id)
    {
        cout << "Student(const Student& s)" << endl;
    }

    Student4& operator=(const Student4& s)
    {
        if (&s != this)
        {
            Person4::operator=(s);
            _id = s._id;
        }
        cout << "Student4& operator=(const Student4& s)" << endl;
        return *this;
    }

    // 由于多态的原因，析构函数统一会被处理成destructor
    // 父子类的析构函数构成隐藏,为了保证析构安全，先子后父
    // 父类析构函数不需要显示调用，子类析构函数结束时会自动调用父类析构,保证先子后父
    ~Student4()
    {
        //Person::~Person();
        cout << "~Student4()" << endl;
    }

protected:
    int _id;
};

//////////////继承和友元////////////////////
// 友元关系不能被继承  类似爸爸的朋友不等于是你的朋友
class Student5;
class Person5
{
public:
    friend void Display(const Person5& p, const Student5& s);
protected:
    string _name; // 姓名
};

class Student5 : public Person5
{
    friend void Display(const Person5& p, const Student5& s);
protected:
    int _stuNum; // 学号
};

void Display(const Person5& p, const Student5& s)
{
    cout << p._name << endl;
    cout << s._stuNum << endl;  //Student5加上友元才不会报错
}

//////////////继承与静态成员////////////////////
class Person6
{
public:
    Person6() { ++_count; }
protected:
    string _name; // 姓名
public:
    static int _count; // 统计人的个数。
};

int Person6::_count = 0;

class Student6 : public Person6
{
protected:
    int _stuNum; // 学号
};

Student6 func6()
{
    Student6 st;
    return st;
}

/////////复杂的菱形继承及菱形虚拟继承///////////////
class Person7
{
public:
    string _name; // 姓名
    int _age;
    int _tel;
};

//虚继承的作用：虚继承解决了菱形继承问题，确保在多重继承中，基类A只有一个实例。
class Student7 : virtual public Person7
{
protected:
    int _num; //学号
};
class Teacher7 : virtual public Person7
{
protected:
    int _id; // 职工编号
};

class Assistant7 : public Student7, public Teacher7
{
protected:
    string _majorCourse; // 主修课程
};

void Test()
{
    // 这样会有二义性无法明确知道访问的是哪一个
    Assistant7 a;
    a.Student7::_name = "小张";
    a.Teacher7::_name = "老张";
    a._name = "张三";
}

////////////虚拟继承解决数据冗余和二义性的原理//////////////////
class A
{
public:
	//int _a[100];  // 测试数据冗余
	int _a;
};


//class B : public A   // 普通继承 vs 虚继承
class B : virtual public A
{
public:
	int _b;
};

//class C : public A  // 普通继承 vs 虚继承
class C : virtual public A
{
public:
	int _c;
};

class D : public B, public C
{
public:
	int _d;
};

///////////////////////////////////////////////////////
//普通类继承
class A1
{
public:
	void func7()
	{}
protected:
	int _a1;
};

class B1 : public A1
{
public:
    void f()
    {
        func7();  //父类的共有可以使用
        ++_a1;  //父类的保护也可以使用
    }
protected:
	int _b1;
};

// 对象组合
class C1
{
public:
    void func()
    {}
protected:
	int _c1;
};

class D1
{
public:
    void f()
    {
        _c2.func();  // 组合类的共有可以使用
        // _c2._c1++;   //组合类的保护不可以使用
    }
protected:
	C1 _c2;
	int _d1;
};


int main()
{
    //////////////继承的概念和定义////////////////////
    Student1 s1;
    Teacher1 t1;
    s1.Print();
    t1.Print();

    //////////////f父子类的复赋值转换////////////////////
    double d = 2.2;
    int i = d;
    const int& r = d;

    //////////////基类和派生类对象赋值转换////////////////////
    string str = "xxxx";
    const string& rstr = "xxxx";
    Student2 s2;
    Person2 p2 = s2;
    Person2& rp2 = s2;

    //////////////派生类的默认成员函数////////////////////
    Student3 s3;
    s3.Print();

    Student4 s4("张三", 18);
    Student4 s5(s4);
    Student4 s6("李四", 19);
    s4 = s6;

    //////////////继承和友元////////////////////
    Person5 p5;
    Student5 s7;
    Display(p5, s7);

    ////////////继承与静态成员//////////////////
    // 静态变量继承的是使用权，用的是同一个地址
    cout << &Person6::_count << endl;
    cout << &Student6::_count << endl;
    Person6 p6;
    Student6 s8;
    func6();
    cout << Student6::_count << endl;

    /////////复杂的菱形继承及菱形虚拟继承///////////////
    Test();

    // ////////////虚拟继承解决数据冗余和二义性的原理//////////////////
    D d1;
    // _a 在虚继承中是共享的，只有一个实例
    d1.B::_a = 1;
    d1.C::_a = 2;
    d1._b = 3;
    d1._c = 4;
    d1._d = 5;
    d1._a = 0;

    cout << sizeof(D) << endl;
    cout << sizeof(B) << endl;
    D1 dd;
    //dd.func();
    B1 bb;
    bb.func7();  //继承的权限更大

    return 0;
}
