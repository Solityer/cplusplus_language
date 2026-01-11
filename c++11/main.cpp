#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <map>
#include <array>  // 固定大小数组（C++11）
#include <algorithm>   // 算法库（如sort）
#include <functional>  // 函数对象包装器（function, bind）
#include <cassert>   // 断言库
#include <cstring>   // C字符串操作（strlen, strcpy）
#include <typeinfo>   // 类型信息（typeid）
using namespace std;

// 一个简单的点结构体，用于演示列表初始化
struct Point
{
    int _x;
    int _y;
};

// 日期类，用于演示构造函数的列表初始化
class Date
{
public:
    // 构造函数：打印调用信息，方便观察
    Date(int year, int month, int day)
        : _year(year)
        , _month(month)
        , _day(day)
    {
        cout << "Date(int year, int month, int day)" << endl;
    }

private:
    int _year;
    int _month;
    int _day;
};

namespace pzh
{
    // 自定义string类，用于演示深拷贝与移动语义的区别
    class string
    {
    public:
        typedef char *iterator; // 迭代器定义为 char* 指针

        // 返回指向字符串开头的迭代器
        iterator begin()
        {
            return _str;
        }
        // 返回指向字符串结尾（\0位置）的迭代器
        iterator end()
        {
            return _str + _size;
        }

        // 构造函数：支持默认参数，进行深拷贝
        string(const char *str = "")
            : _size(strlen(str)) // 计算字符串长度
            , _capacity(_size) // 初始容量等于长度
        {
            cout << "pzh::string(char* str) -- 构造" << endl;
            _str = new char[_capacity + 1]; // 分配内存（多一个字节存\0）
            strcpy(_str, str); // 拷贝数据
        }

        // 交换两个 string 对象的成员变量，只交换指针和整数，不涉及内存拷贝
        void swap(string &s)
        {
            // 前面没有写任何名称时，它代表的是全局作用域，主要原因是为了避免命名冲突和递归死循环
            /* 加上 :: 前缀后，就是在告诉编译器：
             * 不要在当前的 pzh 命名空间或 string 类里找，去全局空间找那个通用的 swap 函数
             * 由于开头写了 using namespace std; 全局空间此时可以找到标准库提供的std::swap）*/
            ::swap(_str, s._str); // 交换指针
            ::swap(_size, s._size); // 交换大小
            ::swap(_capacity, s._capacity); // 交换容量
        }

        // 拷贝构造函数 (C++98 风格)：深拷贝
        // 当使用 const string& (左值) 初始化新对象时调用
        string(const string &s)
        {
            cout << "pzh::string(const string& s) -- 深拷贝" << endl;
            _str = new char[s._capacity + 1]; // 重新分配新内存
            strcpy(_str, s._str); // 复制内容
            _size = s._size;
            _capacity = s._capacity;
        }

        // 移动构造函数 (C++11 核心特性)
        // 当使用 string&& (右值/临时对象) 初始化新对象时调用
        // 这里的 s 是一个将亡值，我们直接"窃取"它的资源，而不是拷贝
        string(string &&s)
        {
            cout << "pzh::string(string&& s) -- 移动构造" << endl;
            // 直接交换资源，让 s 指向空，当前对象接管 s 的堆内存
            swap(s);
        }

        // 赋值运算符重载 (C++98 风格)：深拷贝
        // s1 = s2; s2 是左值时调用
        string &operator=(const string &s)
        {
            cout << "pzh::string& operator=(const string& s) -- 深拷贝" << endl;
            if (this != &s) // 防止自我赋值
            {
                char *tmp = new char[s._capacity + 1]; // 开辟新空间
                strcpy(tmp, s._str); // 拷贝数据
                delete[] _str; // 释放旧空间
                _str = tmp; // 指向新空间
                _size = s._size;
                _capacity = s._capacity;
            }
            return *this;
        }

