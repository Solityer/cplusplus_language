/*优先级队列：优先级是"比较"出来的，也可以理解为堆的使用*/
#include "stack.h"
#include "queue.h"
#include "priority_queue.h"
#include <string>
#include <algorithm>
#include <ctime>
#include <queue>

class Solution {
public:
    int evalRPN(vector<string>& tokens) {
        //pzh::stack<int, vector<int>> st;
        //pzh::stack<int, list<int>> st;
        pzh::stack<int> st;

        for (auto& str : tokens)
        {
            if (str == "+"
                || str == "-"
                || str == "*"
                || str == "/")
            {
                int right = st.top();
                st.pop();
                int left = st.top();
                st.pop();
                switch (str[0])
                {
                case '+':
                    st.push(left + right);
                    break;
                case '-':
                    st.push(left - right);
                    break;
                case '*':
                    st.push(left * right);
                    break;
                case '/':
                    st.push(left / right);
                    break;
                }
            }
            else
            {
                st.push(stoi(str));  //stoi = string to integer 把字符串转换为整数
            }
        }

        return st.top();
    }
};

void test_1() //比较vector和deque在相同数据量下使用std::sort的排序速度
{
    //time(0)获取当前时间，返回从1970年1月1日到现在的秒数
    //srand() - 设置随机种子，相同的种子会产生相同的随机数序列
    //用当前时间作为随机数种子，这样每次运行程序都会得到不同的随机数序列
    srand(time(0));
    const int N = 1000000;
    deque<int> dq;
    vector<int> v;
    for (int i = 0; i < N; ++i)
    {
        auto e = rand()+i;
        v.push_back(e);
        dq.push_back(e);
    }
    //clock()返回从程序启动到调用时的CPU时间
    int begin1 = clock();
    //sort()对[v.begin(), v.end())范围内的元素进行排序（前闭后开）
    sort(v.begin(), v.end());
    int end1 = clock();

    int begin2 = clock();
    sort(dq.begin(), dq.end());
    int end2 = clock();

    printf("vector:%d\n", end1 - begin1);
    printf("deque:%d\n", end2 - begin2);
}

void test_2()
{
    /*比较两种对 deque 排序的策略：
     方法1：直接对 deque 排序
     方法2：先将deque数据复制到vector，对vector排序，再复制回deque*/
    srand(time(0));
    const int N = 1000000;

    deque<int> dq1;
    deque<int> dq2;

    for (int i = 0; i < N; ++i)
    {
        auto e = rand() + i;
        dq1.push_back(e);
        dq2.push_back(e);
    }

    int begin1 = clock();
    sort(dq1.begin(), dq1.end());
    int end1 = clock();

    int begin2 = clock();
    vector<int> v(dq2.begin(), dq2.end());
    sort(v.begin(), v.end());
    //assign()函数清空当前容器的所有元素，用指定的新元素重新填充容器
    dq2.assign(v.begin(), v.end());
    int end2 = clock();

    printf("deque sort:%d\n", end1 - begin1);
    printf("deque copy vector sort, copy back deque:%d\n", end2 - begin2);
}

void test_queue()
{
    /*int：队列中存储的元素类型是 int
     *list<int>：指定使用list<int>作为队列的底层容器*/
    pzh::queue<int, list<int>> q;
    q.push(1);
    q.push(3);
    q.push(2);
    q.push(7);
    q.push(5);
    while (!q.empty())
    {
        cout << q.front() << " ";
        q.pop();
    }
    cout << endl;
}

//优先级队列，默认是一个大根堆
void test_Priority_Queue()
{
    // priority_queue<int, vector<int>, greater<int>> q;  //小根堆
    pzh::priority_queue<int> q;  //大根堆
    q.push(3);
    q.push(1);
    q.push(5);
    q.push(4);
    q.push(2);
    while (!q.empty())
    {
        cout << q.top() << " ";  //取一个
        q.pop();  //出一个
    }
    cout << endl;

    int a[] = { 1,2,6,2,1,5,9,4 };
    cout << "原始数组: ";
    for (int i = 0; i < 8; i++) {
        cout << a[i] << " ";
    }
    cout << endl;

    int b[] = { 1,2,6,2,1,5,9,4 };
    sort(b, b + 8, greater<int>());
    cout << "写法一结果: ";
    for (int i = 0; i < 8; i++) {
        cout << b[i] << " ";
    }
    cout << endl;

    int c[] = { 1,2,6,2,1,5,9,4 };
    greater<int> gt;
    sort(c, c + 8, gt);
    cout << "写法二结果: ";
    for (int i = 0; i < 8; i++) {
        cout << c[i] << " ";
    }
    cout << endl;
}

class Date
{
public:
    Date(int year = 1900, int month = 1, int day = 1)
        : _year(year)
        , _month(month)
        , _day(day)
    {}

    bool operator<(const Date& d) const
    {
        return (_year < d._year) ||
            (_year == d._year && _month < d._month) ||
            (_year == d._year && _month == d._month && _day < d._day);
    }

    bool operator>(const Date& d) const
    {
        return (_year > d._year) ||
            (_year == d._year && _month > d._month) ||
            (_year == d._year && _month == d._month && _day > d._day);
    }

    friend ostream& operator<<(ostream& _cout, const Date& d);
private:
    int _year;
    int _month;
    int _day;
};

ostream& operator<<(ostream& _cout, const Date& d)
{
    _cout << d._year << "-" << d._month << "-" << d._day;
    return _cout;
}

struct PDateCompare
{
    bool operator()(Date* p1, Date* p2)
    {
        return *p1 > *p2;
    }
};

int main()
{
    vector<string> v = {"4","13","5","/","+"};
    cout << Solution().evalRPN(v) << endl;
    test_1();
    test_2();
    test_queue();
    test_Priority_Queue();

    Less<int> less1; // 函数对象
    cout << less1(2, 3) << endl;
    cout << less1(4,3) << endl;
    cout << less1.operator()(2, 3) << endl;
    Less<double> less2; // 函数对象
    cout << less2(2.2, 3.3) << endl;

    pzh::priority_queue<Date> q1;
    q1.push(Date(2018, 10, 29));
    q1.push(Date(2018, 10, 28));
    q1.push(Date(2018, 10, 30));
    cout << q1.top() << endl;
    pzh::priority_queue<Date*, vector<Date*>, PDateCompare> q2;
    q2.push(new Date(2018, 10, 29));
    q2.push(new Date(2018, 10, 28));
    q2.push(new Date(2018, 10, 30));
    cout << *(q2.top()) << endl;
    return 0;
}

