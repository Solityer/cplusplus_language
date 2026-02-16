#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <exception>
#include "string.h"
#include "Vector.h"

using std::cin;
using std::cout;
using std::endl;
using std::exception;

// 前置声明：用于 const 迭代器测试的辅助函数
void func_const_test(const pzh::string &s)
{
    cout << "[Const Iterator Test]: ";
    pzh::string::const_iterator it = s.begin();
    while (it != s.end())
    {
        // *it += 1; // 编译错误：不能修改 const 迭代器指向的内容
        cout << *it << " ";
        ++it;
    }
    cout << endl;
}

/**
 * @brief 模块1：基础构造与迭代器核心测试
 */
void Test_Construction_And_Iteration()
{
    cout << "==============================================================" << endl;
    cout << "[模块1] 基础构造 & 迭代器协议验证" << endl;
    cout << "==============================================================" << endl;

    pzh::string s1("hello world");
    pzh::string s2(s1);
    pzh::string s3 = "test assignment";
    pzh::string s4;
    pzh::string s5(s1, 0, 5);
    pzh::string s6(10, '*');

    cout << "s1 (带参构造): " << s1 << endl;
    cout << "s2 (拷贝构造): " << s2 << endl;
    cout << "s3 (赋值构造): " << s3 << endl;
    cout << "s4 (默认构造): " << s4.c_str() << endl;
    cout << "s5 (子串构造): " << s5 << endl;
    cout << "s6 (重复字符): " << s6 << endl;

    cout << "\n[普通迭代器-写操作] 字符ASCII+1: ";
    pzh::string::iterator it = s1.begin();
    while (it != s1.end())
    {
        (*it)++;
        cout << *it;
        ++it;
    }
    cout << endl;

    cout << "[范围for循环-还原字符]: ";
    for (auto &ch : s1)
    {
        ch--;
    }
    for (auto ch : s1)
    {
        cout << ch;
    }
    cout << endl;
}

/**
 * @brief 模块2：容量管理与深拷贝验证
 */
void Test_Capacity_And_DeepCopy()
{
    cout << "\n==============================================================" << endl;
    cout << "[模块2] 内存管理 & 深拷贝验证" << endl;
    cout << "==============================================================" << endl;

    pzh::string str_src("Deep Copy Test String");
    pzh::string str_dest(str_src);

    if ((void *)str_src.c_str() != (void *)str_dest.c_str())
        cout << "[结果] 深拷贝成功：内存地址不同" << endl;
    else
        cout << "[结果] 深拷贝失败：检测到浅拷贝！" << endl;

    pzh::string s_cap;
    s_cap.reserve(50);
    cout << "\n[reserve(50)] 容量: " << s_cap.capacity()
         << ", 大小: " << s_cap.size() << endl;

    s_cap.resize(60, 'x');
    cout << "[resize(60, 'x')] 容量: " << s_cap.capacity()
         << ", 大小: " << s_cap.size() << endl;
    cout << "[resize后内容]: " << s_cap << endl;

    s_cap.resize(5);
    cout << "[resize(5)缩容] 容量: " << s_cap.capacity()
         << ", 大小: " << s_cap.size() << endl;
}

/**
 * @brief 模块3：修改器与算法核心测试
 */
void Test_Modifiers_And_Algorithms()
{
    cout << "\n==============================================================" << endl;
    cout << "[模块3] 修改器 & 算法（find/insert/erase/substr）" << endl;
    cout << "==============================================================" << endl;

    pzh::string s_modify("hello solity");
    cout << "原始字符串: " << s_modify << endl;

    s_modify.insert(5, 1, ' ');
    cout << "[insert(5, ' ')]: " << s_modify << endl;

    s_modify.erase(5, 1);
    cout << "[erase(5, 1)]: " << s_modify << endl;

    s_modify.replace(5, 1, "%20");
    cout << "[replace(5,1,\"%20\")]: " << s_modify << endl;

    // URL 解析示例
    pzh::string url("ftp://www.example.com/resources/file.zip");
    size_t pos_protocol = url.find(':');
    if (pos_protocol != pzh::string::npos)
    {
        pzh::string protocol = url.substr(0, pos_protocol);
        cout << "协议: " << protocol << endl;
    }
}

/**
 * @brief 模块4：拓展特性测试
 */
void Test_Extended_Features()
{
    cout << "\n==============================================================" << endl;
    cout << "[模块4] 拓展特性（find_first_of/swap/预扩容）" << endl;
    cout << "==============================================================" << endl;

    pzh::string str_replace("please replace the vowels in this sentence by asterisks.");

    size_t found = str_replace.find_first_of("aeiou");
    while (found != pzh::string::npos)
    {
        str_replace[found] = '*';
        found = str_replace.find_first_of("aeiou", found + 1);
    }
    cout << "[find_first_of] 元音替换为*: " << str_replace << endl;

    pzh::string s_swap1("hello solity");
    pzh::string s_swap2("######");
    s_swap1.swap(s_swap2);
    cout << "[swap后] s_swap1: " << s_swap1 << ", s_swap2: " << s_swap2 << endl;
}

