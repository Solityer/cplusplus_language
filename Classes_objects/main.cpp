#include <iostream>
#include "Date.h"
#include "Stack.h"

using namespace std;

// === 演示静态成员与内部类 ===
class AdvancedFeature
{
public:
    AdvancedFeature()
    {
        ++_count;
    }

    AdvancedFeature(const AdvancedFeature &a)
    {
        ++_count;
    }

    ~AdvancedFeature()
    {
        --_count;
    }

    static int GetCount()
    {
        return _count;
    }

    // 内部类 (Inner Class)
    // 内部类天生是外部类的友元，可以访问外部类的私有成员(如 _privateData)
    class InnerObserver
    {
    public:
        void Inspect(const AdvancedFeature &outer)
        {
            cout << "[InnerClass] Accessing outer static private: " << _privateStaticVar << endl;
            cout << "[InnerClass] Accessing outer instance private: " << outer._privateData << endl;
        }
    };

private:
    static int _count;
    static int _privateStaticVar;
    int _privateData = 999;
};

// 静态成员变量必须在类外初始化
int AdvancedFeature::_count = 0;
int AdvancedFeature::_privateStaticVar = 42;


// === 测试函数集 ===

// 测试构造函数(含非法日期检查)
void Test_Constructor()
{
    cout << "===== 测试构造函数 =====" << endl;
    // 合法日期
    Date d1(2025, 2, 28); // 2025平年，2月合法日期
    d1.Print();

    Date d2(2025, 12, 31); // 2025年末日期
    d2.Print();

    // 非法日期(触发错误提示)
    Date d3(2025, 2, 29);  // 2025平年，2月无29天
    Date d4(2026, 13, 1);  // 2026年月份超出范围
    Date d5(2026, 1, 0);   // 2026年日期小于1
    cout << endl;
}

// 测试比较运算符(==、!=、<、<=、>、>=)
void Test_Compare_Operator()
{
    cout << "===== 测试比较运算符 =====" << endl;
    Date d1(2025, 5, 20);
    Date d2(2025, 5, 20);
    Date d3(2025, 5, 21);
    Date d4(2026, 12, 31);

    cout << (d1 == d2) << " (预期:1)" << endl;
    cout << (d1 != d3) << " (预期:1)" << endl;
    cout << (d1 < d3)  << " (预期:1)" << endl;
    cout << (d1 <= d2) << " (预期:1)" << endl;
    cout << (d4 > d1)  << " (预期:1)" << endl;
    cout << (d1 >= d3) << " (预期:0)" << endl;
    cout << endl;
}

// 测试算术运算符（+=、+、-=、- 天数）
void Test_Arithmetic_Operator()
{
    cout << "===== 测试算术运算符（加减天数） =====" << endl;
    Date d1(2025, 2, 28);
    // 测试 += （跨月+平年）
    d1 += 2; 
    cout << "2025-2-28 + 2天 = " << d1 << endl; // 预期：2025年3月2日

    // 测试 + （不修改原对象）
    Date d2(2025, 12, 30);
    Date d3 = d2 + 3;
    cout << "2025-12-30 + 3天 = " << d3 << endl; // 预期：2026年1月2日
    cout << "原对象值：" << d2 << endl;

    // 测试 -= （跨月+跨年）
    Date d4(2026, 1, 1);
    d4 -= 2;
    cout << "2026-1-1 - 2天 = " << d4 << endl;  // 预期：2025年12月30日

    // 测试 - （负数天数，等价于+）
    Date d5(2025, 5, 1);
    Date d6 = d5 - (-10);
    cout << "2025-5-1 - (-10)天 = " << d6 << endl; // 预期：2025年5月11日
    cout << endl;
}

