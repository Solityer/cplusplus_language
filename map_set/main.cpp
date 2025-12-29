#include<iostream>
#include<set>
#include<map>
using namespace std;

/*
std::set 的本质
	底层结构：平衡二叉搜索树（通常是红黑树）
	存储内容：只保存唯一键（key），没有值（value）
	自动排序：元素按键自动升序排列（可自定义比较函数）
	不允许重复：每个元素必须唯一
std::map 的特点
	底层结构：红黑树（Red-Black Tree），一种自平衡的二叉搜索树。
	元素顺序：按键（key）自动排序（默认升序，可通过比较函数自定义）
*/

void test_set1()
{
	// set 排序+去重
	// set是按照一定次序存储元素的容器
	// 每个value必须是唯一的。set中的元素不能在容器中修改(元素总是const)，但是可以从容器中插入或删除它们
	set<int> s;
	s.insert(5);
	s.insert(2);
	s.insert(6);
	s.insert(1);
	s.insert(1);
	s.insert(2);

	set<int>::iterator it1 = s.begin();
	while (it1 != s.end())
	{
		//*it = 10;
		cout << *it1 << " ";
		++it1;
	}
	cout << endl;

	// insert的返回类型是pair<iterator, bool>
	// first：指向插入元素的迭代器
	// second：是否插入成功（true表示成功，false表示已存在）
	auto ret1 = s.insert(3); // 插入3，auto自动推导为pair类型
	cout << ret1.second << endl; // 输出插入结果：1表示成功插入

	// 显式指定返回类型
	pair<set<int>::iterator, bool> ret2 = s.insert(4);
	cout << ret2.second << endl;
	for (auto e : s)
	{
		cout << e << " ";
	}
	cout << endl;
	// 删除存在的(直接用值删除)
	s.erase(2);

	// 用迭代器删除（必须确保删除的值存在）
	set<int>::iterator it2 = s.find(30);
	// it = s.find(3);
	if (it2 != s.end())
	{
		s.erase(it2);
	}

	// 使用count函数检查元素是否存在（返回0或1，因为set元素唯一）
	if (s.count(3))
	{
		cout << "3在" << endl;
	}
	else
	{
		cout << "3不在" << endl;
	}
	// 删除不存在的元素（不会报错）
	s.erase(30);  // 尝试删除30（不存在，无效果）
	for (auto e : s)
	{
		cout << e << " ";
	}
	cout << endl;
}

void test_set2()
{
	set<int> myset1;
	set<int>::iterator itlow, itup;

	for (int i = 1; i < 10; i++)
		myset1.insert(i * 10); // 10 20 30 40 50 60 70 80 90

	// lower_bound返回第一个 >= val 的元素位置的迭代器
	itlow = myset1.lower_bound(25); // 查找第一个>=25的元素，指向30
	// upper_bound返回第一个 > val 的元素位置的迭代器
	itup = myset1.upper_bound(70); // 查找第一个>70的元素，指向80
	// 删除区间[25, 70] -> 实际删除[30, 70]
	myset1.erase(itlow, itup);  // 删除从itlow到itup之间的元素（左闭右开）
	for (auto e : myset1)
	{
		cout << e << " ";
	}
	cout << endl;

	//////////////////////////////////////////////////////////////
	std::set<int> myset2;
	for (int i = 1; i <= 5; i++)
		myset2.insert(i * 10);   // myset: 10 20 30 40 50

	// equal_range() 函数是C++ STL中的一个二分查找算法
	// 返回一个pair，first是lower_bound的结果，second是upper_bound的结果
	//pair<set<int>::const_iterator, set<int>::const_iterator> ret;
	auto ret = myset2.equal_range(40); // 查找40的边界
	std::cout << "the lower bound points to: " << *ret.first << '\n';   // >= val
	std::cout << "the upper bound points to: " << *ret.second << '\n';  // > val
}

