#include <iostream>
using namespace std;

//////////////////////////////////多态的概念/////////////////////////////////////
class Person1
{
public:
    // 虚函数：被virtual修饰的类成员函数称为虚函数。
    virtual void BuyTicket()
    {
        cout << "Person1买票-全价" << endl;
    }

    virtual ~Person1()
    {
        cout << "~Person1()" << endl;
    }

    virtual void func()
    {}

private:
    int a = 0;
};

class Student1 : public Person1
{
public:
    // 虚函数重写/覆盖：派生类重写基类的虚函数
    // 重写条件：函数名、参数、返回值相同，且基类函数有virtual关键字
    virtual void BuyTicket()
    {
        cout << "Student1买票-半价" << endl;
    }

    /*注意：在重写基类虚函数时，派生类的虚函数在不加virtual关键字时，虽然也可以构成重写(因
    为继承后基类的虚函数被继承下来了在派生类依旧保持虚函数属性),但是该种写法不是很规范，不建议
    这样使用*/
    /*void BuyTicket() { cout << "买票-半价" << endl; }*/

    ~Student1()
    {
        cout << "~Student1()" << endl;
    }

private:
    int b = 1;
};

/*
 * 多态的两个必要条件：
 * 1. 虚函数重写：派生类重写基类的虚函数
 * 2. 父类的指针或者引用去调用虚函数
 *
 * 虚函数重写的细节：
 * - 需要在继承关系的两个类中
 * - 三同：函数名、参数、返回值相同（协变除外）
 * - virtual只能修饰成员函数
 * - 协变：返回值可以不同，但必须是父子类关系的指针或引用
 * - 派生类重写的虚函数可以不加virtual（但建议加上）
 */

// 使用引用调用虚函数
void Func1(Person1& p)
{
    p.BuyTicket();  // 多态调用：运行时根据实际对象类型决定调用哪个函数
}

// 使用指针调用虚函数
void Func2(Person1* p)
{
    p->BuyTicket();  // 多态调用
}

// 值传递调用虚函数（不会触发多态）
void Func4(Person1 ptr)
{
    ptr.BuyTicket();  // 不是多态：切片后调用Person1的BuyTicket
}

//////////////////////////////////////虚函数重写///////////////////////////////////////////////////////
class A1
{
public:
    void func()
    {}
protected:
    int _a;
};

class B1 : public A1
{
public:
    void f()
    {
        func();
        _a++;
    }
protected:
    int _b;
};

class Person2
{
public:
    virtual A1* BuyTicket()
    {
        cout << "Person2买票-全价" << endl;
        return nullptr;
    }

    virtual ~Person2()
    {
        cout << "~Person2()" << endl;
    }
};

class Student2 : public Person2
{
public:
    virtual B1* BuyTicket()
    {
        cout << "Student2买票-半价" << endl;
        return nullptr;
    }

    /*注意：在重写基类虚函数时，派生类的虚函数在不加virtual关键字时，
     *虽然也可以构成重写(因为继承后基类的虚函数被继承下来了在派生类依旧保持虚函数属性),但是该种写法不是很规范，不建议这样使用*/
    /*void BuyTicket() { cout << "买票-半价" << endl; }*/

    ~Student2()
    {
        cout << "~Student2()" << endl;
    }
};

////////////////////////////////////////////////////////////////////////////////////////
class A2
{
public:
    virtual void func(int val = 1)
    {
        std::cout << "A2->" << val << std::endl;
    }
    virtual void test()
    {
        func();
    }
};

class B2 : public A2
{
public:
	void func(int val = 0)
	{
	    std::cout << "B2->" << val << std::endl;
	}
};

////////////////////////////////override和final///////////////////////////////////////////////
// final 修饰类，不能被继承
// final 修饰虚函数，不能被重写
class Car1
{
public:
    virtual void Drive() //final
    {}
};

class Benz1 : public Car1
{
public:
    virtual void Drive() { cout << "Benz-舒适" << endl; }
};

////////////////////////////////////override和final/////////////////////////////////////////////
//override: 检查派生类虚函数是否重写了基类某个虚函数，如果没有重写编译报错
class Car2
{
public:
    virtual void Drive(){}
};

class Benz2 : public Car2
{
public:
    // override修饰派生类的虚函数
    // 检查是否完成重写
    virtual void Drive() override { cout << "Benz-舒适" << endl; }
};

//////////////////////////////////抽象类//////////////////////////////////////////////////////
class Car3
{
public:
    // 纯虚函数：在虚函数声明末尾加上 = 0
    // 纯虚函数的作用：
    // 1. 强制派生类去重写该函数
    // 2. 使类成为抽象类，不能实例化对象
    virtual void Drive() = 0;

    // 抽象类可以有普通成员函数
    void func()
    {
        cout << "void func()" << endl;
    }
};

class Benz3 : public Car3
{
public:
    // 纯虚函数规范了派生类必须重写
    virtual void Drive()
    {
        cout << "Benz3-舒适" << endl;
    }
};

class BMW3 : public Car3
{
public:
    virtual void Drive()
    {
        cout << "BMW3-操控" << endl;
    }
};

void func3(Car3* ptr)
{
    ptr->Drive();
}

///////////////////////////////虚函数表////////////////////////////////////////
class Base
{
public:
    virtual void Func1()
    {
        cout << "Base::Func1()" << endl;
    }
    virtual void Func2()
    {
        cout << "Base::Func2()" << endl;
    }
    void Func3()
    {
        cout << "Base::Func3()" << endl;
    }
private:
    int _b = 1;
};
class Derive : public Base
{
public:
    virtual void Func1()
    {
         cout << "Derive::Func1()" << endl;
    }
private:
    int _d = 2;
};


int main()
{
    Person1 ps1;
    Student1 st1;
    Func1(ps1);
    Func1(st1);
    //////////////////////虚函数重写/////////////////////////////////////
    Person2* p2 = new Person2;
    // 析构是虚函数，才能正确调用析构函数
    // p2->destrutor() + operator delete(p2)
    delete p2;
    p2 = new Student2;
    // p2->destrutor() + operator delete(p2)
    delete p2;
    ///////////////////////////////////////////////////////
    Person1* ptr1 = new Person1;
    delete ptr1;
    ptr1 = new Student1;
    delete ptr1;
    //////////////////////////////////////////////////////////////////////////
    B2* p3 = new B2;
    p3->test();
    p3->func();
    //////////////////////////////抽象类///////////////////////////////////////////
    func3(new Benz3);
    func3(new BMW3);
    ////////////////////////////////虚函数表///////////////////////////////////////
    cout << sizeof(Base) << endl;
    Base b;
    Derive d;
    /////////////////////////////多态的原理///////////////////////////////////////
    Person1 p4;
    Person1 p5;
    Person1 p6;
    Student1 s1;
    Student1 s2;
    // 注意：Func4是值传递，会发生切片，不会触发多态
    Func4(p4);
    Func4(s1);

    return 0;
}
