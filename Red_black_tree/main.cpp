#include<iostream>
using namespace std;
#include<vector>
#include<string>
#include<ctime>
#include<cstdlib>
#include"RBTree.h"
#include"MyMap.h"
#include"MySet.h"

// 为直接使用RBTree定义仿函数
struct IntKeyOfT {
    const int &operator()(const int &k) {
        return k;
    }
};

// 为pair<int, int>定义仿函数
struct PairIntKeyOfT {
    const int &operator()(const pair<int, int> &kv) {
        return kv.first;
    }
};

// 测试红黑树的基本功能
void test_RBTree_basic() {
    cout << "========== 测试红黑树 ==========" << endl;

    // 直接使用RBTree（类似于Set）
    {
        cout << "\n直接使用RBTree（类似于Set）: ";
        int a1[] = {790, 760, 969, 270, 31, 424, 377, 24, 702};
        RBTree<int, int, IntKeyOfT> t1;
        for (auto e: a1) {
            cout << "插入 " << e << " ";
            t1.Insert(e); // 注意：这里插入的是int，不是pair
            cout << "是否平衡: " << t1.IsBalance() << endl;
        }
        // 使用迭代器遍历
        cout << "中序遍历结果: ";
        for (auto it = t1.begin(); it != t1.end(); ++it) {
            cout << *it << " ";
        }
        cout << endl;
        cout << "最终平衡检查: " << t1.IsBalance() << endl;
        cout << "树高度: " << t1.Height() << endl;
        cout << "节点数量: " << t1.Size() << endl;
    }

    // 使用RBTree存储pair（类似于Map）
    {
        cout << "\n使用RBTree存储pair（类似于Map）: ";
        int a2[] = {16, 3, 7, 11, 9, 26, 18, 14, 15};
        RBTree<int, pair<int, int>, PairIntKeyOfT> t2;
        for (auto e: a2) {
            cout << "插入 (" << e << ", " << e * 10 << ") ";
            t2.Insert(make_pair(e, e * 10)); // 存储值和值的10倍
            cout << "是否平衡: " << t2.IsBalance() << endl;
        }

        // 使用迭代器遍历pair
        cout << "中序遍历结果 (键值对):" << endl;
        for (auto it = t2.begin(); it != t2.end(); ++it) {
            cout << "Key: " << it->first << ", Value: " << it->second << endl;
        }

        // 测试查找功能
        cout << "\n查找测试: " << endl;
        for (auto e: a2) {
            auto node = t2.Find(e);
            if (node) {
                cout << "找到键 " << e << ", 对应值: " << node->_data.second << endl;
            } else {
                cout << "未找到键 " << e << endl;
            }
        }

        // 测试不存在的元素
        auto notFound = t2.Find(999);
        if (!notFound) {
            cout << "未找到键 999 (正确)" << endl;
        }
    }
}

// 测试红黑树性能
void test_RBTree_performance() {
    cout << "\n========== 测试红黑树性能 ==========" << endl;

    const int N = 100000;
    vector<int> v;
    v.reserve(N);
    srand(time(0));

    // 生成随机数
    for (size_t i = 0; i < N; i++) {
        v.push_back(rand() + i);
    }

    // 测试插入性能 - 类似于Set
    {
        size_t begin_insert = clock();
        RBTree<int, int, IntKeyOfT> t;
        for (auto e: v) {
            t.Insert(e);
        }
        size_t end_insert = clock();
        cout << "插入 " << N << " 个元素(Set模式)耗时: " << end_insert - begin_insert << " ms" << endl;

        // 验证红黑树性质
        cout << "插入后是否平衡: " << t.IsBalance() << endl;
        cout << "树高度: " << t.Height() << endl;
        cout << "节点数量: " << t.Size() << endl;

        // 测试查找性能
        size_t begin_find = clock();
        for (auto e: v) {
            t.Find(e);
        }
        size_t end_find = clock();
        cout << "查找 " << N << " 个元素耗时: " << end_find - begin_find << " ms" << endl;
    }

    // 测试插入性能 - 类似于Map
    {
        size_t begin_insert = clock();
        RBTree<int, pair<int, int>, PairIntKeyOfT> t;
        for (auto e: v) {
            t.Insert(make_pair(e, e));
        }
        size_t end_insert = clock();
        cout << "\n插入 " << N << " 个键值对(Map模式)耗时: " << end_insert - begin_insert << " ms" << endl;

        // 验证红黑树性质
        cout << "插入后是否平衡: " << t.IsBalance() << endl;
        cout << "树高度: " << t.Height() << endl;
        cout << "节点数量: " << t.Size() << endl;
    }
}

