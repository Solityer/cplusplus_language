#include <iostream>
#include "Date.h"
#include "Stack.h"

using namespace std;

// === 演示静态成员与内部类 ===
class AdvancedFeature
{
public:
    AdvancedFeature()
    {
        ++_count;
    }

    AdvancedFeature(const AdvancedFeature &a)
    {
        ++_count;
    }

    ~AdvancedFeature()
    {
        --_count;
    }

    static int GetCount()
    {
        return _count;
    }

    // 内部类 (Inner Class)
    // 内部类天生是外部类的友元，可以访问外部类的私有成员(如 _privateData)
    class InnerObserver
    {
    public:
        void Inspect(const AdvancedFeature &outer)
        {
            cout << "[InnerClass] Accessing outer static private: " << _privateStaticVar << endl;
            cout << "[InnerClass] Accessing outer instance private: " << outer._privateData << endl;
        }
    };

private:
    static int _count;
    static int _privateStaticVar;
    int _privateData = 999;
};

// 静态成员变量必须在类外初始化
int AdvancedFeature::_count = 0;
int AdvancedFeature::_privateStaticVar = 42;

// === 测试函数集 ===

void TestDateBasic()
{
    cout << "=== TestDateBasic ===" << endl;
    Date d1(2026, 1, 1);
    Date d2 = d1; // 调用拷贝构造
    d1.Print();

    d1 += 100; // 调用 operator+=
    cout << "d1 after += 100: " << d1 << endl; // 调用 operator<<

    // 测试前置与后置++
    Date d3 = d1++; // d3 等于 d1旧值，d1自增
    cout << "d3 (old d1): " << d3 << ", d1 (new): " << d1 << endl;
}

void TestStackMemory()
{
    cout << "\n=== TestStackMemory ===" << endl;
    Stack s1(10);
    s1.Push(1);
    s1.Push(2);

    // 调用拷贝构造函数（深拷贝）
    // 如果没有自定义拷贝构造函数，这里会发生浅拷贝，导致s1和s2指向同一块内存
    // 最终析构时会发生 Double Free 错误
    Stack s2(s1);

    cout << "s1: ";
    s1.Print();
    cout << "s2: ";
    s2.Print();
}

void TestStaticAndInner()
{
    cout << "\n=== TestStaticAndInner ===" << endl;
    cout << "Initial Count: " << AdvancedFeature::GetCount() << endl;

    AdvancedFeature a1;
    AdvancedFeature a2;

    cout << "Count after creating 2 objects: " << AdvancedFeature::GetCount() << endl;

    AdvancedFeature::InnerObserver observer;
    observer.Inspect(a1);
}

int main()
{
    // 1. 测试日期类的封装与操作符重载
    TestDateBasic();

    // 2. 测试栈类的资源管理与深拷贝
    TestStackMemory();

    // 3. 测试高级特性
    TestStaticAndInner();

    return 0;
}
