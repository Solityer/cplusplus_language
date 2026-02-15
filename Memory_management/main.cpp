#include <iostream>
#include <cstdlib>   // for malloc, calloc, realloc, free
#include <new>       // for placement new, bad_alloc
#include <exception> // for exception
using namespace std;


// C/C++ 内存分布
namespace Memory_Layout
{

    int globalVar = 1;              // 数据段 - 全局变量
    static int staticGlobalVar = 1; // 数据段 - 静态全局变量

    void Test()
    {
        static int staticVar = 1; // 数据段 - 局部静态变量

        int localVar = 1;            // 栈 - 局部变量
        int num1[10] = {1, 2, 3, 4}; // 栈 - 数组

        char char2[] = "abcd";       // 栈 - 字符数组
        const char *pChar3 = "abcd"; // 代码段 - 常量字符串指针

        // 堆 (Heap) - 动态分配
        int *ptr1 = (int *)malloc(sizeof(int) * 4);
        int *ptr2 = (int *)calloc(4, sizeof(int));
        int *ptr3 = (int *)realloc(ptr2, sizeof(int) * 4);

        cout << "[Memory_Layout] Memory layout test executed." << endl;

        free(ptr1);
        free(ptr3); // ptr2 已被 realloc 处理，无需单独释放
    }
}


// 基础类型分配对比 (malloc/free vs new/delete)
namespace Basic_Allocation
{
    void Test()
    {
        cout << "--------------------------------------------------------" << endl;
        cout << "[Basic_Allocation] Testing Primitives..." << endl;

        // 1. C语言模式: malloc/free
        int *p2 = (int *)malloc(sizeof(int));
        int *p4 = (int *)malloc(sizeof(int) * 10);
        free(p2);
        free(p4);

        // 2. C++模式: new/delete
        // 优势: 自动计算大小，不需要强制类型转换
        int *p3 = new int;
        int *p5 = new int[10];

        delete p3;
        delete[] p5; // 数组释放必须匹配 delete[]

        // 3. C++ 额外支持: 开空间 + 初始化
        int *p6 = new int(10);          // 申请一个int，初始化为10
        int *p7 = new int[10]{1, 2, 3}; // 申请10个int，前三个初始化为1,2,3，其余为0
        int *p8 = new int[10]{};        // 申请10个int，全部初始化为0

        delete p6;
        delete[] p7;
        delete[] p8;

        cout << "[Basic_Allocation] Test finished." << endl;
    }
}

// 自定义类型的内存管理 (Custom Objects)
namespace Object_Management
{

    class A
    {
    public:
        A(int a = 0)
            : _a(a)
        {
            cout << "A():" << this << " val=" << _a << endl;
        }

        ~A()
        {
            cout << "~A():" << this << endl;
        }

    private:
        int _a;
    };

    struct ListNode
    {
        ListNode *_next;
        int _val;

        ListNode(int val = 0)
            : _val(val), _next(nullptr)
        {
        }
    };

    void Test()
    {
        cout << "--------------------------------------------------------" << endl;
        cout << "[Object_Management] Testing Custom Objects..." << endl;

        // 1. malloc/free: 仅分配空间，不调用构造/析构函数
        // malloc没有办法很好支持动态申请的自定义对象初始化
        cout << "--- Malloc Context ---" << endl;
        A *p1 = (A *)malloc(sizeof(A));
        // p1->A(1); // 错误：不能显式调用构造函数
        free(p1);

        // 2. new/delete: 分配空间 + 调用构造/析构函数
        cout << "--- New/Delete Context ---" << endl;
        A *p2 = new A;    // 调用默认构造
        A *p3 = new A(3); // 调用带参构造

        delete p2; // 调用析构 + 释放空间
        delete p3;

        // 3. 对象数组
        cout << "--- Array Context ---" << endl;
        A *p4 = new A[2];
        delete[] p4;

        // 4. 对象数组的显式初始化 (C++11)
        A aa1(1);
        A aa2(2);
        A *p5 = new A[2]{aa1, aa2}; // 拷贝构造
        delete[] p5;

        // 匿名对象构造
        A *p6 = new A[2]{A(10), A(20)};
        delete[] p6;

        // 隐式类型转换构造
        A *p7 = new A[2]{100, 200};
        delete[] p7;

        // 5. 链表节点的应用
        cout << "--- ListNode Context ---" << endl;
        ListNode *n1 = new ListNode(1);
        ListNode *n2 = new ListNode(2);
        ListNode *n3 = new ListNode(3);

        n1->_next = n2;
        n2->_next = n3;

        // 清理链表
        delete n1;
        delete n2;
        delete n3;
    }
}