// 测试Map功能
void test_map() {
    cout << "\n========== 测试MyMap功能 ==========" << endl;

    pzh::map<string, string> dict;

    // 测试插入
    cout << "测试插入操作:" << endl;
    auto ret1 = dict.insert(make_pair("sort", "排序"));
    cout << "插入 (sort, 排序): " << (ret1.second ? "成功" : "失败(已存在)") << endl;

    auto ret2 = dict.insert(make_pair("sort", "xx"));
    cout << "再次插入 (sort, xx): " << (ret2.second ? "成功" : "失败(已存在)") << endl;

    dict.insert(make_pair("left", "左"));
    dict.insert(make_pair("right", "右"));
    dict.insert(make_pair("up", "上"));
    dict.insert(make_pair("down", "下"));

    // 测试迭代器遍历
    cout << "\n遍历字典内容:" << endl;
    pzh::map<string, string>::iterator it = dict.begin();
    while (it != dict.end()) {
        // 注意：实际使用中不应该修改key，这里仅作测试
        // it->first += 'x';  // 这行代码会导致编译错误，因为key是const
        // it->second += 'y'; // 这行代码可以正常执行

        cout << it->first << " : " << it->second << endl;
        ++it;
    }

    // 测试operator[]
    cout << "\n测试operator[]:" << endl;
    dict["hello"] = "你好";
    dict["world"] = "世界";
    dict["sort"] = "重新赋值"; // 修改已存在的键

    cout << "dict[\"hello\"] = " << dict["hello"] << endl;
    cout << "dict[\"sort\"] = " << dict["sort"] << endl;
    cout << "dict[\"nonexistent\"] = " << dict["nonexistent"] << endl; // 访问不存在的键

    // 测试统计功能
    cout << "\n测试统计功能:" << endl;
    string arr[] = {"apple", "banana", "apple", "orange", "banana", "apple", "grape", "banana", "apple"};
    pzh::map<string, int> countMap;
    for (auto &e: arr) {
        countMap[e]++;
    }

    cout << "水果统计结果:" << endl;
    for (auto &kv: countMap) {
        cout << kv.first << ": " << kv.second << endl;
    }

    // 测试const迭代器
    cout << "\n使用范围for循环遍历:" << endl;
    for (auto &kv: dict) {
        cout << kv.first << " -> " << kv.second << endl;
    }
}

// 测试Set功能
void test_set() {
    cout << "\n========== 测试MySet功能 ==========" << endl;

    pzh::set<int> s;

    // 测试插入
    cout << "插入元素: 4, 1, 2, 3, 2, 0, 10, 5" << endl;
    s.insert(4);
    s.insert(1);
    s.insert(2);
    s.insert(3);
    s.insert(2); // 重复元素
    s.insert(0);
    s.insert(10);
    s.insert(5);

    // 测试迭代器遍历
    cout << "\n集合内容(有序):" << endl;
    pzh::set<int>::iterator it = s.begin();
    while (it != s.end()) {
        cout << *it << " ";
        ++it;
    }
    cout << endl;

    // 测试范围for循环
    cout << "使用范围for循环:" << endl;
    for (auto e: s) {
        cout << e << " ";
    }
    cout << endl;

    // 测试插入重复元素
    cout << "\n测试插入重复元素:" << endl;
    auto ret = s.insert(3);
    cout << "再次插入3: " << (ret.second ? "成功" : "失败(已存在)") << endl;

    // 测试边界值
    cout << "\n测试边界值:" << endl;
    s.insert(-1);
    s.insert(100);

    cout << "最小值: " << *s.begin() << endl;

    // 测试大量数据
    cout << "\n测试大量数据插入:" << endl;
    pzh::set<int> large_set;
    for (int i = 0; i < 1000; i++) {
        large_set.insert(rand() % 10000);
    }

    // 测试有序性
    cout << "检查有序性: ";
    it = large_set.begin();
    int prev = *it;
    ++it;
    bool is_sorted = true;
    while (it != large_set.end()) {
        if (*it <= prev) {
            is_sorted = false;
            break;
        }
        prev = *it;
        ++it;
    }
    cout << (is_sorted ? "有序" : "无序") << endl;
}

// 测试红黑树迭代器
void test_RBTree_iterator() {
    cout << "\n========== 测试红黑树迭代器 ==========" << endl;

    // 使用pair存储
    RBTree<int, pair<int, int>, PairIntKeyOfT> t;
    int values[] = {50, 30, 70, 20, 40, 60, 80, 10, 25, 35, 45};

    for (auto v: values) {
        t.Insert(make_pair(v, v * 10)); // 存储值为键的10倍
    }

    cout << "中序遍历结果:" << endl;
    for (auto it = t.begin(); it != t.end(); ++it) {
        cout << "Key: " << it->first << ", Value: " << it->second << endl;
    }

    cout << "\n测试迭代器自增操作:" << endl;
    auto it = t.begin();
    cout << "第一个元素: Key=" << it->first << ", Value=" << it->second << endl;
    ++it;
    cout << "第二个元素: Key=" << it->first << ", Value=" << it->second << endl;

    // 测试迭代器相等性
    auto it1 = t.begin();
    auto it2 = t.begin();
    cout << "\n两个begin()迭代器是否相等: " << (it1 == it2 ? "是" : "否") << endl;

    // 测试遍历所有元素
    cout << "\n遍历所有元素:" << endl;
    int count = 0;
    for (auto it = t.begin(); it != t.end(); ++it) {
        cout << it->first << " ";
        count++;
    }
    cout << "\n总共遍历了 " << count << " 个元素" << endl;
}

