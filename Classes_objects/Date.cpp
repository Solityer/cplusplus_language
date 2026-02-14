#include "Date.h"

// 获取某月天数
// 逻辑：使用静态数组存储平年天数，针对闰年2月做特殊处理
int Date::GetMonthDay(int year, int month)
{
    assert(month > 0 && month < 13);
    static int monthArray[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    // 闰年判断规则：四年一闰百年不闰，或四百年一闰
    if (month == 2 && ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)))
    {
        return 29;
    }
    return monthArray[month];
}

// 构造函数
// 采用初始化列表（Initialization List），效率高于在函数体内赋值
Date::Date(int year, int month, int day)
    : _year(year)
    , _month(month)
    , _day(day)
{
    // 构造后的合法性检查
    if (month < 1 || month > 12 || day < 1 || day > GetMonthDay(year, month))
    {
        cout << "Error: Invalid Date " << year << "-" << month << "-" << day << endl;
        // 在实际工程中，此处应抛出异常
    }
}

// 拷贝构造函数
Date::Date(const Date &d)
    : _year(d._year)
    , _month(d._month)
    , _day(d._day)
{
    // Date类属于POD(Plain Old Data)类型的变体，浅拷贝即可满足需求
    // 但显式定义有助于理解拷贝过程
}

// 赋值运算符重载
// 处理 d1 = d2 的情况
Date &Date::operator=(const Date &d)
{
    if (this != &d) // 防止自我赋值
    {
        _year = d._year;
        _month = d._month;
        _day = d._day;
    }
    return *this;
}

Date::~Date()
{
    // 无资源需释放
}

void Date::Print() const
{
    cout << _year << "-" << _month << "-" << _day << endl;
}

// === 比较运算符实现 ===
bool Date::operator==(const Date &d) const
{
    return _year == d._year && _month == d._month && _day == d._day;
}

bool Date::operator!=(const Date &d) const
{
    return !(*this == d);
}

bool Date::operator<(const Date &d) const
{
    if (_year < d._year)
        return true;
    if (_year == d._year && _month < d._month)
        return true;
    return (_year == d._year && _month == d._month && _day < d._day);
}

bool Date::operator<=(const Date &d) const
{
    return (*this < d) || (*this == d);
}

bool Date::operator>(const Date &d) const
{
    return !(*this <= d);
}

bool Date::operator>=(const Date &d) const
{
    return !(*this < d);
}

// === 算术运算符实现 ===

// 日期 += 天数
Date &Date::operator+=(int day)
{
    if (day < 0)
        return *this -= (-day);

    _day += day;
    while (_day > GetMonthDay(_year, _month))
    {
        _day -= GetMonthDay(_year, _month);
        ++_month;
        if (_month == 13)
        {
            ++_year;
            _month = 1;
        }
    }
    return *this;
}

// 日期 + 天数
// 复用 operator+= 以减少代码重复
Date Date::operator+(int day) const
{
    Date tmp(*this); // 拷贝构造临时对象
    tmp += day; // 复用+=
    return tmp; // 返回临时对象（值返回）
}

// 日期 -= 天数
Date &Date::operator-=(int day)
{
    if (day < 0)
        return *this += (-day);

    _day -= day;
    while (_day <= 0)
    {
        --_month;
        if (_month == 0)
        {
            --_year;
            _month = 12;
        }
        _day += GetMonthDay(_year, _month);
    }
    return *this;
}

// 日期 - 天数
Date Date::operator-(int day) const
{
    Date tmp(*this);
    tmp -= day;
    return tmp;
}

// 前置++
Date &Date::operator++()
{
    *this += 1;
    return *this;
}

// 后置++
// int 参数仅用于占位，编译器通过是否有参数来区分前置/后置
Date Date::operator++(int)
{
    Date tmp(*this); // 保存修改前的值
    *this += 1; // 修改当前对象
    return tmp; // 返回修改前的值
}

Date &Date::operator--()
{
    *this -= 1;
    return *this;
}

Date Date::operator--(int)
{
    Date tmp(*this);
    *this -= 1;
    return tmp;
}

// 日期相减（计算天数差）
int Date::operator-(const Date &d) const
{
    Date max = *this;
    Date min = d;
    int flag = 1;

    if (*this < d)
    {
        max = d;
        min = *this;
        flag = -1;
    }

    int n = 0;
    while (min != max)
    {
        ++min;
        ++n;
    }
    return n * flag;
}

// === 友元函数实现 ===
// 注意：友元函数不属于Date类，因此不需要 Date:: 前缀
ostream &operator<<(ostream &out, const Date &d)
{
    out << d._year << "年" << d._month << "月" << d._day << "日";
    return out;
}

istream &operator>>(istream &in, Date &d)
{
    cout << "请输入日期(年 月 日): ";
    in >> d._year >> d._month >> d._day;
    return in;
}
