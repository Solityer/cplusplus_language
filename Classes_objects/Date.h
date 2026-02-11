#pragma once
#include <iostream>
#include <cassert>

using std::cout;
using std::cin;
using std::endl;
using std::ostream;
using std::istream;

/**
 * @brief Date 类
 * @details 该类主要用于演示“值语义”对象的封装。
 * 包含特性：
 * 1. 构造函数与初始化列表
 * 2. 操作符重载（算术、比较、IO流）
 * 3. const 成员函数的正确使用
 * 4. 友元函数的应用
 */
class Date
{
    // 友元函数声明：允许非成员函数访问私有成员
    // 用于支持 cin >> d 和 cout << d 的流式操作
    friend ostream &operator<<(ostream &out, const Date &d);

    friend istream &operator>>(istream &in, Date &d);

public:
    // 获取某年某月的天数（静态逻辑，不依赖对象实例，但此处作为工具函数存在）
    static int GetMonthDay(int year, int month);

    // 构造函数
    // 使用全缺省参数，通过初始化列表初始化成员
    // explicit 关键字防止隐式类型转换（如 Date d = 2024 被禁止，必须 Date d(2024)）
    explicit Date(int year = 1970, int month = 1, int day = 1);

    // 拷贝构造函数
    Date(const Date &d);

    // 赋值运算符重载
    Date &operator=(const Date &d);

    // 析构函数（Date类无动态资源，默认即可，但为了完整性显式写出）
    ~Date();

    // 打印函数
    void Print() const; // const修饰this指针，保证函数内不修改成员变量

    // === 比较运算符重载 ===
    bool operator==(const Date &d) const;

    bool operator!=(const Date &d) const;

    bool operator<(const Date &d) const;

    bool operator<=(const Date &d) const;

    bool operator>(const Date &d) const;

    bool operator>=(const Date &d) const;

    // === 算术运算符重载 ===
    // 日期 + 天数
    Date &operator+=(int day);

    Date operator+(int day) const; // 返回新对象，原对象不变

    // 日期 - 天数
    Date &operator-=(int day);

    Date operator-(int day) const;

    // 日期 - 日期（返回天数差）
    int operator-(const Date &d) const;

    // === 自增/自减运算符 ===
    Date &operator++(); // 前置++
    Date operator++(int); // 后置++（int参数用于占位区分）
    Date &operator--(); // 前置--
    Date operator--(int); // 后置--

private:
    int _year;
    int _month;
    int _day;
};