void test_set3()
{
	// multiset允许相同的字符进入同一个set容器中
	// multset 仅排序不去重
	multiset<int> s;
	s.insert(5);
	s.insert(2);
	s.insert(6);
	s.insert(1);
	s.insert(1);
	s.insert(2);
	s.insert(1);
	s.insert(5);
	s.insert(2);
	// 遍历multiset（自动排序）
	multiset<int>::iterator it = s.begin();
	while (it != s.end())
	{
		cout << *it << " ";
		++it;
	}
	cout << endl;

	// 查找元素：如果有多个相同值，find返回中序遍历的第一个val
	it = s.find(2);
	while (it != s.end())
	{
		cout << *it << " ";
		++it;
	}
	cout << endl;

	// 统计并输出容器s中值为1的元素个数
	cout << s.count(1) << endl;

	// equal_range示例：返回[>=val, >val)的迭代器对
	// auto ret = s.equal_range(val);
	// s.erase(ret.first, ret.second); // 删除所有值为val的元素

	// 删除所有值为2的元素，返回删除的元素个数
	size_t n = s.erase(2); // 删除值为2的所有元素
	cout << n << endl;  // 删除的个数
	for (auto e : s)
	{
		cout << e << " ";
	}
	cout << endl;
}

void test_map1()
{
	// map是键值对容器，键唯一，自动按键排序
	map<string, string> dict0;
	dict0.insert(make_pair("left", "左边"));
	dict0.insert(make_pair("left", "剩余"));   // 插入相同的key，不会插入（value不会被更新）
	// 插入不会关注value的值，只关注key
	for (auto& kv : dict0)
	{
		cout << kv.first << ":" << kv.second << endl;
	}

	map<string, string> dict;
	dict.insert(pair<string, string>("sort", "排序"));   // 使用pair构造函数
	dict.insert(pair<string, string>("insert", "插入"));
	dict.insert(pair<const char*, const char*>("left", "左边"));
	dict.insert(make_pair("right", "右边")); // 推荐使用make_pair

	// 使用[]操作符
	dict["erase"]; // 如果key不存在，会插入新元素，value使用默认值（空字符串）
	cout << dict["erase"] << endl; // 查找并输出value（空字符串）
	dict["erase"] = "删除"; // 修改已存在key的value
	cout << dict["erase"] << endl; // 再次输出value（"删除"）
	dict["test"] = "测试"; // 插入+修改：key不存在则插入，存在则修改
	dict["left"] = "左边、剩余"; // 修改已存在的key的value

	string s1("xxx"), s2("yyy");
	dict.insert(make_pair(s1, s2));

	// 使用迭代器遍历map
	map<string, string>::iterator it = dict.begin();
	while (it != dict.end())
	{
		// 三种访问方式
		cout << (*it).first << ":" << (*it).second << endl; // 使用解引用
		cout << it.operator->()->first << ":" << it.operator->()->second << endl; // 使用operator->
		cout << it->first << ":" << it->second << endl; // 使用箭头操作符（最常用）
		++it; // 移动到下一个元素
	}
	cout << endl;

	for (auto& kv : dict)
	{
		// kv.first += 'x'; // 错误：key是const，不能修改
		kv.second += 'x'; // 正确：value可以修改
		cout << kv.first << ":" << kv.second << endl;
	}
}

void test_map2()
{
	string arr[] = { "苹果", "西瓜", "苹果", "西瓜", "苹果", "苹果", "西瓜", "苹果", "香蕉", "苹果", "香蕉" };
	map<string, int> countMap;  // 创建map，键是string（水果名），值是int（出现次数）
	// 方法1：使用find查找并统计
	for (auto& str : arr)
	{
		auto ret = countMap.find(str);
		if (ret == countMap.end())
		{
			// 没有表示第一次出现，插入
			countMap.insert(make_pair(str, 1));
		}
		else
		{
			// 次数++
			ret->second++;
		}
	}

	for (auto& kv : countMap)
	{
		cout << kv.first << ":" << kv.second << endl;
	}


	// 方法2：使用[]操作符简化统计
	// 先清空map
	countMap.clear(); // 清空map
	// 或者创建新的map
	map<string, int> countMap2;
	for (auto& str : arr)
	{
		countMap2[str]++; // []操作符会自动插入不存在的key，value默认0，然后++
	}

	// 输出统计结果
	for (auto& kv : countMap2)
	{
		cout << kv.first << ":" << kv.second << endl;
	}
}

int main()
{
	test_set1();
	test_set2();
	test_set3();
	test_map1();
	test_map2();
	return 0;
}