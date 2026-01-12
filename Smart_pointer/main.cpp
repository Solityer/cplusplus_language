#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include "SmartPtr.h"
using namespace std;

// ==========================================
// 模块1: RAII 与异常安全测试
// ==========================================
namespace Test_RAII
{
    // 为了不与全局 SmartPtr 冲突，重命名为 ArraySmartPtr
    // 专门用于演示管理数组资源 delete[]
    template<class T>
    class ArraySmartPtr
    {
    public:
        ArraySmartPtr(T* ptr)
            :_ptr(ptr)
        {}

        ~ArraySmartPtr()
        {
            if (_ptr)
            {
                cout << "  [ArraySmartPtr] delete[] " << _ptr << endl;
                delete[] _ptr;
            }
        }

        T& operator*()
        {
            return *_ptr;
        }

        T* operator->()
        {
            return _ptr;
        }

    private:
        T* _ptr;
    };

    double Division(int a, int b)
    {
        if (b == 0)
            throw invalid_argument("Division by zero condition (std::invalid_argument)!");
        return (double)a / (double)b;
    }

    void Func()
    {
        // RAII 管理资源
        ArraySmartPtr<int> sp1(new int[10]);   // 自动管理int数组
        ArraySmartPtr<double> sp2(new double[10]); // 自动管理double数组
        cout << "  资源已通过 ArraySmartPtr 分配" << endl;
        int len = 10, time = 0;

        // 这里抛出异常时，sp1和sp2的析构函数会被自动调用
        cout << Division(len, time) << endl;
    }

    void Run()
    {
        cout << "--- Test_RAII::Run Start ---" << endl;
        try
        {
            Func();
        }
        catch (const exception& e)
        {
            cout << "  捕获异常: " << e.what() << endl;
        }
        cout << "--- Test_RAII::Run End ---\n" << endl;
    }
}

// ==========================================
// 模块2: 基础智能指针测试
// ==========================================

void TestSmartPtr1()
{
    cout << "--- TestSmartPtr1 Start ---" << endl;
    SmartPtr<int> sp1(new int);
    *sp1 = 1;

    SmartPtr<pair<string, int>> sp2(new pair<string, int>("xxxx", 1));
    sp2->first += 'y';
    sp2->second += 1;
    sp2.operator->()->second += 1;
    cout << "  Pair value: " << sp2->first << ", " << sp2->second << endl;
    cout << "--- TestSmartPtr1 End ---\n" << endl;
}

void TestSmartPtr2()
{
    cout << "--- TestSmartPtr2 Start ---" << endl;
    SmartPtr<int> sp1(new int);
    // 注意：这里的 SmartPtr 是浅拷贝，析构时会double free，
    // 原代码中只是演示编译通过或原理，实际运行可能会崩，
    // 但为了保留原貌，这里保留代码，但建议注释掉运行以防崩溃。
    // SmartPtr<int> sp2 = sp1;
    cout << "  [Skip] SmartPtr copy test (double free risk in basic implementation)" << endl;
    cout << "--- TestSmartPtr2 End ---\n" << endl;
}

void test_auto_ptr1()
{
    cout << "--- test_auto_ptr1 Start ---" << endl;
    pzh::auto_ptr<int> ap1(new int(1));
    pzh::auto_ptr<int> ap2 = ap1;

    // 管理权转移，导致对象悬空
    // (*ap1)++; // 这行代码会报错，因为ap1已经悬空
    if(ap2.operator->())
    {
        (*ap2)++;
        cout << "  ap2 value: " << *ap2 << endl;
    }
    cout << "--- test_auto_ptr1 End ---\n" << endl;
}

// ==========================================
// 模块3: shared_ptr 与 weak_ptr 测试
// ==========================================

void test_shared_ptr2()
{
    cout << "--- test_shared_ptr2 Start ---" << endl;
    pzh::shared_ptr<string> sp1(new string("xxxxxxxxxxxxxxxxxx"));
    pzh::shared_ptr<string> sp2(sp1);
    pzh::shared_ptr<string> sp3(new string("yyyyyyyyy"));

    sp3 = sp3; // 自赋值检测
    cout << "  *sp3: " << *sp3 << endl;
    sp1 = sp2;
    cout << "  sp1 use_count: " << sp1.use_count() << endl;
    cout << "--- test_shared_ptr2 End ---\n" << endl;
}

struct ListNode
{
    int val;
    pzh::weak_ptr<ListNode> next;
    pzh::weak_ptr<ListNode> prev;

    ~ListNode()
    {
        cout << "~ListNode()" << endl;
    }
};

void test_shared_ptr3()
{
    cout << "--- test_shared_ptr3 (Cycle Reference) Start ---" << endl;
    pzh::shared_ptr<ListNode> n1(new ListNode);
    pzh::shared_ptr<ListNode> n2(new ListNode);
    cout << "  n1 use_count: " << n1.use_count() << endl;
    cout << "  n2 use_count: " << n2.use_count() << endl;

    // 循环引用测试
    n1->next = n2;
    n2->prev = n1;

    cout << "  After linking:" << endl;
    cout << "  n1 use_count: " << n1.use_count() << endl;
    cout << "  n2 use_count: " << n2.use_count() << endl;
    cout << "--- test_shared_ptr3 End ---\n" << endl;
}

// ==========================================
// 模块4: 定制删除器测试
// ==========================================

