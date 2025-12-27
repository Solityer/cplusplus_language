#include "Date.h"

void TestDate1()
{
    Date d1(2023, 7, 23);
    Date d2;
    d1.Print();
    d2.Print();
    Date d3(2010, 2, 29);
    d3.Print();
    Date d4(2023, 13, 29);
    d4.Print();
}

void TestDate2()
{
    Date d1(2023, 8, 6);
    // 拷贝构造，一个已经存在的对象去初始化另一个要创建对象
    Date d2(d1);

    d1.Print();
    d2.Print();

    Date d3(2023, 8, 13);
    // 赋值，两个已经存在对象进行拷贝
    //d1 = d3;  // d1.operator=(d3)
    d1 = d2 = d3;
    d1.Print();
    d2.Print();
    d3.Print();

    d1 = d1;

    int i, j;
    i = j = 10;
    cout << (j = 10) << endl;
}

void TestDate3()
{
    Date d1(2023, 7, 27);
    d1 += 20000;
    d1.Print();

    Date d2(2023, 7, 27);
    //Date ret = d2;
    //Date ret(d2 + 20000);
    // 拷贝构造
    //Date ret = d2 + 20000;

    Date ret;
    // 赋值
    ret = d2 + 20000;

    ret.Print();
}

void TestDate4()
{
    Date d1(2023, 7, 27);
    d1 -= 20000;
    d1.Print();

    Date d2(2023, 7, 27);
    d2 += -200;
    d2.Print();

    Date d3(2023, 7, 27);
    d3 -= -200;
    d3.Print();
}

void TestDate5()
{
    Date d1(2023, 7, 27);
    Date ret1 = d1++;
    //Date ret1 = d1.operator++(0);
    ret1.Print();
    d1.Print();

    Date ret2 = ++d1;
    //Date ret2 = d1.operator++();
    ret2.Print();
    d1.Print();
}

void TestDate6()
{
    Date d1(2023, 7, 27);
    Date d2(2003, 1, 1);

    cout << d1 - d2 << endl;
}

void TestDate7()
{
//    const Date d1(2023, 7, 27);
//    d1.Print(&d1);
//    d1.Print();

    Date d2(2023, 7, 27);
    // d2.Print(&d2);
    d2.Print();
}

int main()
{
    Date d1(2024,3,10);
    Date d2(2024,3,1);
    d1 == d2;
    cout << (d1 == d2) << endl;

    const Date d3(2000,01,01);
    d3.Print();


    return 0;
}


