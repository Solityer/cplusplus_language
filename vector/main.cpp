#include "vector.h"

using std::cout;
using std::endl;
using std::string;

// 辅助打印函数，支持所有支持流插入运算符的类型
template<typename T>
void print_vector(const pzh::vector<T>& v, const string& msg = "")
{
    if (!msg.empty()) 
        cout << "[" << msg << "] ";
    for (const auto& e : v)
    {
        cout << e << " ";
    }
    cout << endl;
}

// 模块一：基础构造与遍历测试
void Test_Construction_And_Traversal()
{
    cout << "=== Test 1: Construction & Traversal ===" << endl;
    
    // 1. 默认构造 + push_back
    pzh::vector<int> v1;
    v1.push_back(1);
    v1.push_back(2);
    v1.push_back(3);
    print_vector(v1, "Default Construct");

    // 2. 迭代器区间构造
    pzh::vector<int> v2(v1.begin(), v1.end());
    print_vector(v2, "Range Construct");

    // 3. n个val构造
    pzh::vector<int> v3(5, 10);
    print_vector(v3, "Fill Construct");

    // 4. 拷贝构造
    pzh::vector<int> v4(v3);
    print_vector(v4, "Copy Construct");

    // 5. 赋值重载
    v1 = v3;
    print_vector(v1, "Assignment Operator");
}

// 模块二：容量管理与扩容测试
void Test_Capacity_And_Memory()
{
    cout << "\n=== Test 2: Capacity & Memory Management ===" << endl;
    
    pzh::vector<int> v;
    v.reserve(10);
    cout << "After reserve(10): Size=" << v.size() << ", Capacity=" << v.capacity() << endl;

    v.resize(5, 1);
    print_vector(v, "After resize(5, 1)");
    cout << "Size=" << v.size() << ", Capacity=" << v.capacity() << endl;

    v.resize(15, 2);
    print_vector(v, "After resize(15, 2)");
    cout << "Size=" << v.size() << ", Capacity=" << v.capacity() << endl;
    
    // 验证扩容时的自动增长
    size_t old_cap = v.capacity();
    cout << "Growing vector..." << endl;
    for(int i = 0; i < 20; ++i)
    {
        v.push_back(i);
        if(v.capacity() != old_cap)
        {
            cout << "Capacity changed from " << old_cap << " to " << v.capacity() << endl;
            old_cap = v.capacity();
        }
    }
}

// 模块三：修改器与迭代器失效测试
void Test_Modifiers_And_IteratorInvalidation()
{
    cout << "\n=== Test 3: Modifiers & Iterator Invalidation ===" << endl;
    
    pzh::vector<int> v;
    for(int i = 1; i <= 6; ++i) 
        v.push_back(i);
    print_vector(v, "Original");

    // 测试 insert (扩容可能导致迭代器失效，vector.h中已做pos重置处理)
    auto it_pos = std::find(v.begin(), v.end(), 3);
    if (it_pos != v.end())
    {
        // 在3前面插入30
        v.insert(it_pos, 30);
    }
    print_vector(v, "After insert 30 before 3");

    // 测试 erase (删除偶数)
    // 演示标准的 erase 迭代器更新写法
    auto it = v.begin();
    while (it != v.end())
    {
        if (*it % 2 == 0)
        {
            it = v.erase(it); // erase返回删除元素的下一个位置
        }
        else
        {
            ++it;
        }
    }
    print_vector(v, "After erasing even numbers");
}

// 模块四：复杂类型与深拷贝验证
void Test_Complex_Type_DeepCopy()
{
    cout << "\n=== Test 4: Complex Type (std::string) Deep Copy ===" << endl;
    
    pzh::vector<string> v_str;
    v_str.push_back("Hello");
    v_str.push_back("World");
    v_str.push_back("ZKP");
    v_str.push_back("Non-linear");

    // 验证拷贝构造的深拷贝特性
    pzh::vector<string> v_copy(v_str);
    
    // 修改原向量，观察副本是否受影响
    v_str[0] = "Modified";

    print_vector(v_str, "Original (Modified)");
    print_vector(v_copy, "Copy (Should remain 'Hello')");

    // 验证大量数据下的扩容是否正确处理了 string 的移动/拷贝
    pzh::vector<string> v_resize_test;
    v_resize_test.resize(5, "Init");
    print_vector(v_resize_test, "Resize Test");
}

int main()
{
    Test_Construction_And_Traversal();
    Test_Capacity_And_Memory();
    Test_Modifiers_And_IteratorInvalidation();
    Test_Complex_Type_DeepCopy();
    return 0;
}