        // 移动赋值运算符 (C++11 核心特性)
        // s1 = move(s2); s2 变为右值时调用
        string &operator=(string &&s)
        {
            cout << "pzh::string& operator=(string&& s)-- 移动赋值" << endl;
            // 直接交换资源，不仅高效，而且避免了内存分配
            swap(s);
            return *this;
        }

        // 析构函数
        ~string()
        {
            delete[] _str; // 释放堆内存
            _str = nullptr;
        }

        // 下标访问操作符
        char &operator[](size_t pos)
        {
            assert(pos < _size); // 越界检查
            return _str[pos];
        }

        // 扩容函数
        void reserve(size_t n)
        {
            if (n > _capacity) // 如果请求空间大于当前容量
            {
                char *tmp = new char[n + 1]; // 开辟更大的新空间
                strcpy(tmp, _str); // 拷贝旧数据
                delete[] _str; // 释放旧空间
                _str = tmp; // 更新指针
                _capacity = n; // 更新容量
            }
        }

        // 尾插字符
        void push_back(char ch)
        {
            if (_size >= _capacity) // 如果满了，需要扩容
            {
                // 首次给4，之后翻倍
                size_t newcapacity = _capacity == 0 ? 4 : _capacity * 2;
                reserve(newcapacity);
            }
            _str[_size] = ch; // 插入字符
            ++_size; // 增加长度
            _str[_size] = '\0'; // 补上字符串结束符
        }

        // += 操作符重载
        string &operator+=(char ch)
        {
            push_back(ch);
            return *this;
        }

        // 获取 C 风格字符串
        const char *c_str() const
        {
            return _str;
        }

    private:
        char *_str = nullptr; // 字符串指针
        size_t _size = 0; // 有效字符个数
        size_t _capacity = 0; // 容量
    };

    // 辅助函数：将整数转换为 pzh::string
    // 用于演示函数返回对象时的移动语义/RVO优化
    pzh::string to_string(int x)
    {
        pzh::string ret;
        while (x)
        {
            int val = x % 10;
            x /= 10;
            ret += ('0' + val); // 将数字转字符追加
        }
        std::reverse(ret.begin(), ret.end()); // 翻转字符串
        return ret; // 返回局部对象
        // 在C++11中，这里如果不触发RVO优化，会优先调用移动构造，而不是拷贝构造
    }

    // --- pzh::list 实现 (双向链表) ---
    // 模板类 T：链表存储的数据类型
    template<class T>
    struct list_node
    {
        T _data; // 数据域
        list_node<T> *_next;
        list_node<T> *_prev;

        // 构造函数：接受 const T& (左值)，进行拷贝构造
        list_node(const T &x = T())
            : _data(x)
            , _next(nullptr)
            , _prev(nullptr)
        {}

        // 构造函数：接受 T&& (右值)，进行移动构造
        // 针对 push_back(move(obj)) 这种场景
        list_node(T &&x)
            : _data(move(x))
            , _next(nullptr)
            , _prev(nullptr)
        {}

        // 构造函数：可变参数模板 (Variadic Templates)
        // 针对 emplace_back，直接用参数包构造 _data，避免临时对象
        template<class... Args>
        list_node(Args &&... args)
        // std::forward<Args>(args)... 是完美转发，保持参数的原有属性（左值/右值）
            : _data(std::forward<Args>(args)...)
            , _next(nullptr)
            , _prev(nullptr)
        {}
    };

    // 链表迭代器封装
    // Ref: T& 或 const T&
    // Ptr: T* 或 const T*
    template<class T, class Ref, class Ptr>
    struct __list_iterator
    {
        typedef list_node<T> Node;
        typedef __list_iterator<T, Ref, Ptr> self; // 自身类型别名
        Node *_node; // 迭代器本质就是对节点指针的封装

        // 构造函数
        __list_iterator(Node *node)
            : _node(node)
        {}

