#include <iostream>
using namespace std;

typedef int DataType;
class Stack {
public:
	Stack(size_t capacity = 4)
	{
		cout << "Stack()" << endl;
		_array = new int[capacity];
		_size = 0;
		_capacity = 4;
	}
	~Stack()
	{
		cout << "~Stack()" << endl;
		delete[] _array;
		_size = _capacity = 0;
	}
private:
    DataType* _array;
	int _capacity;
    int _size;
};

void func()
{
	char* p5 = new char[0x7ffffffff];
	cout << "func()" << endl;
}

// c/c++内存分布
int globalVar = 1;
static int staticGlobalVar = 1;
void Test()
{
    static int staticVar = 1;
    int localVar = 1;
    int num1[10] = { 1, 2, 3, 4 };
    char char2[] = "abcd";
    const char* pChar3 = "abcd";
    int* ptr1 = (int*)malloc(sizeof(int) * 4);
    int* ptr2 = (int*)calloc(4, sizeof(int));
    int* ptr3 = (int*)realloc(ptr2, sizeof(int) * 4);
    free(ptr1);
    free(ptr3);
}

int main()
{
	int* p1 = new int;  //不会初始化
    delete p1;
	int* p2 = new int(1);  //会初始化
    delete p2;
	int* p3 = new int[10];  //申请10个int的数组,不初始化
    delete[] p3;
	int* p4 = new int[10] {1, 2, 3, 4};  //会初始化
    delete[] p4;

	//内存申请失败抛异常,会导致执行流跳跃，和c语言的goto相似
	try {

//		char* p5 = new char[0x7ffffffff];
//		cout << "try char* p5" << endl;
		//可以直接try，也可以间接try
		func();
	}
	catch(const exception& e)
	{
		cout << e.what() << endl;
	}

    //定位new表达式
    Stack* pst1 = (Stack*)operator new(sizeof(Stack));
    // pst1->Stack(4);  //不支持
    new(pst1)Stack(4);  //显示调用构造函数
    pst1->~Stack();
    operator delete(pst1);

    Test();

	return 0;
}