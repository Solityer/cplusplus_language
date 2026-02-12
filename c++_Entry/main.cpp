#include <iostream>
#include <vector>
#include <string>
#include <typeinfo>  // RTTI 支持: typeid()
#include <time.h>    // 时间度量
#include <stdlib.h>  // C 标准库: malloc, free, rand
#include <assert.h>  // 断言宏

// 引入标准输入输出流，但在工程中通常建议仅在 .cpp 文件中展开，避免头文件污染
using std::cout;
using std::endl;
using std::cin;

// ============================================================================
// [SECTION 1] 核心数据结构库 (Core Library)
// ----------------------------------------------------------------------------
// 描述：包含栈、队列、链表的基础定义。
// 对比：展示 C 语言（Struct + 函数）与 C++（Class + 成员函数）的封装差异。
// ============================================================================
namespace CoreLib
{
    namespace pzh
    {
        // C 风格栈结构体定义
        typedef struct Stack
        {
            int *a; // 动态数组基地址
            int top; // 栈顶索引（指向下一个可用位置）
            int capacity; // 当前物理容量
        } ST;

        /**
         * @brief 栈初始化函数
         * @param ps 栈结构体指针
         * @param N 初始容量 (缺省参数: 4)
         * * [编译器原理] 缺省参数只需在声明处指定。若在定义处重复指定，将导致编译二义性错误。
         */
        void StackInit(ST *ps, int N = 4);

        /**
         * @brief 入栈操作
         * @note 包含基于倍增策略（Amortized Doubling）的动态扩容逻辑。
         */
        void StackPush(ST *ps, int x);

        void QueueInit();

        // 嵌套命名空间：演示作用域的层级隔离
        namespace pzh1
        {
            // 全局静态存储区变量，与 std::rand 隔离
            int rand = 1;
        }
    }

    // ---------------------------------------------------------
    // 抽象数据类型 (ADT) 定义
    // ---------------------------------------------------------
    namespace AQueue
    {
        struct Node
        {
            struct Node *next;
            int val;
        };

        struct Queue
        {
            struct Node *head;
            struct Node *tail;
        };
    }

    namespace BList
    {
        struct Node
        {
            struct Node *next;
            struct Node *prev;
            int val;
        };
    }

    // ---------------------------------------------------------
    // C++ 面向对象风格实现
    // ---------------------------------------------------------
    /**
     * @class CppStack
     * @brief 利用 C++ 的类机制实现封装 (Encapsulation)
     * * [安全性分析]
     * 成员变量 private 化是必要的。在密码学库中，这防止了外部直接修改多项式系数
     * 或椭圆曲线点坐标，从而保证内部状态的不变量（Invariant）始终成立。
     */
    class CppStack
    {
    public:
        void Init(); // 初始化 (类似于构造函数逻辑)
        void Push(int x); // 入栈
        int Top(); // 获取栈顶元素

    private:
        int *a;
        int top;
        int capacity;
    };

    // 辅助数据结构：链表与树节点
    typedef struct ListNode
    {
        int val;
        struct ListNode *next;
    } ListNode, *PListNode;

    struct TreeNode
    {
        int val;
        struct TreeNode *left;
        struct TreeNode *right;
    };

    // ---------------------------------------------------------
    // 实现区域 (Implementation Block)
    // ---------------------------------------------------------

    // pzh 命名空间实现
    void pzh::StackInit(ST *ps, int N)
    {
        // [内存管理] malloc 分配的是未初始化的原始内存。
        // 在 C++ 中，推荐使用 new，因为 new 会自动调用构造函数。
        ps->a = (int *) malloc(sizeof(int) * N);
        if (ps->a == NULL)
        {
            perror("malloc failed"); // 输出 errno 对应的错误信息
            exit(-1);
        }
        ps->top = 0;
        ps->capacity = N;
    }

