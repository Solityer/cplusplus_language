#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <list>
#include <string>
#include <vector>

#include "list.h"
using namespace std;

// =========================================================================
// 模块一：C++ 标准库 std::list 接口与性能基准测试
// =========================================================================
namespace std_list_test
{
    /**
     * @brief 验证标准库链表的基础数据管理与排序逻辑
     * @details 考察 std::list 底层的双向循环链表结构如何结合 STL 算法库进行数据升降序处理。
     */
    void test_basic_and_sort()
    {
        std::cout << "\n[std_list_test] 1. 基础接口与排序测试 (Basic IO & Sort)..." << std::endl;
        list<int> lt = {1, 2, 3, 4, 5};  // 使用现代 C++ 初始化列表

        cout << "原始数据: ";
        for (auto e : lt)
        {
            cout << e << " ";
        }
        cout << endl;

        // 调用 std::list 自带的 sort 方法（底层通常实现为自底向上的归并排序，时间复杂度 O(N log N)）
        // 传入 greater<int>() 仿函数以实现降序排列
        lt.sort(greater<int>());

        cout << "降序排列: ";
        for (auto e : lt)
        {
            cout << e << " ";
        }
        cout << endl;
    }

    /**
     * @brief 验证节点的高效转移 (Splice) 与去重 (Unique)
     * @details 链表相对于连续内存容器(如vector)的核心优势在于通过指针操作实现 O(1) 复杂度的节点转移，而无需拷贝数据。
     */
    void test_unique_remove_splice()
    {
        std::cout << "\n[std_list_test] 2. 节点特有操作测试 (Unique, Remove & Splice)..." << std::endl;

        // 1. 测试 unique 与 remove
        list<int> lt = {1, 2, 3, 4, 3, 3, 3, 5, 5, 3};
        lt.sort();     // unique 仅能移除相邻的重复元素，故去重前必须保证有序
        lt.unique();   // 去重操作
        lt.remove(3);  // 移除所有值为 3 的节点

        cout << "去重并移除元素 '3' 后: ";
        for (auto e : lt)
        {
            cout << e << " ";
        }
        cout << endl;

        // 2. 测试 splice (节点接合)
        list<int> mylist1 = {1, 2, 3, 4};
        list<int> mylist2 = {10, 20, 30};

        auto it = mylist1.begin();
        ++it;  // it 指向 mylist1 的元素 '2'

        // 将 mylist2 的一段区间 [++mylist2.begin(), mylist2.end()) 转移至 mylist1 的 it 位置之前。
        // 该过程仅涉及底层指针的重新指向，不触发任何内存的申请与释放。
        mylist1.splice(it, mylist2, ++mylist2.begin(), mylist2.end());

        cout << "Splice 操作后 mylist1: ";
        for (auto e : mylist1)
        {
            cout << e << " ";
        }
        cout << endl;
    }

    /**
     * @brief 验证空间局部性 (Spatial Locality) 对排序性能的深远影响
     * @details 虽然 std::list 的归并排序与 std::vector 结合 std::sort 的时间复杂度同为 O(N log N)，
     * 但由于 vector 内存连续，能高度契合 CPU 缓存预取机制（Cache Prefetching），因此拷贝到 vector 排序往往更快。
     */
    void test_sort_performance()
    {
        std::cout << "\n[std_list_test] 3. 排序性能基准对比 (Performance Benchmark)..." << std::endl;
        srand(static_cast<unsigned int>(time(0)));
        const int N = 1000000;

        list<int> lt1;
        list<int> lt2;

        for (int i = 0; i < N; ++i)
        {
            auto e = rand();
            lt1.push_back(e);
            lt2.push_back(e);
        }

        // 测试方式一：利用连续内存（vector）进行排序
        int begin1 = clock();
        vector<int> v(lt2.begin(), lt2.end());  // 1. O(N) 拷贝
        sort(v.begin(), v.end());               // 2. O(N log N) 快排/内省排序
        lt2.assign(v.begin(), v.end());         // 3. O(N) 拷回
        int end1 = clock();

        // 测试方式二：直接在链表上利用指针调整进行排序
        int begin2 = clock();
        lt1.sort();  // 归并排序
        int end2 = clock();

        printf("方式一 (List -> Vector -> Sort -> List) 耗时: %d ms\n", end1 - begin1);
        printf("方式二 (List 自带 Sort)                 耗时: %d ms\n", end2 - begin2);
    }
}

// =========================================================================
// 模块二：自定义泛型双向链表 (pzh::list) 功能性与鲁棒性验证
// =========================================================================
namespace pzh_list_test
{
    // 自定义聚合类型，用于测试成员访问操作符
    struct AA
    {
        AA(int a1 = 0, int a2 = 0)
            : _a1(a1)
            , _a2(a2)
        {}
        int _a1;
        int _a2;
    };

