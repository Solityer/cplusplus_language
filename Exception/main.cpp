#include <iostream>
#include <string>
#include <vector>
#include <exception>
#include <windows.h> // 用于 Sleep
#include <ctime>   // 用于 time/srand
#include <new>  // 用于 bad_alloc
using namespace std;

// ==========================================
// 1. 公共类定义 (Global Classes)
// ==========================================

// 用于演示构造/析构的类
class A
{
public:
    A()
    {
        cout << "  [A] 构造: A()" << endl;
    }

    ~A()
    {
        cout << "  [A] 析构: ~A()" << endl;
    }
};

// ------------------------------------------
// 服务器开发异常体系 (Exception Hierarchy)
// ------------------------------------------
class Exception
{
public:
    // 构造函数：初始化错误信息和编号
    Exception(const string& errmsg, int id)
        :_errmsg(errmsg)
        ,_id(id)
    {}

    // 虚函数，返回错误描述，允许子类重写
    virtual string what() const
    {
        return _errmsg;
    }

    // 获取错误编号
    int getId() const
    {
        return _id;
    }

protected:
    string _errmsg; // 错误描述
    int _id;  // 错误编号
};

// SQL 操作异常，继承自 Exception
class SqlException : public Exception
{
public:
    SqlException(const string& errmsg, int id, const string& sql)
        :Exception(errmsg, id)
        ,_sql(sql)
    {}

    // 重写 what 函数，附加 SQL 语句信息
    virtual string what() const
    {
        string str = "SqlException:";
        str += _errmsg;
        str += "->";
        str += _sql;
        return str;
    }

private:
    const string _sql;  // 导致异常的 SQL 语句
};

// 缓存操作异常，继承自 Exception
class CacheException : public Exception
{
public:
    CacheException(const string& errmsg, int id)
        :Exception(errmsg, id)
    {}

    // 重写 what 函数，添加异常类型前缀
    virtual string what() const
    {
        string str = "CacheException:";
        str += _errmsg;
        return str;
    }
};

// HTTP 服务器异常
class HttpServerException : public Exception
{
public:
    HttpServerException(const string& errmsg, int id, const string& type)
        :Exception(errmsg, id)
        ,_type(type)
    {}

    virtual string what() const
    {
        string str = "HttpServerException:";
        str += _type;
        str += ":";
        str += _errmsg;
        return str;
    }

private:
    const string _type;   // HTTP 请求类型（GET/POST等）
};

// ==========================================
// 2. 独立测试模块 (Test Namespaces)
// ==========================================

// 场景一：基础除法异常 (抛出const char*)
namespace Test_Basic
{
    double Division(int len, int time)
    {
        if (time == 0)
            throw "除0错误 (const char*)";  //throw可以抛任意类型的异常
        return (double)len / (double)time;
    }

    void Func()
    {
        A aa;
        try
        {
            int len = 10, time = 0; // 硬编码模拟输入，避免频繁交互
            cout << "  执行除法: " << len << " / " << time << endl;
            cout << Division(len, time) << endl;
        }
        catch (const char* s)  //抛的是什么类型异常，就捕获什么类型
        {
            cout << "  捕获异常: " << s << endl;
        }
        cout << "  Test_Basic Func 结束" << endl;
    }
}

// 场景二：抛出任意类型与捕获任意类型 (...)
namespace Test_CatchAll
{
    double Division(int len, int time)
    {
        if (time == 0)
        {
            string s("除0错误 (string object)");
            throw s;
        }
        return (double)len / (double)time;
    }

    void f1()
    {
        throw 1;
    }

    void Func()
    {
        A aa;
        try
        {
            int len = 10, time = 0;
            cout << "  执行除法: " << len << " / " << time << endl;
            cout << Division(len, time) << endl;
        }
        catch (const char* s)
        {
            cout << s << endl;
        }

        // 测试 f1 抛出 int
        try
        {
            f1();  // 调用会抛出int异常的函数
        }
        catch (...)
        {
             // 这里的catch是为了演示，为了不让程序崩溃，我们在main里做总控
             throw; // 重新抛出，让外层捕获
        }
        cout << "xxxxxxxxxxxx" << endl;
    }

    void Run()
    {
        try
        {
            Func();
        }
        catch (const string& s)  // 捕获从Func重新抛出的string异常
        {
            cout << "  捕获string异常: " << s << endl;
        }
        catch (...)  // 捕获所有其他类型异常（包括int）
        {
            cout << "  捕获未知异常 (可能是f1抛出的int)" << endl;
        }
    }
}