// 测试自增/自减运算符(前置/后置)
void Test_Increment_Decrement()
{
    cout << "===== 测试自增/自减运算符 =====" << endl;
    Date d1(2025, 2, 28);
    
    // 前置++(先加后用)
    ++d1;
    cout << "2025-2-28 前置++ = " << d1 << endl; // 预期：2025年3月1日

    // 后置++(先用后加)
    Date d2 = d1++;
    cout << "2025-3-1 后置++ 临时值 = " << d2 << endl; // 预期：2025年3月1日
    cout << "2025-3-1 后置++ 原对象 = " << d1 << endl; // 预期：2025年3月2日

    // 前置--(先减后用)
    --d1;
    cout << "2025-3-2 前置-- = " << d1 << endl; // 预期：2025年3月1日

    // 后置--(先用后减)
    Date d3 = d1--;
    cout << "2025-3-1 后置-- 临时值 = " << d3 << endl; // 预期：2025年3月1日
    cout << "2025-3-1 后置-- 原对象 = " << d1 << endl; // 预期：2025年2月28日
    cout << endl;
}

// 测试日期相减(计算天数差)
void Test_Date_Subtract()
{
    cout << "===== 测试日期相减（天数差） =====" << endl;
    Date d1(2025, 1, 1);
    Date d2(2025, 1, 10);
    Date d3(2024, 12, 25); // 跨2024-2025年

    cout << "2025-1-10 - 2025-1-1 = " << (d2 - d1) << " 天" << endl;  // 预期：9
    cout << "2025-1-1 - 2025-1-10 = " << (d1 - d2) << " 天" << endl;  // 预期：-9
    cout << "2025-1-1 - 2024-12-25 = " << (d1 - d3) << " 天" << endl; // 预期：7
    cout << endl;
}

// 测试IO流运算符(cin >> Date / cout << Date)
void Test_IO_Operator()
{
    cout << "===== 测试IO流运算符 =====" << endl;
    Date d;
    cin >> d;  // 提示输入日期，例如输入：2026 6 18
    cout << "你输入的日期是：" << d << endl;
    cout << endl;
}

// 测试友元函数
void Test_friend()
{
    // ========== 测试 operator<<（输出友元函数） ==========
    cout << "----- 测试日期输出功能 -----" << endl;
    Date d1(2024, 5, 20);
    // 直接使用 cout 输出 Date 对象（调用 operator<< 友元函数）
    cout << "默认日期输出：" << d1 << endl;
    // 对比原有 Print 函数，验证输出一致性
    cout << "Print 函数输出：";
    d1.Print();
    cout << endl;

    // ========== 测试 operator>>（输入友元函数） ==========
    cout << "----- 测试日期输入功能 -----" << endl;
    Date d2;
    // 调用 operator>> 友元函数，手动输入日期（格式：年 月 日）
    cin >> d2;

    // 验证输入结果
    cout << "你输入的日期是：" << d2 << endl;
    // 额外验证：输入日期 + 10 天，测试功能完整性
    Date d3 = d2 + 10;
    cout << "输入日期 + 10 天 = " << d3 << endl;

    // ========== 边界值输入测试  ==========
    cout << "\n----- 测试边界日期输入 -----" << endl;
    Date d4;
    cout << "请输入闰年2月29日(例如:2024 2 29):";
    cin >> d4;
    cout << "你输入的闰年日期: " << d4 << endl;
}