    void pzh::StackPush(ST *ps, int x)
    {
        if (ps->top == ps->capacity)
        {
            // 扩容逻辑：如果容量为0则初始化为4，否则翻倍
            int newcapacity = ps->capacity == 0 ? 4 : ps->capacity * 2;

            // [内存管理] realloc 可能原地扩容，也可能异地开辟+拷贝+释放原内存。
            int *tmp = (int *) realloc(ps->a, sizeof(int) * newcapacity);
            if (tmp == NULL)
            {
                perror("realloc failed");
                exit(-1);
            }
            ps->a = tmp;
            ps->capacity = newcapacity;
        }
        ps->a[ps->top++] = x;
    }

    void pzh::QueueInit()
    {
        cout << "[CoreLib] void QueueInit() invoked." << endl;
    }

    // CppStack 类成员实现
    void CppStack::Init()
    {
        a = nullptr; // nullptr (std::nullptr_t) 比 NULL (0) 类型更安全
        top = capacity = 0;
    }

    void CppStack::Push(int x)
    {
        if (top == capacity)
        {
            size_t newcapacity = capacity == 0 ? 4 : capacity * 2;
            int *tmp = (int *) realloc(a, sizeof(int) * newcapacity);
            if (tmp == nullptr)
            {
                perror("realloc failed");
                exit(-1);
            }
            a = tmp;
            capacity = newcapacity;
        }
        a[top++] = x;
    }

    int CppStack::Top()
    {
        assert(top > 0);
        return a[top - 1];
    }
}

// ============================================================================
// [SECTION 2] 语言机制深度验证 (Language Mechanism Verification)
// 描述：涵盖命名空间、函数重载、引用语义、内联与宏、递归算法等高级特性的验证。
// ============================================================================
namespace Module_Systematic
{
    using namespace CoreLib;

    // 全局变量演示 (Global Scope)
    int global_val = 1;

    // ---------------------------------------------------------
    // 2.1 作用域与命名空间 (Scope & Resolution)
    // ---------------------------------------------------------
    void Func_Namespace()
    {
        cout << "\n--- [Systematic] Namespace & Scope ---" << endl;
        int global_val = 0; // 局部变量屏蔽同名全局变量

        cout << "Local global_val: " << global_val << endl; // 输出 0

        // 域作用限定符 (::) 强制访问全局符号表
        cout << "Global ::Module_Systematic::global_val: " << Module_Systematic::global_val << endl; // 输出 1

        // 访问嵌套命名空间
        cout << "CoreLib::pzh::pzh1::rand: " << CoreLib::pzh::pzh1::rand << endl;
        CoreLib::pzh::QueueInit();
    }

    // ---------------------------------------------------------
    // 2.2 缺省参数 (Default Arguments)
    // ---------------------------------------------------------
    // 规则：缺省值必须从右向左连续赋值。
    void Default_ArgsFull(int a = 10, int b = 20, int c = 30)
    {
        cout << "Default_ArgsFull: a=" << a << ", b=" << b << ", c=" << c << endl;
    }

    void Default_ArgsPartial(int a, int b = 10, int c = 20)
    {
        cout << "Default_ArgsPartial: a=" << a << ", b=" << b << ", c=" << c << endl;
    }

    void Func_Default_Arguments()
    {
        cout << "\n--- [Systematic] Default Arguments ---" << endl;
        Default_ArgsFull(); // 使用全部缺省值
        Default_ArgsFull(1); // a=1, b=20, c=30
        Default_ArgsPartial(100); // a=100 (强制传参), b=10, c=20
    }

    // ---------------------------------------------------------
    // 2.3 函数重载与名字修饰 (Overloading & Name Mangling)
    // ---------------------------------------------------------
    /**
     * [原理详解]
     * C++ 编译器通过 Name Mangling 技术将参数类型编码进函数符号中。
     * 例如 Add(int, int) 可能被修饰为 _Z3Addii，而 Add(double, double) 为 _Z3Adddd。
     * 这使得链接器能够区分同名函数。返回值类型不是重载的依据。
     */
    int Add(int left, int right)
    {
        cout << "Called: int Add(int, int)" << endl;
        return left + right;
    }