template<class T>
struct DelArray
{
    void operator()(T* ptr)
    {
        cout << "  Custom DelArray calling delete[]" << endl;
        delete[] ptr;
    }
};

void test_shared_ptr4()
{
    cout << "--- test_shared_ptr4 (Custom Deleter) Start ---" << endl;
    // 定制删除器 - 仿函数
    pzh::shared_ptr<ListNode> sp1(new ListNode[2], DelArray<ListNode>());

    // 定制删除器 - Lambda
    pzh::shared_ptr<ListNode> sp2(new ListNode[2], [](ListNode* ptr) {
        cout << "  Lambda delete[]" << endl;
        delete[] ptr;
    });

    // 定制删除器 - 文件指针
    pzh::shared_ptr<FILE> sp3(fopen("main.cpp", "r"), [](FILE* ptr) {
        cout << "  Closing file" << endl;
        if(ptr) fclose(ptr);
    });

    pzh::shared_ptr<ListNode> sp4(new ListNode);
    cout << "--- test_shared_ptr4 End ---\n" << endl;
}

// ==========================================
// 模块5: 特殊类设计 (HeapOnly / StackOnly)
// ==========================================

// 设计一个类，只能在堆上创建对象
class HeapOnly
{
public:
    static HeapOnly* CreateObj()
    {
        return new HeapOnly;
    }

    void Destroy()
    {
        delete this;
    }

    HeapOnly(const HeapOnly& hp) = delete;

private:
    // 构造函数私有化
    HeapOnly()
    {
        cout << "HeapOnly()" << endl;
    }

    ~HeapOnly()
    {
        cout << "~HeapOnly()" << endl;
    }
};

void TestHeapOnly()
{
    cout << "--- TestHeapOnly Start ---" << endl;
    // HeapOnly hp1; // 编译错误
    // static HeapOnly hp2; // 编译错误
    HeapOnly* ptr = HeapOnly::CreateObj();
    ptr->Destroy();
    cout << "--- TestHeapOnly End ---\n" << endl;
}

// 设计一个类，只能在栈上创建对象
class StackOnly
{
public:
    static StackOnly CreateObj()
    {
        StackOnly obj;
        return obj;
    }

    // 禁掉 operator new
    void* operator new(size_t size) = delete;

private:
    StackOnly()
    {
        cout << "StackOnly()" << endl;
    }
};

void TestStackOnly()
{
    cout << "--- TestStackOnly Start ---" << endl;
    StackOnly obj = StackOnly::CreateObj();
    // StackOnly* ptr = new StackOnly; // 编译错误：operator new deleted
    cout << "--- TestStackOnly End ---\n" << endl;
}

// ==========================================
// 模块6: 类型转换 (Cast) 测试
// ==========================================

class A
{
public:
    virtual void f()
    {}

    int _a = 0;
};

class B : public A
{
public:
    int _b = 1;
};

void fun(A* pa)
{
    // 向下转换：dynamic_cast 安全检查
    B* ptr = dynamic_cast<B*>(pa);
    if (ptr)
    {
        ptr->_a++;
        ptr->_b++;
        cout << "  Conversion Success: _a=" << ptr->_a << ", _b=" << ptr->_b << endl;
    }
    else
    {
        cout << "  Conversion Failed" << endl;
    }
}

void TestCast()
{
    cout << "--- TestCast Start ---" << endl;
    // 1. static_cast
    double d = 12.34;
    int a = static_cast<int>(d);
    cout << "  static_cast double to int: " << a << endl;

    // 2. reinterpret_cast
    // 注意：将 int 转为指针在 64 位系统下可能会有警告或截断，这里仅演示语法
    long long val = a;
    int* ptr = reinterpret_cast<int*>(val);
    cout << "  reinterpret_cast int to int*: " << ptr << endl;

    // 3. const_cast
    volatile const int c = 2;
    int* p = const_cast<int*>(&c);
    *p = 3;
    cout << "  const_cast (modifying const, undefined behavior check): c=" << c << ", *p=" << *p << endl;

    // 4. dynamic_cast
    A objA;
    B objB;
    cout << "  Testing dynamic_cast with parent object:" << endl;
    fun(&objA); // 失败
    cout << "  Testing dynamic_cast with child object:" << endl;
    fun(&objB); // 成功
    cout << "--- TestCast End ---\n" << endl;
}

void TestInsert()
{
    cout << "--- TestInsert Start ---" << endl;
    // 原代码演示了无符号数回绕的死循环风险，这里做个简单演示即可
    // size_t pos = 0;
    // int end = 10;
    // while (end >= pos) ... // 如果pos是size_t且end是int，比较时可能会有问题
    cout << "  (Insert function logic test skipped to avoid infinite loop output)" << endl;
    cout << "--- TestInsert End ---\n" << endl;
}

int main()
{
    // 1. RAII 与异常安全测试
    Test_RAII::Run();

    // 2. 基础 SmartPtr 测试
    TestSmartPtr1();
    TestSmartPtr2();

    // 3. auto_ptr 测试
    test_auto_ptr1();

    // 4. shared_ptr 基础与循环引用测试
    test_shared_ptr2();
    test_shared_ptr3();

    // 5. shared_ptr 定制删除器测试
    test_shared_ptr4();

    // 6. 特殊类设计测试
    TestHeapOnly();
    TestStackOnly();

    // 7. 类型转换测试
    TestCast();

    return 0;
}