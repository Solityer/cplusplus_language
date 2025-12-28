#include"BinarySearchTree.h"
int main()
{
    int a1[] = {8, 3, 1, 10, 6, 4, 7, 14, 13};
    // 构造二叉树
    pzh::BSTree<int> tr1;
    for (auto e : a1)
    {
        tr1.Insert(e);
    }
    // 中序遍历二叉树
    tr1.InOrder();
    //删除节点
    tr1.Erase(14);
    tr1.InOrder();
    tr1.Erase(3);
    tr1.InOrder();
    tr1.Erase(8);
    tr1.InOrder();
    for (auto e : a1)
    {
        tr1.EraseR(e);
    }
    tr1.InOrder(); //测试，已经将全部节点删除了

    /////////////////////////////////////////////////////////////////////
    int a2[] = {8, 3, 1, 10, 6, 4, 7, 14, 13};
    pzh::BSTree<int> tr2;
    for (auto e : a2)
    {
        tr2.InsertR(e);
    }
    tr2.InOrder();
    pzh::BSTree<int> copy(tr2);
    copy.InOrder();
    pzh::BSTree<int> tr3;
    tr3 = tr2;
    tr3.InOrder();

    //////////////////////////////////////////////////////////////////////
    kv::BSTree<string, string> dict;
    dict.Insert("sort", "排序");
    dict.Insert("left", "左边");
    dict.Insert("right", "右边");
    dict.Insert("insert", "插入");
    dict.Insert("key", "关键字");

    string str;
    while (cin >> str)
    {
        if (str == "exit")
            break;
        kv::BSTreeNode<string, string>* ret = dict.Find(str);
        if (ret)
        {
            cout << ret->_value << endl;
        }
        else
        {
            cout << "无此单词" << endl;
        }
    }

    string arr[] = {"苹果", "西瓜", "苹果", "西瓜", "苹果", "苹果", "西瓜", "苹果", "香蕉", "苹果", "香蕉"};
    kv::BSTree<string, int> countTree;
    for (auto& e : arr)
    {
        kv::BSTreeNode<string, int>* ret = countTree.Find(e);
        if (ret == nullptr)
        {
            countTree.Insert(e, 1);
        }
        else
        {
            ret->_value++;
        }
    }
    countTree.InOrder();
    return 0;
}