    /**
     * @brief 验证迭代器的基础遍历与内存修改能力
     */
    void test_iterator_read_write()
    {
        std::cout << "\n[pzh_list_test] 1. 迭代器读写测试 (Iterator Read/Write)..." << std::endl;
        pzh::list<int> lt;
        lt.push_back(1);
        lt.push_back(2);
        lt.push_back(3);

        pzh::list<int>::iterator it = lt.begin();
        while (it != lt.end())
        {
            *it += 20;  // 验证 operator* 返回引用类型的写入能力
            cout << *it << " ";
            ++it;
        }
        cout << endl;
    }

    /**
     * @brief 验证拷贝构造与赋值重载的深拷贝 (Deep Copy) 语义
     * @details 若未正确实现深拷贝，析构时将导致同一块内存被释放两次（Double Free），引发系统崩溃。
     */
    void test_copy_control_semantics()
    {
        std::cout << "\n[pzh_list_test] 2. 拷贝控制语义测试 (Copy Control Semantics)..." << std::endl;
        pzh::list<int> lt1;
        lt1.push_back(10);
        lt1.push_back(20);

        // 验证拷贝构造
        pzh::list<int> lt2(lt1);

        pzh::list<int> lt3;
        lt3.push_back(99);

        // 验证赋值运算符重载 (Copy-and-Swap idiom)
        lt3 = lt1;

        cout << "lt1 原始数据: ";
        for (auto e : lt1)
        {
            cout << e << " ";
        }
        cout << "\nlt2 拷贝构造: ";
        for (auto e : lt2)
        {
            cout << e << " ";
        }
        cout << "\nlt3 赋值重载: ";
        for (auto e : lt3)
        {
            cout << e << " ";
        }
        cout << endl;
    }

    /**
     * @brief 验证重载的 operator-> 对聚合类型（如结构体）的支持
     * @details 迭代器模拟了原生指针的行为。此测试确保 it->成员名 能够被编译器正确解析为 (it.operator->())->成员名。
     */
    void test_aggregate_type_access()
    {
        std::cout << "\n[pzh_list_test] 3. 聚合类型成员访问测试 (Aggregate Type Access)..." << std::endl;
        pzh::list<AA> lt;
        lt.push_back(AA(1, 1));
        lt.push_back(AA(2, 4));
        lt.push_back(AA(3, 9));

        pzh::list<AA>::iterator it = lt.begin();
        while (it != lt.end())
        {
            // 通过 operator-> 直接访问内部元素的成员属性
            cout << "AA._a1: " << it->_a1 << ", AA._a2: " << it->_a2 << endl;
            ++it;
        }
    }

    /**
     * @brief 验证泛型算法接口的兼容性
     * @details 阐明 C++ 模板编程中 typename 关键字的核心用途：消除编译器对“依赖型作用域 (Dependent Scope)”内嵌类型的歧义。
     */
    void test_generic_algorithm_compatibility()
    {
        std::cout << "\n[pzh_list_test] 4. 泛型打印支持测试 (Generic Algorithm Compatibility)..." << std::endl;

        pzh::list<int> lt_int;
        lt_int.push_back(1);
        lt_int.push_back(2);
        cout << "泛型打印 pzh::list<int>: ";
        pzh::print_Container(lt_int);

        pzh::list<string> lt_str;
        lt_str.push_back("Zero");
        lt_str.push_back("Knowledge");
        lt_str.push_back("Proof");
        cout << "泛型打印 pzh::list<string>: ";
        pzh::print_Container(lt_str);

        // 验证我们实现的通用函数不仅支持 pzh::list，亦能完美兼容 std::vector
        vector<string> v_str;
        v_str.push_back("Non-linear");
        v_str.push_back("Function");
        cout << "泛型打印 std::vector<string>: ";
        pzh::print_Container(v_str);
    }
}

// =========================================================================
// 主程序入口
// =========================================================================
int main()
{
    std::cout << "========== Phase 1: Standard Library (std::list) Analysis ==========" << std::endl;
    std_list_test::test_basic_and_sort();
    std_list_test::test_unique_remove_splice();
    std_list_test::test_sort_performance();

    std::cout << "\n========== Phase 2: Custom Implementation (pzh::list) Analysis ==========" << std::endl;
    pzh_list_test::test_iterator_read_write();
    pzh_list_test::test_copy_control_semantics();
    pzh_list_test::test_aggregate_type_access();
    pzh_list_test::test_generic_algorithm_compatibility();

    std::cout << "\n[All Tests Finished Successfully]" << std::endl;
    return 0;
}