        // 前置++ (++it)
        self &operator++()
        {
            _node = _node->_next; // 指向下一个节点
            return *this;
        }

        // 前置-- (--it)
        self &operator--()
        {
            _node = _node->_prev; // 指向前一个节点
            return *this;
        }

        // 后置++ (it++)
        self operator++(int)
        {
            self tmp(*this); // 记录当前值
            _node = _node->_next; // 移动
            return tmp; // 返回旧值
        }

        // 解引用操作符 *it，返回数据的引用
        Ref operator*()
        {
            return _node->_data;
        }

        // 箭头操作符 it->，返回数据的地址
        Ptr operator->()
        {
            return &_node->_data;
        }

        // 比较操作符 !=
        bool operator!=(const self &s)
        {
            return _node != s._node;
        }
        // 比较操作符 ==
        bool operator==(const self &s)
        {
            return _node == s._node;
        }
    };

    // 链表类
    template<class T>
    class list
    {
        typedef list_node<T> Node; // 节点类型别名
    public:
        // 普通迭代器和const迭代器定义
        typedef __list_iterator<T, T &, T *> iterator;
        typedef __list_iterator<T, const T &, const T *> const_iterator;

        // begin() 返回头节点的下一个节点（第一个有效节点）
        const_iterator begin() const { return const_iterator(_head->_next); }
        const_iterator end() const { return const_iterator(_head); } // end() 返回哨兵位头节点
        iterator begin() { return iterator(_head->_next); }
        iterator end() { return iterator(_head); }

        // 初始化空链表（带头双向循环链表）
        void empty_init() {
            _head = new Node; // 创建哨兵位头节点
            _head->_next = _head; // next指向自己
            _head->_prev = _head; // prev指向自己
            _size = 0;
        }

        // 默认构造函数
        list()
        {
            empty_init();
        }

        // 拷贝构造函数：利用已有接口深拷贝
        list(const list<T> &lt)
        {
            empty_init();
            for (auto e: lt) push_back(e); // 遍历源链表，逐个尾插
        }

        // 交换两个链表的头指针和大小
        void swap(list<T> &lt)
        {
            std::swap(_head, lt._head);
            std::swap(_size, lt._size);
        }

        // 赋值运算符重载（现代写法）
        // 参数 lt 是传值传参，会调用拷贝构造生成副本
        // 然后将当前对象与副本交换，函数结束副本自动销毁
        list<T> &operator=(list<T> lt)
        {
            swap(lt);
            return *this;
        }

        // 析构函数
        ~list()
        {
            clear(); // 清除所有有效节点
            delete _head; // 释放哨兵位头节点
            _head = nullptr;
        }

        // 清空链表
        void clear()
        {
            iterator it = begin();
            while (it != end()) it = erase(it); // 遍历删除
        }

        // 尾插（左值版本）：调用 insert
        void push_back(const T &x)
        {
            insert(end(), x);
        }

        // 尾插（右值版本）：C++11 新增
        // x 是右值引用，std::forward 保持其右值属性传递给 insert
        void push_back(T &&x)
        {
            insert(end(), std::forward<T>(x));
        }

        // C++11 核心：Emplace Back (原地构造)
        // 接收可变参数包 Args...，直接在节点内存中构造对象 T
        // 避免了先构造临时对象再拷贝/移动的过程
        template<class... Args>
        void emplace_back(Args &&... args)
        {
            // 直接传递参数包给 Node 构造函数，Node 内部再传给 T 的构造函数
            Node *newnode = new Node(std::forward<Args>(args)...);
            // 手动链接节点（相当于在 end() 位置插入）
            Node *prev = _head->_prev;
            Node *cur = _head;
            prev->_next = newnode;
            newnode->_prev = prev;
            newnode->_next = cur;
            cur->_prev = newnode;
            ++_size;
        }