/**
 * @brief 模块5：泛型容器集成测试
 */
void Test_Vector_Integration()
{
    cout << "\n==============================================================" << endl;
    cout << "[模块5] 泛型容器集成（Vector<pzh::string>）" << endl;
    cout << "==============================================================" << endl;

    Vector<pzh::string> vStr(10);
    pzh::string s1("Generic");
    pzh::string s2("Programming");
    pzh::string s3("Test");

    vStr.PushBack(s1);
    vStr.PushBack(s2);
    vStr.PushBack(s3);
    vStr.PushBack("Implicit Construct");

    cout << "Vector大小: " << vStr.Size() << endl;
    for (size_t i = 0; i < vStr.Size(); ++i)
    {
        cout << "[" << i << "]: " << vStr[i] << endl;
    }
}

/**
 * @brief 模块6：比较运算符重载测试
 */
void Test_Comparison_Operators()
{
    cout << "\n==============================================================" << endl;
    cout << "[模块6] 比较运算符重载验证" << endl;
    cout << "==============================================================" << endl;

    pzh::string s1("apple");
    pzh::string s2("banana");

    cout << "s1: " << s1 << ", s2: " << s2 << endl;
    cout << "s1 < s2: " << (s1 < s2) << endl;
    cout << "s1 == s1: " << (s1 == s1) << endl;
}

/**
 * @brief 模块7：综合知识点演示
 */
void Test_Knowledge_Points()
{
    cout << "\n==============================================================" << endl;
    cout << "[模块7] 综合知识点演示 (Char/Capacity/Reverse Iterator)" << endl;
    cout << "==============================================================" << endl;

    // 1. 字符编码测试 (注意：依赖源文件编码，通常UTF-8下中文为3字节，GBK为2字节)
    char str2[] = "小透明";
    cout << "sizeof(str2): " << sizeof(str2) << " (包含\\0)" << endl;
    // 修改字节演示
    str2[3]++;
    cout << "修改字节后: " << str2 << endl;
    str2[3]--;
    cout << "还原字节后: " << str2 << endl;

    // 2. 容量与大小接口
    pzh::string s8("hello solity");
    cout << "size(): " << s8.size() << endl;
    cout << "length(): " << s8.length() << endl;     // 已补全
    cout << "max_size(): " << s8.max_size() << endl; // 已补全
    cout << "capacity(): " << s8.capacity() << endl;

    // 3. 追加操作
    pzh::string s9("hello solity");
    s9 += ' ';
    s9 += "world";
    cout << "Append Result: " << s9 << endl;

    // 4. 反向迭代器演示
    pzh::string s11("hello solity");
    cout << "Reverse Iterator: ";
    pzh::string::reverse_iterator rit = s11.rbegin(); // 已补全
    while (rit != s11.rend())                         // 已补全
    {
        cout << *rit << " ";
        ++rit;
    }
    cout << endl;

    // 5. Const迭代器调用
    func_const_test(s11);

    // 6. 迭代器删除
    pzh::string s14("hello solity");
    // 原始调用: s14.erase(s14.begin() + 5);
    // string.h 中已补全 erase(iterator) 接口以支持此操作
    s14.erase(s14.begin() + 5);
    cout << "Iterator Erase Result: " << s14 << endl;

    // 7. 查找与替换循环
    pzh::string s18("hello world solity pzh");
    size_t num = 0;
    for (auto ch : s18)
    {
        if (ch == ' ')
            ++num;
    }

    // 预扩容策略
    s18.reserve(s18.size() + 3 * num);

    size_t pos2 = s18.find(' ');
    while (pos2 != pzh::string::npos)
    {
        s18.replace(pos2, 1, "%d%");
        pos2 = s18.find(' ', pos2 + 3);
    }
    cout << "Batch Replace Result: " << s18 << endl;
}

int main()
{
    try
    {
        Test_Construction_And_Iteration();
        Test_Capacity_And_DeepCopy();
        Test_Modifiers_And_Algorithms();
        Test_Extended_Features();
        Test_Vector_Integration();
        Test_Comparison_Operators();
        Test_Knowledge_Points();

        cout << "\n==============================================================" << endl;
        cout << "[全部测试完成] 无异常触发" << endl;
    }
    catch (const exception &e)
    {
        std::cerr << "\n[标准异常捕获] " << e.what() << endl;
    }
    catch (...)
    {
        std::cerr << "\n[未知异常捕获] 程序异常退出" << endl;
    }

    return 0;
}