// 栈类实现与异常处理 (Stack Implementation & Exceptions)
namespace Stack_Exceptions
{
    typedef int DataType;

    class Stack
    {
    public:
        Stack(size_t capacity = 4)
        {
            cout << "Stack(capacity=" << capacity << ")" << endl;
            _array = new DataType[capacity];
            _size = 0;
            _capacity = capacity;
        }

        void Push(DataType data)
        {
            if (_size < _capacity)
            {
                _array[_size] = data;
                _size++;
            }
        }

        ~Stack()
        {
            cout << "~Stack()" << endl;
            if (_array)
            {
                delete[] _array;
                _array = nullptr;
            }
            _size = _capacity = 0;
        }

    private:
        DataType *_array;
        int _capacity;
        int _size;
    };

    // 模拟抛出异常的函数
    void func_throw()
    {
        // 尝试分配极大内存，触发 bad_alloc
        // 注意：不同编译器/系统下 0x7fffffff 行为可能不同，这里使用一个显著的大数
        // 为了演示效果，可能需要根据实际环境调整大小，或者直接由 std::bad_alloc 抛出
        // char* p5 = new char[0x7ffffffff];

        // cout << "Attempting huge allocation..." << endl;
        // char *p1 = new char[2000000000]; // 尝试分配约2GB，在32位程序下必挂，64位视内存而定
        // cout << (void *)p1 << endl;      // 如果成功则打印地址
        // cout << "func_throw() finished" << endl;
    }

    void Test()
    {
        cout << "--------------------------------------------------------" << endl;
        cout << "[Stack_Exceptions] Testing Stack and Exceptions..." << endl;

        // 基础 Stack 测试
        Stack *pst = new Stack(4);
        pst->Push(1);
        delete pst;

        // 异常捕获测试
        // 内存申请失败抛异常,会导致执行流跳跃，和c语言的goto相似
        try
        {
            // 可以直接try，也可以间接try
            func_throw();
        }
        catch (const exception &e)
        {
            cout << "Exception caught: " << e.what() << endl;
        }
    }
}

// 底层操作与定位New (Low-Level & Placement New)
namespace LowLevel
{
    using Stack_Exceptions::Stack; // 复用 Stack 类

    void Test()
    {
        cout << "--------------------------------------------------------" << endl;
        cout << "[LowLevel] Testing operator new & placement new..." << endl;

        // 1. operator new / operator delete
        // 它们是库函数，不是运算符重载。new 底层就是调用 operator new + 构造函数
        Stack *pst1 = (Stack *)operator new(sizeof(Stack));

        // 2. 定位 new (Placement New)
        // 用于对已分配的原始内存空间调用构造函数初始化
        // 语法: new(指针) 类型(参数)

        // pst1->Stack(4);  // 错误：构造函数不能显式调用
        new (pst1) Stack(4); // 显式调用构造函数初始化

        // 3. 析构与释放
        // 对于定位new创建的对象，需要显式调用析构函数
        pst1->~Stack();

        // 最后释放内存
        operator delete(pst1);

        // 4. Mismatch Warnings
        /*
        int* p1 = new int[10];
        free(p1);    // 错误用法
        delete p1;   // 错误用法
        delete[] p1; // 正确用法
        */
        cout << "[LowLevel] Placement new test finished." << endl;
    }
}

int main()
{
    // 1. 内存分布展示
    Memory_Layout::Test();

    // 2. 基础类型分配 (C vs C++)
    Basic_Allocation::Test();

    // 3. 对象分配与初始化
    Object_Management::Test();

    // 4. 类封装与异常处理
    Stack_Exceptions::Test();

    // 5. 底层内存操作 (Placement New)
    LowLevel::Test();

    return 0;
}