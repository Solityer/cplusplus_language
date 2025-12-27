#include <iostream>
using namespace std;
//using std::cout;
//using std::endl;

int a = 1;
void fun1();
void fun2(int a = 10, int b = 20, int c = 30);  //全缺省
int Add(int left, int right);
double Add(double left, double right);
//函数重载（顺序/类型不同）
void f(int a, char b);
void f(char b, int a);
void func(int a)
{
    cout << "void func(int a)" << endl;
}
void func(int a, int b = 1)
{
    cout << "void func(int a, int b)" << endl;
}
#define ADD(x,y) ((x) + (y)) //宏函数
void swap(int& x, int& y);
typedef struct Node
{
    struct Node* next;
    int val;
}Node, * pNode;
void pushback(Node*& phead, int x); //void pushback(pNode& phead, int x);

int main()
{
    cout << "pengzhihao";  //无换行
    cout << "pengzhihao" << endl;
    cout << "pengzhihao" << "\n";

    int n1 = 0;
    cout << "please enter n1 number:";
    cin >> n1;
    double* a1 = (double*)malloc(sizeof(double) * n1);
    if (a1 == NULL)
    {
        perror("malloc");
        exit(-1);
    }
    for (int i = 0; i < n1; ++i)
    {
        cin >> a1[i];
    }
    for (int i = 0; i < n1; ++i)
    {
        cout << a1[i] << endl;
    }

    fun1();

    //只能从左到右缺省
    fun2();
    fun2(12);

    Add(12, 12);
    cout << Add(12, 12) << endl;  //24
    Add(1.1, 1.1);

    f(1, 'a');
    f('a', 1);

    //两个fun构成函数重载，但是存在调用歧义
    //func(1);  //调用存在歧义
    func(1,2);

    int a2 = 10;
    int& ra2 = a2;  //等于a2取了一个别名叫ra2
    printf("%p\n", &a2);
    printf("%p\n", &ra2);  //a2和ra2是同一个地址

    int i2 = 0, j2 = 1;
    swap(i2, j2);
    cout << "i2 = " << i2 << endl;
    cout << "j2 = " << j2 << endl;

    Node* plist = NULL;
    pushback(plist, 1);
    pushback(plist, 2);
    pushback(plist, 3);

//    const int c = 2;
//    int& d = c;
//
//    int* p1 = NULL;
//    const int* p2 = p1;

    ADD(1, 2) * 3;
    ADD(0 | 2, 0 & 2);

    int a = 0;
    auto b = a;
    auto c = &a;
    cout << typeid(b).name() << endl;
    cout << typeid(c).name() << endl;

    //范围for
    int array[] = { 1, 2, 3, 4, 5 };
    for (int i = 0; i < sizeof(array) / sizeof(array[0]); ++i)  //正常写法
        cout << array[i] << " ";
    cout << endl;
    //范围for写法，依次取数组中的数据赋值给e，自定判断结束，自动迭代
    for (auto e : array)
        cout << e << " ";
    cout << endl;
    for(auto& e1 : array)
    {
        e1 *= 2;
        cout << e1 << endl;
    }
    return 0;
}

void fun1()
{
    int a = 0;
    printf("%d\n", a);
    //当局部变量与全局变量同名时，使用 ::a 可以明确指定要访问全局变量，避免命名冲突带来的混淆
    printf("%d\n", ::a);
}

void fun2(int a, int b, int c)
{
    cout << "a = " << a << endl;
    cout << "b = " << b << endl;
    cout << "c = " << c << endl;
    cout << endl;
}

int Add(int left, int right)
{
    cout << "int Add(int left, int right)" << endl;
    return left + right;
}
double Add(double left, double right)
{
    cout << "double Add(double left, double right)" << endl;
    return left + right;
}

void f(int a, char b)
{
    cout << "f(int a,char b)" << endl;
}
void f(char b, int a)
{
    cout << "f(char b, int a)" << endl;
}

void swap(int& x, int& y)
{
    int tmp = x;
    x = y;
    y = tmp;
}

void pushback(Node*& phead, int x)
{
    Node* newnode = (Node*)malloc(sizeof(Node));
    if (phead == nullptr)
    {
        phead = newnode;
    }
}