void Test_Inner_Class()
{
    cout << "\n===== 测试Date内部类 DateValidator =====" << endl;

    // 1. 测试内部类访问外部类私有成员
    Date d1(2026, 6, 18);
    Date::DateValidator::PrintPrivate(d1); // 直接打印私有成员

    // 2. 测试内部类的日期合法性校验
    cout << "\n2. 测试日期合法性校验：" << endl;
    Date d2(2025, 2, 29); // 2025平年，2月29日非法
    cout << "  2025-2-29 是否合法：" << (Date::DateValidator::IsValid(d2) ? "是" : "否") << endl;

    Date d3(2024, 2, 29); // 2024闰年，2月29日合法
    cout << "  2024-2-29 是否合法：" << (Date::DateValidator::IsValid(d3) ? "是" : "否") << endl;

    Date d4(2026, 13, 1); // 月份非法
    cout << "  2026-13-1 是否合法：" << (Date::DateValidator::IsValid(d4) ? "是" : "否") << endl;

    // 3. 结合IO流测试：输入非法日期，内部类自动校验
    cout << "\n3. 输入非法日期（测试内部类校验联动）：" << endl;
    Date d5;
    cin >> d5; // 输入如：2025 2 29，会触发内部类校验并重置
    cout << "  最终日期：" << d5 << endl;
    cout << endl;
}

void Test_stack()
{
    // ========== 测试1：基本构造、入栈(Push)、打印(Print) ==========
    std::cout << "=== 测试1:基本入栈和打印功能 ===" << std::endl;
    Stack s1(5);  // 构造容量为5的栈
    s1.Push(1);
    s1.Push(2);
    s1.Push(3);
    std::cout << "s1的元素:";
    s1.Print();  // 预期输出：1 2 3

    // ========== 测试2：栈满的边界情况 ==========
    std::cout << "\n=== 测试2:栈满的边界情况 ===" << std::endl;
    s1.Push(4);
    s1.Push(5);  // 此时size == capacity（5）
    s1.Push(6);  // 触发栈满提示
    std::cout << "s1满栈后元素:";
    s1.Print();  // 预期输出：1 2 3 4 5

    // ========== 测试3：拷贝构造函数（验证深拷贝） ==========
    std::cout << "\n=== 测试3:拷贝构造(深拷贝验证 )===" << std::endl;
    Stack s2 = s1;  // 调用拷贝构造函数
    std::cout << "s2(拷贝s1)的元素:";
    s2.Print();  // 预期输出：1 2 3 4 5

    // 尝试修改s2，验证深拷贝（s1不会被影响）
    s2.Push(6);  // s2容量5，触发栈满提示
    std::cout << "修改s2后，s1的元素:";
    s1.Print();  // 预期仍为：1 2 3 4 5（深拷贝特性）

    // ========== 测试4：赋值运算符重载（验证深拷贝） ==========
    std::cout << "\n=== 测试4:赋值运算符(深拷贝验证) ===" << std::endl;
    Stack s3(3);  // 构造容量为3的临时栈
    s3.Push(10);
    s3.Push(20);
    std::cout << "赋值前s3的元素:";
    s3.Print();  // 预期输出：10 20

    s3 = s1;  // 调用赋值运算符
    std::cout << "赋值后s3的元素:";
    s3.Print();  // 预期输出：1 2 3 4 5

    // 尝试修改s3，验证深拷贝（s1不会被影响）
    s3.Push(6);  // 触发栈满提示
    std::cout << "修改s3后:s1的元素:";
    s1.Print();  // 预期仍为：1 2 3 4 5（深拷贝特性）

    // ========== 测试5：析构函数（局部对象销毁触发） ==========
    std::cout << "\n=== 测试5:析构函数调用（局部对象销毁） ===" << std::endl;
    {
        Stack s4;  // 局部对象，使用默认容量4
        s4.Push(100);
        s4.Push(200);
        std::cout << "s4的元素:";
        s4.Print();  // 预期输出：100 200
    }  // s4在此处离开作用域，触发析构函数（打印释放提示）

    // 程序结束时，s1/s2/s3会依次销毁，触发析构函数
    std::cout << "\n程序即将结束:全局作用域的s1/s2/s3开始销毁..." << std::endl;
}

int main()
{
    Test_Constructor();
    Test_Compare_Operator();
    Test_Arithmetic_Operator();
    Test_Increment_Decrement();
    Test_Date_Subtract();
    Test_IO_Operator();
    Test_friend();
    Test_Inner_Class();
    Test_stack();
    return 0;
}
