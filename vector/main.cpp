#include "vector.h"

void test_vector1()
{
	vector<int> v;
	v.push_back(1);
	v.push_back(2);
	v.push_back(3);
	v.push_back(4);
	//遍历vector
	for (size_t i = 0; i < v.size(); ++i)
	{
		cout << v[i] << " ";
	}
	cout << endl;

	vector<int>::iterator it = v.begin();
	while (it != v.end())
	{
		cout << *it << " ";
		++it;
	}
	cout << endl;

	for (auto e : v)
	{
		cout << e << " ";
	}
	cout << endl;

	//拷贝
	vector<int> v1(v);
	for (auto e : v1)
	{
		cout << e << " ";
	}
	cout << endl;
}

void test_vector2()
{
	vector<int> v1(10, 1);  //初始化10个1
	for (auto e : v1)
	{
		cout << e << " ";
	}
	cout << endl;

	//迭代器区间构造
	vector<int> v2(v1.begin(), v1.end());
	for (auto e : v2)
	{
		cout << e << " ";
	}
	cout << endl;

	string s1("hello solity");
	vector<int> v3(s1.begin(), s1.end());
	for (auto e : v3)
	{
		cout << e << " ";  //char给int,输出的是字符的ascll值
	}
	cout << endl;
}

void test_vector3()
{
	vector<int> v;
	v.push_back(1);
	v.push_back(2);
	v.push_back(3);
	v.push_back(4);

	vector<int>::iterator it = v.begin();
	while (it != v.end())
	{
		cout << *it << " ";
		++it;
	}
	cout << endl;

	//auto rit = v.rbegin();
	//反向迭代器
	vector<int>::reverse_iterator rit = v.rbegin();
	while (rit != v.rend())
	{
		cout << *rit << " ";
		++rit;
	}
	cout << endl;
	//const迭代器 只读不写
}

//测试vector的默认扩容机制
/*默认构造的 vector 初始容量为 0（C++11 起）。
首次插入元素（如 push_back）时分配最小容量*/
void TestVectorExpand()
{
	size_t sz;
	vector<int> v;
	//v.reserve(100);  //提前开空间，则不存在下面的扩容机制
	sz = v.capacity();
	cout << "making v grow:\n";
	for (int i = 0; i < 100; ++i)
	{
		v.push_back(i);
		if (sz != v.capacity())
		{
			sz = v.capacity();
			cout << "capacity changed: " << sz << '\n';
		}
	}
}

void test_vector4()
{
	vector<int> v1;  // 创建空vector，size=0, capacity=0
	v1.resize(10, 1); // 调整大小为10，新元素初始化为1

	vector<int> v;
	v.push_back(1);
	v.push_back(2);
	v.push_back(3);
	v.push_back(4);
	for (auto e : v)
	{
		cout << e << " ";
	}
	cout << endl;
	//查找插入
	vector<int>::iterator pos = find(v.begin(), v.end(), 2);
	if (pos != v.end())
	{
		v.insert(pos, 20);  //在find值前面插入
	}
	for (auto e : v)
	{
		cout << e << " ";
	}
	cout << endl;
	//查找删除
	pos = find(v.begin(), v.end(), 2);
	if (pos != v.end())
	{
		v.erase(pos);  //删除find查找的值
	}
	for (auto e : v)
	{
		cout << e << " ";
	}
	cout << endl;
}

int main()
{
	test_vector1();
	test_vector2();
	test_vector3();
	TestVectorExpand();
	test_vector4();

	pzh::test_vector1();
	pzh::test_vector2();
	pzh::test_vector3();
	pzh::test_vector4();
	return 0;
}