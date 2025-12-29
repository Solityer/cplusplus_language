#include<iostream>
#include<vector>
using namespace std;
#include"AVLTree.h"

int main()
{
	int a[] = { 4, 2, 6, 1, 3, 5, 15, 7, 16, 14 };
	AVLTree<int, int> t;
	for (auto e : a)
	{
		// 使用make_pair创建键值对并插入
		t.Insert(make_pair(e, e));
	}
	t.InOrder();
	// 检查AVL树是否平衡，输出结果（1表示平衡，0表示不平衡）
	cout << t.IsBalance() << endl;


	const int N = 30;  // 定义常量N为30，表示要插入的随机数数量
	vector<int> v;
	v.reserve(N);  // 预分配向量容量为N，提高性能
	// 随机种子设置，若启用可使每次运行产生不同的随机数
	//srand(time(0));
	for (size_t i = 0; i < N; i++)
	{
		// 生成随机数并添加到向量末尾
		v.push_back(rand());
		cout << v.back() << endl;
	}

	AVLTree<int, int> t1;
	for (auto e : v)  // 遍历向量v中的所有随机数
	{
		// 将随机数作为键值对插入t1
		t1.Insert(make_pair(e, e));
		// 输出插入的元素，并检查原AVL树t是否平衡
		cout << "Insert:" << e << "->" << t1.IsBalance() << endl;
	}
	cout << t1.IsBalance() << endl;
	return 0;
}