        // 在 pos 位置前插入 x (左值版本)
        iterator insert(iterator pos, const T &x)
        {
            Node *cur = pos._node;
            Node *newnode = new Node(x); // 调用 T 的拷贝构造
            // 链接逻辑
            Node *prev = cur->_prev;
            prev->_next = newnode;
            newnode->_prev = prev;
            newnode->_next = cur;
            cur->_prev = newnode;
            ++_size;
            return iterator(newnode);
        }

        // 在 pos 位置前插入 x (右值版本)
        iterator insert(iterator pos, T &&x)
        {
            Node *cur = pos._node;
            // 调用 Node(T&&)，Node 内部再调用 T(T&&) 移动构造
            // 这里的 forward 是为了把 x (作为右值引用传入，本身是左值) 转回右值
            Node *newnode = new Node(std::forward<T>(x));
            // 链接逻辑
            Node *prev = cur->_prev;
            prev->_next = newnode;
            newnode->_prev = prev;
            newnode->_next = cur;
            cur->_prev = newnode;
            ++_size;
            return iterator(newnode);
        }

        // 删除 pos 位置的节点
        iterator erase(iterator pos)
        {
            Node *cur = pos._node;
            Node *prev = cur->_prev;
            Node *next = cur->_next;
            delete cur; // 释放节点内存
            // 重新链接
            prev->_next = next;
            next->_prev = prev;
            --_size;
            return iterator(next); // 返回被删除节点的下一个位置
        }

    private:
        Node *_head; // 头节点
        size_t _size; // 链表大小
    };
}

// 4个重载函数，用于检测参数类型
void Fun(int &x)
{
    cout << "左值引用" << endl;
}
void Fun(const int &x)
{
    cout << "const 左值引用" << endl;
}
void Fun(int &&x)
{
    cout << "右值引用" << endl;
}
void Fun(const int &&x)
{
    cout << "const 右值引用" << endl;
}

// 万能引用 (Forwarding Reference) 模板：T&&
// 当 T&& 用于模板参数时，它既能接收左值也能接收右值
template<typename T>
void PerfectForward(T &&t)
{
    // t 在这里作为一个函数参数，本身永远是左值（因为它有名字）
    // std::forward<T>(t) 会根据 T 的类型，还原 t 最初传入时的属性（左值或右值）
    // 如果没有 std::forward，t 传给 Fun 永远会调用左值版本
    Fun(std::forward<T>(t));
}

// ==========================================
// 演示 default 和 delete 关键字
// ==========================================
class Person
{
public:
    Person(const char *name = "", int age = 0)
        : _name(name)
        , _age(age)
    {}

    // C++11 强制编译器生成默认的移动构造函数
    Person(Person &&p) = default;

    // C++11 强制编译器生成默认的拷贝构造函数
    Person(const Person &p) = default;

    // 如果想禁止拷贝，可以使用 delete 关键字：
    // Person(const Person& p) = delete;
    ~Person() { cout << "~Person()" << endl; }

private:
    pzh::string _name; // 使用自定义的 string
    int _age;
};

// ==========================================
// 可变参数模板 (Variadic Templates) 演示
// ==========================================

// 递归终止函数：当参数包为空时调用
void _ShowList_Recursive()
{
    cout << endl;
}

// 递归展开函数
// T 是第一个参数的类型，Args... 是剩余参数包
template<class T, class... Args>
void _ShowList_Recursive(const T &val, Args... args)
{
    cout << val << " "; // 处理第一个参数
    _ShowList_Recursive(args...); // 递归调用，处理剩余参数
}

// 供外部调用的接口
template<class... Args>
void ShowList_Recursive(Args... args)
{
    _ShowList_Recursive(args...);
}

// --- 数组展开方式 (C++11 高级玩法) ---
// 辅助函数：打印单个参数并返回0
template<class T>
int PrintArg(T &&t)
{
    cout << t << " ";
    return 0;
}