    double Add(double left, double right)
    {
        cout << "Called: double Add(double, double)" << endl;
        return left + right;
    }

    // 重载演示
    void func_overload(int a)
    {
        cout << "func_overload(int)" << endl;
    }

    void func_overload(int a, int b)
    {
        cout << "func_overload(int, int)" << endl;
    }

    void Func_Over_loading()
    {
        cout << "\n--- [Systematic] Function Overloading ---" << endl;
        Add(1, 2);
        Add(1.1, 2.2);
        func_overload(1);
        func_overload(1, 2);
    }

    // ---------------------------------------------------------
    // 2.4 引用与指针 (Reference vs Pointer)
    // ---------------------------------------------------------
    void swap_by_ref(int &x, int &y)
    {
        int tmp = x;
        x = y;
        y = tmp;
    }

    // C 风格：二级指针修改头结点
    void PushBack_C(ListNode **pphead, int x)
    {
        ListNode *newnode = (ListNode *) malloc(sizeof(ListNode));
        newnode->val = x;
        newnode->next = NULL;
        if (*pphead == NULL)
            *pphead = newnode;
        // 省略 else 遍历逻辑
    }

    // C++ 风格：指针的引用 (ListNode*&)
    // 语义上是指针的别名，操作该引用即直接操作原指针变量，无需解引用 (*)
    void PushBack_CPP(ListNode *&phead, int x)
    {
        ListNode *newnode = (ListNode *) malloc(sizeof(ListNode));
        newnode->val = x;
        newnode->next = nullptr;
        if (phead == nullptr)
            phead = newnode;
        // 省略 else 遍历逻辑
    }

    // 性能测试对象
    struct BigData
    {
        int a[10000];
    } big_data_instance;

    BigData TestReturnByValue()
    {
        return big_data_instance;
    } // 发生一次深拷贝
    BigData &TestReturnByRef()
    {
        return big_data_instance;
    } // 仅返回地址（别名）

    void Func_References()
    {
        cout << "\n--- [Systematic] References & Performance ---" << endl;
        int a2 = 10;
        int &ra2 = a2;

        // 验证：引用和原变量共享同一内存地址
        cout << "Addr a2: " << &a2 << ", Addr ra2: " << &ra2 << endl;

        ListNode *plist = nullptr;
        PushBack_CPP(plist, 1);
        cout << "List insert via ref. Val: " << plist->val << endl;
        free(plist);

        // 性能对比：值返回 vs 引用返回
        // 在大规模数值计算（如 FFT 或 多项式乘法）中，减少拷贝是优化的关键。
        size_t begin1 = clock();
        for (size_t i = 0; i < 100000; ++i)
            TestReturnByValue();
        size_t end1 = clock();

        size_t begin2 = clock();
        for (size_t i = 0; i < 100000; ++i)
            TestReturnByRef();
        size_t end2 = clock();

        cout << "Return by Value Time: " << end1 - begin1 << " ms" << endl;
        cout << "Return by Ref Time:   " << end2 - begin2 << " ms" << endl;
    }

    // ---------------------------------------------------------
    // 2.5 宏、内联与自动类型推导 (Macro, Inline, Auto)
    // ---------------------------------------------------------
    // 宏的风险：仅仅是文本替换，无类型检查，易受优先级影响。
#define ADD_MACRO(x, y) ((x) + (y))

    // 内联函数：编译期展开，有类型检查，是宏的安全替代品。
    inline int add_inline(int x, int y)
    {
        return x + y;
    }

