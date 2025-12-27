#include<iostream>
using namespace std;
#include<unordered_set>  //无序_set容器
#include<unordered_map>
#include<map>
#include<set>
#include<ctime>
#include<vector>

// #include "HashTable_1.h"
// int main()
// {
//     unordered_set<int> s;
//     s.insert(5);
//     s.insert(2);
//     s.insert(6);
//     s.insert(1);
//     s.insert(4);
//     unordered_set<int>::iterator it = s.begin();
//     while (it != s.end())
//     {
//         cout << *it << " ";
//         ++it;
//     }
//     cout << endl;
//
//     unordered_map<string, string> dict1;
//     dict1["sort"] = "排序";
//     dict1["string"] = "字符串";
//     dict1["abc"] = "xx";
//     for (auto& kv : dict1)
//     {
//         cout << kv.first << ":" << kv.second << endl;
//     }
//     cout << endl;
//
//     map<string, string> dict2;
//     dict2["sort"] = "排序";
//     dict2["string"] = "字符串";
//     dict2["abc"] = "xx";
//     for (auto& kv : dict2)
//     {
//         cout << kv.first << ":" << kv.second << endl;
//     }
//     cout << endl;
//
//     const size_t N = 100000;
//
//     vector<int> v1;
//     srand(time(0));
//     for (size_t i = 0; i < N; ++i)
//     {
//         //v1.push_back(rand()); // N比较大时，重复值比较多
//         //v1.push_back(rand()+i); // 重复值相对少
//         v1.push_back(i); // 没有重复，有序
//         // cout << v1[i] << " ";
//     }
//     cout << endl;
//     ///////////////////////性能测试/////////////////////////////
//     unordered_set<int> us1;
//     set<int> s1;
//     // clock() 返回程序从启动到当前时刻的 CPU 时间（时钟周期数）
//     size_t begin1 = clock();
//     for (auto e : v1)
//     {
//         s1.insert(e);
//     }
//     size_t end1 = clock();
//     cout << "set insert:" << end1 - begin1 << endl;
//
//     size_t begin2 = clock();
//     for (auto e : v1)
//     {
//         us1.insert(e);
//     }
//     size_t end2 = clock();
//     cout << "unordered_set insert:" << end2 - begin2 << endl;
//
//     size_t begin3 = clock();
//     for (auto e : v1)
//     {
//         s1.find(e);
//     }
//     size_t end3 = clock();
//     cout << "set find:" << end3 - begin3 << endl;
//
//     size_t begin4 = clock();
//     for (auto e : v1)
//     {
//         us1.find(e);
//     }
//     size_t end4 = clock();
//     cout << "unordered_set find:" << end4 - begin4 << endl << endl;
//
//     cout << "插入数据个数：" << s1.size() << endl;
//     cout << "插入数据个数：" << us1.size() << endl << endl;
//
//     size_t begin5 = clock();
//     for (auto e : v1)
//     {
//         s1.erase(e);
//     }
//     size_t end5 = clock();
//     cout << "set erase:" << end5 - begin5 << endl;
//
//     size_t begin6 = clock();
//     for (auto e : v1)
//     {
//         us1.erase(e);
//     }
//     size_t end6 = clock();
//     cout << "unordered_set erase:" << end6 - begin6 << endl << endl;
//
//     pzh::TestHT1();
//     pzh::TestHT2();
//
//     hash_bucket::TestHT();
//     return 0;
// }

// /////////////////////////////////////////////////////////////////////////////////////////////

#include "HashTable.h"
#include"MyUnorderedMap.h"
#include"MyUnorderedSet.h"
int main()
{
    pzh_map::test_map();
    pzh_set::test_set();
    return 0;
}