// 使用初始化列表展开参数包
template<class... Args>
void ShowList_Expand(Args &&... args)
{
    // 逻辑解释：
    // 1. { PrintArg(args)... } 会被编译器展开为 { PrintArg(arg1), PrintArg(arg2), ... }
    // 2. 初始化数组需要计算每个元素的值，因此会依次调用 PrintArg
    // 3. 数组 arr 仅仅是为了触发这个求值过程，本身无其他用途
    int arr[] = {PrintArg(args)...};
    cout << endl;
}

// ==========================================
// Lambda 表达式与排序相关结构体
// ==========================================
struct Goods {
    string _name; // 商品名字
    double _price; // 价格
    int _evaluate; // 评价

    Goods(const char *str, double price, int evaluate)
        : _name(str), _price(price), _evaluate(evaluate) {
    }
};

// 仿函数：价格升序
struct ComparePriceLess
{
    bool operator()(const Goods &gl, const Goods &gr) { return gl._price < gr._price; }
};

// 仿函数：价格降序
struct ComparePriceGreater
{
    bool operator()(const Goods &gl, const Goods &gr) { return gl._price > gr._price; }
};

// ==========================================
// 包装器 (function) 相关
// ==========================================

// 普通函数
void swap_func(int &r1, int &r2)
{
    int tmp = r1;
    r1 = r2;
    r2 = tmp;
}

// 仿函数类
struct Swap
{
    void operator()(int &r1, int &r2)
    {
        int tmp = r1;
        r1 = r2;
        r2 = tmp;
    }
};

// 用于 bind 演示的减法函数
int Sub(int a, int b)
{
    return a - b;
}

// 用于 bind 成员函数的类
class Plus
{
public:
    // 静态成员函数
    static int plusi(int a, int b)
    {
        return a + b;
    }
    // 非静态成员函数
    double plusd(double a, double b)
    {
        return a + b;
    }
};


