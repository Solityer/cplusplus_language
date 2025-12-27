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

	// insert的返回类型是pair
	auto ret1 = s.insert(3);
	cout << ret1.second << endl;

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

	if (s.count(3))
	{
		cout << "3在" << endl;
	}
	else
	{
		cout << "3不在" << endl;
	}

	// 删除不存在的
	s.erase(30);

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

	itlow = myset1.lower_bound(25);   // lower_bound返回 大于等于 val值位置的iterator
	itup = myset1.upper_bound(70);    // upper_bound返回 大于 val值位置的iterator

	// [25, 70]
	myset1.erase(itlow, itup);  // 10 20 70 80 90

	for (auto e : myset1)
	{
		cout << e << " ";
	}
	cout << endl;

	//////////////////////////////////////////////////////////////
	std::set<int> myset2;

	for (int i = 1; i <= 5; i++)
		myset2.insert(i * 10);   // myset: 10 20 30 40 50

	//pair<set<int>::const_iterator, set<int>::const_iterator> ret;
	// equal_range() 函数是C++ STL中的一个二分查找算法
	auto ret = myset2.equal_range(40);

	std::cout << "the lower bound points to: " << *ret.first << '\n';   // >= val
	std::cout << "the upper bound points to: " << *ret.second << '\n';  // > val

}

void test_set3()
{
	// 排序
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

	// multiset允许相同的字符进入同一个set容器中
	// multset 仅排序
	multiset<int>::iterator it = s.begin();
	while (it != s.end())
	{
		cout << *it << " ";
		++it;
	}
	cout << endl;

	// 如果有多个值，find返回中序第一个val
	it = s.find(2);
	while (it != s.end())
	{
		cout << *it << " ";
		++it;
	}
	cout << endl;

	// 统计并输出容器s中值为1的元素个数
	cout << s.count(1) << endl;

	//auto ret = s.equal_range(val);   // 返回[>=val, >val)
	//s.erase(ret.first, ret.second);
	size_t n =  s.erase(2);
	cout << n << endl;  // 删除的个数
	for (auto e : s)
	{
		cout << e << " ";
	}
	cout << endl;
}

void test_map1()
{
	map<string, string> dict0;
	dict0.insert(make_pair("left", "左边"));
	dict0.insert(make_pair("left", "剩余"));
	// 插入不会关注value的值，只关注key
	for (auto& kv : dict0)
	{
		cout << kv.first << ":" << kv.second << endl;
	}

	map<string, string> dict;
	dict.insert(pair<string, string>("sort", "排序"));
	dict.insert(pair<string, string>("insert", "插入"));
	dict.insert(pair<const char*, const char*>("left", "左边"));
	dict.insert(make_pair("right", "右边")); // 推荐这个

	dict["erase"];  // 插入
	cout << dict["erase"] << endl; // 查找
	dict["erase"] = "删除"; // 修改
	cout << dict["erase"] << endl;// 查找
	dict["test"] = "测试";  // 插入+修改
	dict["left"] = "左边、剩余"; // 修改

	string s1("xxx"), s2("yyy");
	dict.insert(make_pair(s1, s2));

	map<string, string>::iterator it = dict.begin();
	while (it != dict.end())
	{
		cout << (*it).first << ":"<<(*it).second << endl;
		cout << it.operator->()->first << ":" << it.operator->()->second << endl;
		cout << it->first << ":" << it->second << endl;

		++it;
	}
	cout << endl;

	for (auto& kv : dict)
	{
		// kv.first += 'x';
		kv.second += 'x';
		cout << kv.first << ":" << kv.second << endl;
	}
}

void test_map2()
{
	string arr[] = { "苹果", "西瓜", "苹果", "西瓜", "苹果", "苹果", "西瓜", "苹果", "香蕉", "苹果", "香蕉" };
	map<string, int> countMap;
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

	for (auto& str : arr)
	{
		countMap[str]++;
	}

	for (auto& kv : countMap)
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