    void Func_Inline_Auto()
    {
        cout << "\n--- [Systematic] Inline & Auto ---" << endl;

        // 优先级陷阱演示
        // ADD_MACRO(1|2, 1&2) -> ((1|2) + (1&2)) = 3 + 0 = 3 (正确)
        // 若定义为 x+y，则 1|2 + 1&2 -> 1 | (2+1) & 2 -> 1 | 3 & 2 -> 1 | 2 -> 3
        int res_macro = ADD_MACRO(1 | 2, 1 & 2);
        cout << "Macro result: " << res_macro << endl;

        // auto 类型推导
        int a = 0;
        auto b = a; // int
        auto c = &a; // int*
        cout << "Type b: " << typeid(b).name() << ", Type c: " << typeid(c).name() << endl;

        // 范围 for 循环 (Range-based for loop)
        int array[] = {1, 2, 3, 4, 5};
        // auto& e 表示引用原数组元素，可修改
        for (auto &e: array)
            e *= 2;

        cout << "Modified array: ";
        for (auto e: array)
            cout << e << " ";
        cout << endl;
    }

    // ---------------------------------------------------------
    // 2.6 算法整合：递归与指针 (Algorithm)
    // ---------------------------------------------------------
    int TreeSize(TreeNode *root)
    {
        if (root == nullptr)
            return 0;
        return TreeSize(root->left) + TreeSize(root->right) + 1;
    }

    // 使用引用 int& ri 维护全局下标，避免使用全局变量或复杂的指针传递
    void _preorderTraversal(TreeNode *root, int *a, int &ri)
    {
        if (root == nullptr)
            return;
        a[ri++] = root->val;
        _preorderTraversal(root->left, a, ri);
        _preorderTraversal(root->right, a, ri);
    }

    int *preorderTraversal(TreeNode *root, int &returnSize)
    {
        int size = TreeSize(root);
        if (size == 0)
        {
            returnSize = 0;
            return nullptr;
        }
        int *a = (int *) malloc(sizeof(int) * size);
        int i = 0;
        _preorderTraversal(root, a, i);
        returnSize = size;
        return a;
    }

    void Func_Algorithm()
    {
        cout << "\n--- [Systematic] Algorithm (Tree) ---" << endl;
        TreeNode root;
        root.val = 99;
        root.left = nullptr;
        root.right = nullptr;

        int retSize = 0;
        int *arr = preorderTraversal(&root, retSize);
        cout << "Preorder Root: " << arr[0] << ", Size: " << retSize << endl;
        free(arr); // 显式释放 malloc 申请的内存
    }

    // 模块统一执行入口
    void Execute()
    {
        Func_Namespace();
        Func_Default_Arguments();
        Func_Over_loading();
        Func_References();
        Func_Inline_Auto();
        Func_Algorithm();
    }
}

// ============================================================================
// [SECTION 3] 遗留交互测试 (Legacy Interactive Module)
// ============================================================================
namespace Module_Interactive_Optimized
{
    using std::cout;
    using std::endl;

    // ---------------------------------------------------------
    // 1. 数据结构定义的优化
    // ---------------------------------------------------------
    // 使用 struct 构造函数简化节点初始化，确保原子性
    struct Node
    {
        int val;
        Node *next;

        // 构造函数：确保节点创建即初始化，杜绝脏数据
        Node(int v) : val(v), next(nullptr)
        {}
    };

    // ---------------------------------------------------------
    // 2. 算法逻辑的严谨实现
    // ---------------------------------------------------------

    /**
     * @brief 链表尾插法 (标准 O(N) 实现)
     * @param phead 链表头指针的引用 (Node*&)，允许修改外部头指针
     * @param x 待插入的数值
     * @note
     * 1. 若链表为空，直接修改头指针。
     * 2. 若链表非空，遍历至尾节点 (tail)，将其 next 指向新节点。
     */
    void PushBack(Node *&phead, int x)
    {
        // [内存管理] 使用 new 替代 malloc，自动调用构造函数
        Node *newNode = new Node(x);

        if (phead == nullptr)
        {
            phead = newNode; // 边界条件：空链表处理
        }
        else
        {
            // 通用情况：寻找尾节点
            Node *tail = phead;
            while (tail->next != nullptr)
            {
                tail = tail->next;
            }
            // 链接新节点
            tail->next = newNode;
        }
    }

