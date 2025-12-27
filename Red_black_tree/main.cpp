#include<iostream>
using namespace std;
#include"RBTree.h"
#include<vector>
#include <ctime>
int main()
{
    int a[] = {4, 2, 6, 1, 3, 5, 15, 7, 16, 14};
    RBTree<int, int> t1;
    for (auto e : a)
    {
        t1.Insert(make_pair(e, e));
    }
    t1.InOrder();
    cout << t1.IsBalance() << endl;

    return 0;
}