// 测试红黑树的旋转和平衡
void test_RBTree_rotations() {
    cout << "\n========== 测试红黑树旋转和平衡 ==========" << endl;

    // 测试左旋情况
    {
        cout << "测试左旋情况(插入升序序列):" << endl;
        RBTree<int, int, IntKeyOfT> t;
        // 插入一个需要左旋的序列
        int values[] = {10, 20, 30}; // 插入10, 然后20, 然后30会触发左旋
        for (auto v: values) {
            t.Insert(v);
            cout << "插入 " << v << " 后是否平衡: " << t.IsBalance() << endl;
        }
        cout << "最终树高度: " << t.Height() << endl;
    }

    // 测试右旋情况
    {
        cout << "\n测试右旋情况(插入降序序列):" << endl;
        RBTree<int, int, IntKeyOfT> t;
        // 插入一个需要右旋的序列
        int values[] = {30, 20, 10}; // 插入30, 然后20, 然后10会触发右旋
        for (auto v: values) {
            t.Insert(v);
            cout << "插入 " << v << " 后是否平衡: " << t.IsBalance() << endl;
        }
        cout << "最终树高度: " << t.Height() << endl;
    }

    // 测试左右双旋情况
    {
        cout << "\n测试左右双旋情况:" << endl;
        RBTree<int, int, IntKeyOfT> t;
        // 插入一个需要左右双旋的序列
        int values[] = {30, 10, 20}; // 插入30, 然后10, 然后20会触发左右双旋
        for (auto v: values) {
            t.Insert(v);
            cout << "插入 " << v << " 后是否平衡: " << t.IsBalance() << endl;
        }
        cout << "最终树高度: " << t.Height() << endl;
    }

    // 测试右左双旋情况
    {
        cout << "\n测试右左双旋情况:" << endl;
        RBTree<int, int, IntKeyOfT> t;
        // 插入一个需要右左双旋的序列
        int values[] = {10, 30, 20}; // 插入10, 然后30, 然后20会触发右左双旋
        for (auto v: values) {
            t.Insert(v);
            cout << "插入 " << v << " 后是否平衡: " << t.IsBalance() << endl;
        }
        cout << "最终树高度: " << t.Height() << endl;
    }
}

// 测试红黑树的平衡性
void test_RBTree_balance() {
    cout << "\n========== 测试红黑树平衡性 ==========" << endl;

    // 测试极端情况：升序插入
    {
        cout << "测试升序插入(100个元素):" << endl;
        RBTree<int, int, IntKeyOfT> t;
        for (int i = 1; i <= 100; i++) {
            t.Insert(i);
        }
        cout << "升序插入100个元素后:" << endl;
        cout << "是否平衡: " << t.IsBalance() << endl;
        cout << "高度: " << t.Height() << endl;
        cout << "节点数: " << t.Size() << endl;
    }

    // 测试极端情况：降序插入
    {
        cout << "\n测试降序插入(100个元素):" << endl;
        RBTree<int, int, IntKeyOfT> t;
        for (int i = 100; i >= 1; i--) {
            t.Insert(i);
        }
        cout << "降序插入100个元素后:" << endl;
        cout << "是否平衡: " << t.IsBalance() << endl;
        cout << "高度: " << t.Height() << endl;
        cout << "节点数: " << t.Size() << endl;
    }

    // 测试随机插入
    {
        cout << "\n测试随机插入(1000个元素):" << endl;
        RBTree<int, int, IntKeyOfT> t;
        srand(time(0));
        for (int i = 0; i < 1000; i++) {
            t.Insert(rand() % 10000);
        }
        cout << "随机插入1000个元素后:" << endl;
        cout << "是否平衡: " << t.IsBalance() << endl;
        cout << "高度: " << t.Height() << endl;
        cout << "节点数: " << t.Size() << endl;
    }
}

int main() {
    // 测试红黑树基本功能
    test_RBTree_basic();
    // 测试红黑树旋转和平衡
    test_RBTree_rotations();
    // 测试红黑树迭代器
    test_RBTree_iterator();
    // 测试红黑树平衡性
    test_RBTree_balance();
    // 测试Map功能
    test_map();
    // 测试Set功能
    test_set();
    // 性能测试（注释掉，可能需要较长时间）
    // test_RBTree_performance();
    return 0;
}