    /**
     * @brief 链表销毁函数 (内存回收)
     * @details 最佳实践要求：谁分配 (new)，谁释放 (delete)。
     * 必须遍历释放每个节点，防止内存泄漏。
     */
    void DestroyList(Node *&phead)
    {
        Node *current = phead;
        while (current != nullptr)
        {
            Node *nextNode = current->next;
            delete current; // 释放当前节点内存
            current = nextNode;
        }
        phead = nullptr; //以此防止悬空指针 (Dangling Pointer)
    }

    /**
     * @brief 链表打印辅助函数
     */
    void PrintList(const Node *phead)
    {
        const Node *cur = phead;
        cout << "List: ";
        while (cur != nullptr)
        {
            cout << cur->val << " -> ";
            cur = cur->next;
        }
        cout << "NULL" << endl;
    }

    // ---------------------------------------------------------
    // 3. 安全的函数替代方案
    // ---------------------------------------------------------

    // 使用内联模板函数替代 #define 宏
    // 优势：具备类型检查 (Type Safety) 和调试能力，同时保持内联的高效性。
    template<typename T>
    inline T SafeAdd(T x, T y)
    {
        return x + y;
    }

    // 引用交换函数 (保持原样，这已经是 C++ 的标准写法)
    void Swap(int &x, int &y)
    {
        int tmp = x;
        x = y;
        y = tmp;
    }

    // ---------------------------------------------------------
    // 4. 执行入口
    // ---------------------------------------------------------
    void Execute()
    {
        cout << "\n===== EXECUTE OPTIMIZED INTERACTIVE TESTS =====" << endl;

        // --- 链表逻辑验证 ---
        cout << "[Test 1] Linked List Correctness Verification" << endl;
        Node *plist = nullptr;

        // 插入测试
        PushBack(plist, 1); // Case: 空链表插入
        PushBack(plist, 2); // Case: 非空链表插入 (原 BUG 发生处)
        PushBack(plist, 3); // Case: 连续插入

        // 验证输出
        PrintList(plist); // 预期输出: 1 -> 2 -> 3 -> NULL

        // 资源清理 (至关重要)
        DestroyList(plist);
        if (plist == nullptr)
        {
            cout << "Memory successfully released." << endl;
        }

        // --- 模板函数与引用测试 ---
        cout << "\n[Test 2] Safe Functions & References" << endl;
        int i = 0, j = 1;
        Swap(i, j);
        cout << "Swap Result: i=" << i << ", j=" << j << endl;

        // 宏替代方案测试
        // 自动推导为 int
        cout << "SafeAdd(1, 2) = " << SafeAdd(1, 2) << endl;
        // 自动推导为 double，且处理了优先级问题
        cout << "SafeAdd(1.1, 2.2) = " << SafeAdd(1.1, 2.2) << endl;

        // 复杂表达式测试 (宏定义的痛点)
        // SafeAdd(1|2, 1&2) -> SafeAdd(3, 0) -> 3. 逻辑清晰，无优先级歧义。
        cout << "SafeAdd(Bitwise) = " << SafeAdd(1 | 2, 1 & 2) << endl;
    }
}

// ============================================================================
// [MAIN ENTRY] 程序主入口
// ============================================================================
int main()
{
    // 1. 系统化机制验证
    Module_Systematic::Execute();

    // 2. 语言机制测试
    Module_Systematic::Execute();

    // 3. 遗留交互测试
    Module_Interactive_Optimized::Execute();

    cout << "\nExecution Terminated." << endl;
    return 0;
};