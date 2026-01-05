#include<iostream>
using namespace std;

#include<string>
#include<unordered_set>
#include<unordered_map>
#include<map>
#include<set>

#include"HashTable.h"
#include "MyUnorderedSet.h"
#include"MyUnorderedMap.h"


int main()
{
    unordered_set<int> s1;
    s1.insert(5);
    s1.insert(2);
    s1.insert(6);
    s1.insert(1);
    s1.insert(4);

    unordered_set<int>::iterator it = s1.begin();
    while (it != s1.end()) {
        cout << *it << " ";
        ++it;
    }
    cout << endl;
    for (auto e: s1) {
        cout << e << " ";
    }
    cout << endl;
    cout << s1.load_factor() << endl;
    cout << s1.max_load_factor() << endl;

    unordered_map<string, string> dict;
    dict["sort"];
    dict["sort"] = "排序";
    dict["string"] = "字符串";
    dict["abc"] = "xx";
    for (auto &kv: dict)
    {
        cout << kv.first << ":" << kv.second << endl;
    }
    cout << endl;

    const size_t N = 100000;
    unordered_set<int> us1;
    set<int> s2;
    vector<int> v1;
    v1.reserve(N);
    srand(time(0));
    for (size_t i = 0; i < N; ++i)
    {
        //v.push_back(rand()); // N比较大时，重复值比较多
        //v.push_back(rand()+i); // 重复值相对少
        v1.push_back(i); // 没有重复，有序
    }
    size_t begin1 = clock();
    for (auto e: v1) {
        s2.insert(e);
    }
    size_t end1 = clock();
    cout << "set insert:" << end1 - begin1 << endl;

    size_t begin2 = clock();
    for (auto e: v1) {
        us1.insert(e);
    }
    size_t end2 = clock();
    cout << "unordered_set insert:" << end2 - begin2 << endl;

    size_t begin3 = clock();
    for (auto e: v1) {
        s2.find(e);
    }
    size_t end3 = clock();
    cout << "set find:" << end3 - begin3 << endl;

    size_t begin4 = clock();
    for (auto e: v1) {
        us1.find(e);
    }
    size_t end4 = clock();
    cout << "unordered_set find:" << end4 - begin4 << endl << endl;

    cout << "插入数据个数：" << s2.size() << endl;
    cout << "插入数据个数：" << us1.size() << endl << endl;

    size_t begin5 = clock();
    for (auto e: v1) {
        s2.erase(e);
    }
    size_t end5 = clock();
    cout << "set erase:" << end5 - begin5 << endl;

    size_t begin6 = clock();
    for (auto e: v1) {
        us1.erase(e);
    }
    size_t end6 = clock();
    cout << "unordered_set erase:" << end6 - begin6 << endl << endl;

    pzh_open_address::TestHT1();
    pzh_open_address::TestHT2();

    pzh::test_map();
    pzh::test_set();

    return 0;
}