// 场景三：服务器异常继承体系模拟
namespace Test_ServerHierarchy
{
    void SQLMgr()
    {
        if (rand() % 7 == 0)
            throw SqlException("权限不足", 100, "select * from name = '张三'");
    }

    // 模拟缓存管理器，可能抛出CacheException或调用SQLMgr
    void CacheMgr()
    {
        if (rand() % 5 == 0)
            throw CacheException("权限不足", 100);
        else if (rand() % 6 == 0)
            throw CacheException("数据不存在", 101);
        SQLMgr();  // 调用下一层
    }

    // 模拟HTTP服务器，可能抛出HttpServerException或调用CacheMgr
    void HttpServer()
    {
        if (rand() % 3 == 0)
            throw HttpServerException("请求资源不存在", 100, "get");
        else if (rand() % 4 == 0)
            throw HttpServerException("权限不足", 101, "post");
        CacheMgr();
    }

    void Run()
    {
        srand((unsigned int)time(0));  // 设置随机数种子
        // 模拟5次请求处理
        for(int i = 0; i < 5; ++i)
        {
            cout << "  [请求 " << i+1 << "] ";
            try
            {
                HttpServer();  // 处理HTTP请求
                cout << "正常处理" << endl;
            }
            catch (const Exception& e) // 多态捕获：可以捕获所有派生类异常
            {
                cout << "捕获异常: " << e.what() << endl;
            }
            catch (...)  // 安全网：捕获所有其他异常
            {
                cout << "Unknown Exception" << endl;
            }
            Sleep(100);  // 等待100毫秒，模拟处理间隔
        }
    }
}

// 场景四：异常安全 (手动delete + 重新抛出)
namespace Test_ExceptionSafety_Manual
{
    double Division(int a, int b)
    {
        if (b == 0)
            throw "Division by zero condition!";
        return (double)a / (double)b;
    }

    void Func()
    {
        // 手动分配两段内存
        int* array1 = new int[10];
        int* array2 = new int[20];
        cout << "  已分配内存: array1=" << array1 << ", array2=" << array2 << endl;
        try
        {
            int len = 10, time = 0;
            Division(len, time);
        }
        catch (...)
        {
            cout << "  [异常发生] 手动清理资源..." << endl;
            // 异常发生时手动释放资源
            cout << "  delete [] " << array1 << endl;
            delete[] array1;
            cout << "  delete [] " << array2 << endl;
            delete[] array2;
            throw;  // 重新抛出异常
        }
        // 正常流程的释放（这里不会执行，因为上面已经抛出异常）
        delete[] array1;
        delete[] array2;
    }

    void Run()
    {
        try
        {
            Func();
        }
        catch (const char* errmsg)
        {
            cout << "  外层捕获重抛的异常: " << errmsg << endl;
        }
    }
}

// 场景五：Bad Alloc 标准异常
namespace Test_BadAlloc
{
    void Run()
    {
        try
        {
            // 尝试分配巨大的内存 (可能失败)
            // 注意：现代64位系统可能允许很大的虚拟内存，这里数字需要足够大
            // 为了安全起见，这里只做演示，捕获 bad_alloc
            cout << "  尝试分配超大内存..." << endl;
            char* p = new char[0x7fffffff]; // 尝试分配约2GB
            cout << "  分配成功: " << (void*)p << endl;
            delete[] p;
        }
        catch (const exception& e)  // 捕获标准异常
        {
            cout << "  捕获标准异常: " << e.what() << endl;
        }
    }
}

// ==========================================
// 3. Main 函数整合
// ==========================================
int main()
{
    cout << "========================================" << endl;
    cout << "1. 测试基础异常捕获 (Test_Basic)" << endl;
    cout << "========================================" << endl;
    Test_Basic::Func();
    cout << endl;

    cout << "========================================" << endl;
    cout << "2. 测试 Catch(...) 与重抛 (Test_CatchAll)" << endl;
    cout << "========================================" << endl;
    Test_CatchAll::Run();
    cout << endl;

    cout << "========================================" << endl;
    cout << "3. 测试服务器异常继承体系 (Test_ServerHierarchy)" << endl;
    cout << "========================================" << endl;
    Test_ServerHierarchy::Run();
    cout << endl;

    cout << "========================================" << endl;
    cout << "4. 测试异常安全 - 手动管理 (Test_ExceptionSafety_Manual)" << endl;
    cout << "========================================" << endl;
    Test_ExceptionSafety_Manual::Run();
    cout << endl;

    cout << "========================================" << endl;
    cout << "5. 测试 bad_alloc (Test_BadAlloc)" << endl;
    cout << "========================================" << endl;
    Test_BadAlloc::Run();
    cout << endl;
    return 0;
}