int main()
{
    // ==========================================
    // 1. 列表初始化 (List Initialization)
    // ==========================================
    cout << "========== 1. 列表初始化 (List Initialization) ==========" << endl;
    {
        // C++98 风格
        int array1[] = {1, 2, 3, 4, 5};
        int array2[5] = {0};
        Point p = {1, 2};

        // C++11 新特性：一切皆可列表初始化，且可以省略赋值号 =
        int i = 0;
        int j = {0}; // 列表初始化
        int k{0}; // 省略等号的列表初始化

        // 数组和结构体的省略等号初始化
        int array3[]{1, 2, 3, 4, 5};
        int array4[5]{0};
        Point p2{1, 2};

        // 对象的列表初始化
        Date d1(2026, 1, 8); // 传统构造

        // 这里的逻辑：
        // {2026, 11, 8} 本质上是一个初始化列表
        // 编译器会尝试匹配 Date 的构造函数。
        // C++98是先构造临时对象再拷贝，C++11优化为直接构造
        Date d2 = {2023, 11, 25};
        Date d3{2023, 11, 25}; // 推荐写法

        // 标准容器支持列表初始化（底层原理是 initializer_list 构造函数）
        vector<int> v1 = {1, 2, 3, 4};
        vector<int> v2 = {1, 2, 3, 4, 5, 6};
        v1 = {10, 20, 30}; // 调用赋值运算符重载，参数是 initializer_list
        list<int> lt = {10, 20, 30};

        // initializer_list 自动推导
        auto il1 = {10, 20, 30, 40, 50}; // il1 类型是 initializer_list<int>
        cout << "Type of il1: " << typeid(il1).name() << endl; // 打印类型名

        // 手动定义 initializer_list 并遍历
        initializer_list<int> il2 = {10, 20, 30};
        for (auto e: il2) { cout << e << " "; } // 范围for循环
        cout << endl;

        // map 的列表初始化，内部是 pair 的列表
        map<string, string> dict = {{"insert", "插入"}, {"get", "获取"}};
        for (auto &kv: dict)
        {
            cout << kv.first << ":" << kv.second << endl;
        }
    }

    // ==========================================
    // 2. auto 和 decltype
    // ==========================================
    cout << "\n========== 2. auto 和 decltype ==========" << endl;
    {
        int i = 1;
        double d = 2.2;

        // RTTI (运行时类型识别)
        cout << typeid(i).name() << endl; // int
        cout << typeid(d).name() << endl; // double

        auto j = i; // auto: 编译器根据右边推导左边类型 (int)

        auto ret = i * d; // ret 推导为 double

        // decltype: 获取表达式的类型，但不计算表达式的值
        // x 的类型被定义为 ret 的类型 (double)
        decltype(ret) x;

        // 用 decltype 推导的类型来实例化 vector 模板
        vector<decltype(ret)> v;
        v.push_back(1);
        v.push_back(1.1);
        for (auto e: v)
        {
            cout << e << " ";
        }
        cout << endl;
    }

    // ==========================================
    // 3. array 容器 (vs vector)
    // ==========================================
    cout << "\n========== 3. array 容器 (vs vector) ==========" << endl;
    {
        // array 是 C++11 提供的固定大小数组，封装了原生数组
        // 存储在栈上（如果对象在栈上），大小为 sizeof(int) * 10
        array<int, 10> a1;
        cout << "sizeof(array): " << sizeof(a1) << endl;

        // vector 是动态数组，存储在堆上
        // sizeof(v) 只包含 vector 类的控制结构（指针等），通常是 12 或 24 字节
        vector<int> v(10, 0);
        cout << "sizeof(vector): " << sizeof(v) << endl;
    }

    // ==========================================
    // 4. 左值引用与右值引用基础
    // ==========================================
    cout << "\n========== 4. 左值引用与右值引用基础 ==========" << endl;
    {
        int i = 0;
        int j = i;

        // 左值：可以取地址的、有名字的变量（如 i, j）
        // 右值：不能取地址的、临时的值（如 字面量10, 表达式返回值i+j）
        // 左值引用承诺：我引用的是一个长期存在的对象。而右值通常马上就没了

        // 左值引用：只能引用左值
        // int& r = 10; // 错误：不能用左值引用绑定右值

        // const 左值引用：具有“万能”特性，既能引用左值也能引用右值
        const int &r1 = 10; // 引用字面量
        const int &r2 = i + j; // 引用临时计算结果

        // 右值引用 (C++11)：只能引用右值
        int &&rr1 = 10;
        int &&rr2 = i + j;

        // std::move：将一个左值强制转换为右值引用
        // 意味着承诺随后不再使用该左值，允许窃取其资源
        int &&rr3 = move(i);
    }

    // ==========================================
    // 5. 移动语义 (pzh::string)
    // ==========================================
    cout << "\n========== 5. 移动语义 (pzh::string) ==========" << endl;
    {
        pzh::string s1("hello world"); // 调用构造函数

        cout << "--- 拷贝构造 ---" << endl;
        pzh::string s2 = s1; // s1 是左值，调用拷贝构造（深拷贝）

        cout << "--- 移动构造 (std::move) ---" << endl;
        // s1 被 move 强转为右值，调用移动构造
        // s1 的资源（字符数组指针）转移给了s3，s1变为空
        pzh::string s3 = std::move(s1);

        cout << "--- to_string 返回值优化演示 ---" << endl;
        // pzh::to_string 返回局部的 string 对象。
        // 在 C++11 之前，这会发生拷贝构造。
        // 在 C++11，编译器会优先尝试 RVO (返回值优化)。
        // 如果不优化，返回的是一个“将亡值”，会调用移动构造，成本极低。
        pzh::string ret = pzh::to_string(1234);
    }

    // ==========================================
    // 6. 完美转发 (Perfect Forwarding)
    // ==========================================
    cout << "\n========== 6. 完美转发 (Perfect Forwarding) ==========" << endl;
    {
        // PerfectForward 接受万能引用 T&&

        PerfectForward(10);
        // 10 是右值 -> T 推导为 int -> 参数类型 int&&
        // forward<int> 保持右值属性 -> 调用 Fun(int&&)

        int a;
        PerfectForward(a);
        // a 是左值 -> T 推导为 int& (引用折叠) -> 参数类型 int&
        // forward<int&> 保持左值属性 -> 调用 Fun(int&)

        PerfectForward(std::move(a));
        // move(a) 是右值 -> T 推导为 int -> 参数类型 int&&
        // 调用 Fun(int&&)

        const int b = 8;
        PerfectForward(b);
        // b 是 const 左值 -> T 推导为 const int& -> 调用 Fun(const int&)

        PerfectForward(std::move(b));
        // move(b) 是 const 右值 -> T 推导为 const int -> 调用 Fun(const int&&)
    }

    // ==========================================
    // 7. 默认成员函数控制 (default/delete)
    // ==========================================
    cout << "\n========== 7. 默认成员函数控制 (default/delete) ==========" << endl;
    {
        Person s1;
        // Person 类中显式使用了 default，让编译器生成默认版本
        Person s2 = s1; // 调用默认生成的拷贝构造
        Person s3 = std::move(s1); // 调用默认生成的移动构造
    }

    // ==========================================
    // 8. 可变参数模板 (Variadic Templates)
    // ==========================================
    cout << "\n========== 8. 可变参数模板 (Variadic Templates) ==========" << endl;
    {
        cout << "--- 递归方式展开 ---" << endl;
        // 依次打印所有参数
        ShowList_Recursive(1);
        ShowList_Recursive(1, 2);
        ShowList_Recursive(1, 2.2, 'x', "recursive");

        cout << "--- 逗号表达式/数组方式展开 ---" << endl;
        // 使用初始化列表技巧展开参数包，无需递归
        ShowList_Expand(1);
        ShowList_Expand(1, 'A');
        ShowList_Expand(1, 'A', std::string("sort"));
    }

    // ==========================================
    // 9. emplace_back vs push_back
    // ==========================================
    cout << "\n========== 9. emplace_back vs push_back ==========" << endl;
    {
        // 使用我们自己实现的 pzh::list 观察构造过程
        pzh::list<pzh::string> lt;
        pzh::string s1("1111");

        cout << "--- push_back(lvalue) ---" << endl;
        lt.push_back(s1); // 参数是左值，调用拷贝构造（深拷贝）

        cout << "--- push_back(move(rvalue)) ---" << endl;
        lt.push_back(move(s1)); // 参数是右值，调用移动构造（资源转移）

        cout << "--- emplace_back(args...) ---" << endl;
        // emplace_back 直接接收构造 string 所需的参数 "2222"
        // 并在 list 节点内部直接构造对象。
        // 相比 push_back("2222")，少了一次临时对象的构造和移动/拷贝。
        lt.emplace_back("2222");

        cout << "--- emplace_back pair 演示 ---" << endl;
        pzh::list<pair<pzh::string, int> > lt_pair;

        // push_back 需要先构造一个 pair 临时对象，再拷贝/移动进去
        lt_pair.push_back(make_pair("1111", 1));

        // emplace_back 直接传参数给 pair 的构造函数，在节点内原地构造 pair
        lt_pair.emplace_back("2222", 2);
    }

    // ==========================================
    // 10. Lambda 表达式
    // ==========================================
    cout << "\n========== 10. Lambda 表达式 ==========" << endl;
    {
        vector<Goods> v = {{"苹果", 2.1, 5}, {"香蕉", 3, 4},
            {"橙子", 2.2, 3}, {"菠萝", 1.5, 4}};

        // Lambda 语法: [](args) { body }
        // 这是一个匿名函数对象，用于替代简单的仿函数
        sort(v.begin(), v.end(), [](const Goods &g1, const Goods &g2) {
            return g1._price < g2._price; // 价格升序
        });

        sort(v.begin(), v.end(), [](const Goods &g1, const Goods &g2) {
            return g1._evaluate > g2._evaluate; // 评价降序
        });

        // 捕捉列表演示
        int x = 0, y = 1;

        // [=]：传值捕捉所有外部变量
        // mutable：默认 lambda 是 const 的，不允许修改捕捉的副本，mutable 取消此限制
        auto f2 = [x, y]() mutable
        {
            // 这里修改的是 x 和 y 的拷贝，不影响外部的 x, y
            int tmp = x;
            x = y;
            y = tmp;
            cout << "Inside lambda(mutable): " << x << " " << y << endl;
        };
        f2();
        cout << "Outside lambda: " << x << " " << y << " (外部未改变)" << endl;

        // [&]：引用捕捉所有外部变量
        auto f3 = [&x, &y]()
        {
            // 这里修改的是外部的 x 和 y 本身
            int tmp = x;
            x = y;
            y = tmp;
        };
        f3();
        cout << "Outside lambda(ref): " << x << " " << y << " (外部已改变)" << endl;
    }

    // ==========================================
    // 11. 包装器 (function & bind)
    // ==========================================
    cout << "\n========== 11. 包装器 (function & bind) ==========" << endl;
    {
        int x = 10, y = 20;

        // Lambda 也是一个可调用对象
        auto swaplambda = [](int &r1, int &r2)
        {
            int tmp = r1;
            r1 = r2;
            r2 = tmp;
        };

        // std::function 是一个通用的多态函数包装器
        // 它可以存储、复制和调用任何可调用对象（函数、lambda、bind表达式、其他函数对象）

        // 包装 普通函数指针
        function<void(int &, int &)> f1 = swap_func;
        // 包装 仿函数对象
        function<void(int &, int &)> f2 = Swap();
        // 包装 Lambda 表达式
        function<void(int &, int &)> f3 = swaplambda;

        // 在 map 中统一存储不同的可调用对象，构建命令分发表
        map<string, function<void(int &, int &)> > cmdOP =
        {
            {"函数指针", swap_func},
            {"仿函数", Swap()},
            {"lambda", swaplambda},
        };

        // 调用 map 中存储的 lambda
        cmdOP["lambda"](x, y);
        cout << "After swap: " << x << " " << y << endl;

        // std::bind：函数参数绑定器
        // 它可以固定部分参数，或者调整参数顺序，生成一个新的可调用对象

        // 调整参数顺序：placeholders::_2 变为了新函数的第1个参数
        // 调用 f_sub(10, 5) 相当于调用 Sub(5, 10) -> 5 - 10 = -5
        function<int(int, int)> f_sub = bind(Sub, placeholders::_2, placeholders::_1);
        cout << "bind(Sub, _2, _1) calling (10, 5) -> 5 - 10 = " << f_sub(10, 5) << endl;

        // 固定参数：将 Sub 的第一个参数固定为 20
        // 新函数 f_sub_fixed 只需要一个参数
        // 调用 f_sub_fixed(5) 相当于调用 Sub(20, 5) -> 20 - 5 = 15
        function<int(int)> f_sub_fixed = bind(Sub, 20, placeholders::_1);
        cout << "bind(Sub, 20, _1) calling (5) -> 20 - 5 = " << f_sub_fixed(5) << endl;

        // 绑定成员函数
        // 成员函数需要对象指针才能调用，bind 可以帮我们把对象绑定进去
        // 这样 f_member 就变成了一个普通的函数对象，不需要显式传对象了
        function<double(double, double)> f_member = bind(&Plus::plusd, Plus(), placeholders::_1, placeholders::_2);
        cout << "bind member function: " << f_member(1.1, 2.2) << endl;
    }

    return 0;
}
