// ????????
#include<iostream>
using namespace std;
#include"AVLTree.h"
#include<vector>
#include <ctime>

int main()
{
    int a[] = {4, 2, 6, 1, 3, 5, 15, 7, 16, 14};
    AVLTree<int, int> t;
    for (auto e : a)
    {
        t.Insert(make_pair(e, e));
    }
    t.InOrder();

    cout << t.IsBalance() << endl;

    const int N = 20;
    vector<int> v;
    v.reserve(N);
    srand(time(0));
    for (size_t i = 0; i < N; i++)
    {
    	v.push_back(rand());
    	cout << v.back() << endl;
    }
    AVLTree<int, int> t0;
    for (auto e : v)
    {
    	if (e == 14604)
    	{
    		int x = 0;
    	}
    	t0.Insert(make_pair(e, e));
    	cout << "Insert:" << e << "->" << t0.IsBalance() << endl;
    }
    cout << t0.IsBalance() << endl;
    return 0;
}