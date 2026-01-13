#include <iostream>
#include <map>
#include <string>
#include <list>
#include <vector>
#include <fstream>   // 引入文件流库，用于文件读写（ifstream, ofstream）
#include <sstream>   // 引入字符串流库，用于字符串拼接和序列化（stringstream）
using namespace std;

// ==========================================================================
//                          Part 1: 单例模式相关类
// ==========================================================================

// -------------------------
// 类 A: 饿汉模式
// -------------------------
// 优点：实现简单，线程安全（因为静态成员在 main 之前初始化）
// 缺点：1、可能会导致进程启动慢（加载时就要初始化）；2、无法控制不同单例之间的初始化顺序
class A
{
public:
    // 获取单例实例的公共静态方法
    static A* GetInstance()
    {
       return &_inst; // 直接返回静态成员变量 _inst 的地址
    }

    // 业务函数：向字典中添加数据
    void Add(const string& key, const string& value)
    {
       _dict[key] = value; // 利用 map 的 [] 运算符插入或修改键值对
    }

    // 业务函数：打印字典中的所有数据
    void Print()
    {
       for (auto& kv : _dict) // 使用范围 for 循环遍历 map，kv 类型自动推导为 pair<const string, string>&
       {
          cout << kv.first << ":" << kv.second << endl; // 输出 key 和 value
       }
       cout << endl; // 输出空行以便分隔
    }

private:
    // 私有化构造函数，禁止在类外部随意创建对象
    A()
    {}

    // 删掉拷贝构造函数，禁止拷贝单例对象（C++11 特性）
    A(const A& aa) = delete;
    // 删掉赋值运算符重载，禁止赋值单例对象
    A& operator=(const A& aa) = delete;

    map<string, string> _dict; // 成员变量：用于存储数据的字典
    int _n = 0; // 成员变量：辅助整型（示例用）

    static A _inst; // 声明静态成员变量，这是饿汉模式的关键，它属于类而非对象
};

// 饿汉模式静态成员初始化
// 在程序入口 main 函数执行之前，这个对象就已经被创建好了
A A::_inst;

// -------------------------
// 类 B: 懒汉模式 (带GC回收)
// -------------------------
// 特点：第一次用的时候再创建（现吃现做），解决了启动慢的问题
// 注意：new 出来的懒汉对象一般不需要手动释放，进程正常结束操作系统会回收资源
// 但如果需要在析构时做持久化（写文件）等操作，则需要利用内部 GC 类来实现自动析构
class B
{
public:
    // 获取单例实例的静态方法
    static B* GetInstance()
    {
       // 判断是否是第一次调用
       if (_inst == nullptr)
       {
          _inst = new B; // 如果为空，则在堆上动态分配一个 B 对象
       }
       return _inst; // 返回实例指针
    }

    // 业务函数：添加数据
    void Add(const string& key, const string& value)
    {
       _dict[key] = value; // 插入数据到 map
    }

    // 业务函数：打印数据
    void Print()
    {
       for (auto& kv : _dict) // 遍历 map
       {
          cout << kv.first << ":" << kv.second << endl; // 打印键值对
       }
       cout << endl; // 换行
    }

    // 静态函数：用于手动释放单例实例
    static void DelInstance()
    {
       if (_inst) // 如果实例存在
       {
          delete _inst; // 释放堆内存，这会调用 B 的析构函数
          _inst = nullptr; // 指针置空，防止野指针
       }
    }

private:
    // 私有化构造函数
    B()
    {}

    // 析构函数
    ~B()
    {
       // 模拟持久化操作：当对象被销毁时，将数据写入文件或数据库
       cout << "[B析构] 数据写到文件(模拟持久化)" << endl;
    }

    // 禁用拷贝构造，保证唯一性
    B(const B& aa) = delete;
    // 禁用赋值运算，保证唯一性
    B& operator=(const B& aa) = delete;

    map<string, string> _dict; // 数据存储容器
    int _n = 0; // 辅助变量

    static B* _inst; // 声明静态指针，初始化为 nullptr

    // 定义内部类 GC (Garbage Collector)，用于自动回收资源
    class gc
    {
    public:
       // GC 类的析构函数
       ~gc()
       {
          DelInstance(); // 当 GC 对象销毁时，自动调用 B 的销毁函数
       }
    };

    static gc _gc; // 声明一个静态的 GC 成员变量
};

// 懒汉模式静态指针初始化为空
B* B::_inst = nullptr;
// 定义静态 GC 对象。
// 原理：当程序结束（main函数返回）时，静态区/全局区的对象（包括 _gc）会自动调用析构函数。
// _gc 的析构函数中调用了 B::DelInstance()，从而实现了 B 的自动释放和持久化逻辑。
B::gc B::_gc;


// ==========================================================================
//                       Part 2: IO流、类型转换与序列化
// ==========================================================================

// 类型转换测试类 C
class C
{
public:
    // 构造函数，可以用 int 初始化 C 对象
    // 这允许隐式类型转换：C c = 10;
    C(int x)
    {}
};

// 类型转换测试类 D
class D
{
public:
    // 构造函数，可以用 C 对象初始化 D 对象
    // 这允许隐式类型转换：D d = c_obj;
    D(const C& c)
    {}
};

// 类型转换测试类 E
class E
{
public:
    // 类型转换运算符重载
    // 允许将 E 类型的对象隐式转换为 int 类型
    operator int()
    {
       return 0; // 返回转换后的整数值
    }
};

// Date 类 (重载 IO 运算符)
class Date
{
    // 声明友元函数，允许 operator<< 和 operator>> 访问 Date 的私有成员 (_year 等)
    friend ostream& operator << (ostream& out, const Date& d);
    friend istream& operator >> (istream& in, Date& d);
public:
    // 构造函数，带默认参数
    Date(int year = 1, int month = 1, int day = 1)
       :_year(year)   // 初始化列表初始化 _year
       , _month(month) // 初始化列表初始化 _month
       , _day(day)     // 初始化列表初始化 _day
    {}

    // bool 类型转换运算符重载
    // 允许对象直接用于条件判断，如 while(date_obj)
    operator bool()
    {
       // 逻辑定义：如果 _year 为 0，则视为 false (无效日期)，用于结束输入循环
       if (_year == 0)
          return false;
       else
          return true;
    }
private:
    int _year;  // 年
    int _month; // 月
    int _day;   // 日
};

// 重载输入流运算符 >>
// 使得可以使用 cin >> date_obj; 这种方式
istream& operator >> (istream& in, Date& d)
{
    in >> d._year >> d._month >> d._day; // 依次从输入流读取年、月、日
    return in; // 返回输入流引用，支持连续输入 (cin >> d1 >> d2)
}

// 重载输出流运算符 <<
// 使得可以使用 cout << date_obj; 这种方式
ostream& operator << (ostream& out, const Date& d)
{
    out << d._year << " " << d._month << " " << d._day; // 依次向输出流写入年、月、日
    return out; // 返回输出流引用，支持连续输出 (cout << d1 << endl)
}

// 文件读写辅助结构体
struct ServerInfo
{
    string _address; // 服务器地址
    double _x;       // 某个浮点数据
    Date _date;      // 日期对象
};

// 1. 二进制读写工具类
class BinIO
{
public:
    // 构造函数，设置默认文件名为 "info.bin"
    BinIO(const char* filename = "info.bin")
       :_filename(filename) // 初始化文件名
    {}

    // 写二进制文件的方法
    void Write(const ServerInfo& winfo)
    {
       // 创建输出文件流，模式为：输出(out) | 二进制(binary)
       ofstream ofs(_filename, ofstream::out | ofstream::binary);

       // write 接收 char* 指针和字节数，将内存块直接写入文件
       // 【警告】：这里直接写入包含 std::string 的结构体是非常危险的。
       // std::string 内部包含指针，直接写入只会写入指针地址而非字符串内容（浅拷贝）。
       // 重新读取时，指针指向的内存可能已无效。此处仅为演示二进制写接口的用法。
       ofs.write((char*)&winfo, sizeof(winfo));
    }

    // 读二进制文件的方法
    void Read(ServerInfo& rinfo)
    {
       // 创建输入文件流，模式为：输入(in) | 二进制(binary)
       ifstream ifs(_filename, ofstream::in | ofstream::binary);
       // read 从文件读取指定字节数到内存
       ifs.read((char*)&rinfo, sizeof(rinfo));
    }

private:
    string _filename; // 文件名
};

// 2. 文本读写工具类
class TextIO
{
public:
    // 构造函数，设置默认文件名为 "info.text"
    TextIO(const char* filename = "info.text")
       :_filename(filename)
    {}

    // 写文本文件的方法
    void Write(const ServerInfo& winfo)
    {
       ofstream ofs(_filename); // 创建文本输出流
       // 利用重载的 << 运算符，将各成员格式化为文本写入，以换行符分隔
       ofs << winfo._address << endl;
       ofs << winfo._x << endl;
       ofs << winfo._date << endl; // 这里会调用 Date 类的 operator<<
    }

    // 读文本文件的方法
    void Read(ServerInfo& rinfo)
    {
       ifstream ifs(_filename); // 创建文本输入流
       // 利用重载的 >> 运算符，按顺序读取数据
       ifs >> rinfo._address;
       ifs >> rinfo._x;
       ifs >> rinfo._date; // 这里会调用 Date 类的 operator>>
    }

private:
    string _filename; // 文件名
};

// 序列化测试结构 (模拟聊天信息)
struct ChatInfo
{
    string _name; // 名字
    int _id;      // 用户ID
    Date _date;   // 发送时间
    string _msg;  // 聊天内容
};

int main()
{
    // ---------------------------------------------------------
    // 1. 测试单例模式
    // ---------------------------------------------------------
    { // 使用大括号创建局部作用域，隔离变量
       cout << "=== 1. 单例模式测试 ===" << endl; // 打印提示信息

       // 获取饿汉式单例 A 的实例并添加数据
       A::GetInstance()->Add("sort", "排序");
       A::GetInstance()->Add("left", "左边");
       A::GetInstance()->Add("right", "右边");
       A::GetInstance()->Print(); // 打印 A 的数据

       // 获取懒汉式单例 B 的实例（第一次调用时创建）并添加数据
       B::GetInstance()->Add("sort", "排序");
       B::GetInstance()->Add("left", "左边");
       B::GetInstance()->Add("right", "右边");
       B::GetInstance()->Print(); // 打印 B 的数据

       // 此处不需要手动 delete B，因为 B 类中有静态成员 _gc
       // 当 main 函数结束时，_gc 析构会自动清理 B 的实例
       cout << endl; // 换行
    }

    // ---------------------------------------------------------
    // 2. 测试类型转换
    // ---------------------------------------------------------
    { // 新的作用域
       cout << "=== 2. 类型转换测试 ===" << endl;

       // 演示：自定义类型 <- 内置类型
       // 调用 C(int x) 构造函数，将整数 11 隐式转换为 C 对象
       C c1 = 11;

       // 演示：自定义类型 <- 自定义类型
       // 调用 D(const C& c) 构造函数，将 c1 转换为 D 对象
       D d = c1;

       // 演示：内置类型 <- 自定义类型
       E e; // 创建 E 对象
       // 调用 E::operator int()，将对象 e 转换为整数并赋值给 x
       int x = e;
       cout << "E converted to int: " << x << endl << endl; // 打印转换结果
    }

    // ---------------------------------------------------------
    // 3. 测试文件 IO (BinIO & TextIO)
    // ---------------------------------------------------------
    { // 新的作用域
       cout << "=== 3. 文件读写测试 ===" << endl;
       // 初始化一个 ServerInfo 结构体数据
       ServerInfo winfo = { "https://legacy.cplusplus.com", 12.13, { 2026, 1, 10 } };

       // BinIO (二进制读写) 测试
       BinIO bin; // 创建二进制读写工具对象
       bin.Write(winfo); // 将 winfo 写入文件 "info.bin"
       // 注：此处省略读取测试，因为 ServerInfo 含 string，二进制直接读写不安全

       // TextIO (文本读写) 测试
       TextIO text; // 创建文本读写工具对象
       text.Write(winfo); // 将 winfo 格式化写入 "info.text"

       ServerInfo rinfoText; // 创建空对象用于接收读取的数据
       text.Read(rinfoText); // 从文本文件读取数据到 rinfoText

       // 打印读取到的结果，验证正确性
       cout << "TextIO 读取结果: " << rinfoText._address << " | " << rinfoText._date << endl;
       cout << endl;
    }

    // ---------------------------------------------------------
    // 4. 测试 StringStream 与 序列化
    // ---------------------------------------------------------
    { // 新的作用域
       cout << "=== 4. 序列化与反序列化测试 ===" << endl;

       // 场景1：利用 stringstream 拼接 SQL 语句
       Date d(2026, 1, 19); // 创建日期对象
       ostringstream sqloss; // 创建输出字符串流
       // 向流中像 cout 一样写入文本，自动将 d 转换为字符串
       sqloss << "select * from t_data where date = '" << d << "'";
       // .str() 获取流中拼接好的字符串
       cout << "SQL: " << sqloss.str() << endl;

       // 场景2：利用 stringstream 进行结构体序列化（转为字符串）和反序列化（转回对象）
       ChatInfo winfo = { "张三", 135246, { 2024, 4, 10 }, "晚上一起看电影吧" };
       stringstream oss; // stringstream 既可以读也可以写

       // --- 序列化过程 ---
       oss << winfo._name << endl; // 写入名字 + 换行
       oss << winfo._id << endl;   // 写入 ID + 换行
       oss << winfo._date << endl; // 写入日期 + 换行
       oss << winfo._msg << endl;  // 写入消息 + 换行
       cout << "序列化数据流:\n" << oss.str(); // 打印序列化后的整个字符串块

       // --- 反序列化过程 ---
       ChatInfo rinfo; // 准备接收数据的对象
       string str = oss.str(); // 获取刚才序列化的字符串
       stringstream iss(str);  // 用这个字符串初始化一个新的流（或者重置 oss）

       iss >> rinfo._name; // 从流中提取名字
       iss >> rinfo._id;   // 提取 ID
       iss >> rinfo._date; // 提取日期（调用 Date 的 >>）
       iss >> rinfo._msg;  // 提取消息

       // 打印反序列化还原的数据
       cout << "反序列化结果: " << rinfo._name << ": " << rinfo._msg << endl << endl;
    }

    // ---------------------------------------------------------
    // 5. 测试标准输入流 (Date交互)
    // ---------------------------------------------------------
    // 注意：此部分需要用户输入，为了演示流程放置在最后
    {
       cout << "=== 5. 标准IO交互测试 ===" << endl;
       cout << "请输入日期 (例如: 2023 5 20)，输入 0 0 0 结束:" << endl;

       Date d; // 创建临时日期对象
       // 循环条件解析：
       // 1. cin >> d : 从键盘读取输入，如果输入格式错误流会置错，返回 false
       // 2. && d     : 调用 Date::operator bool()，检查 _year 是否为 0
       while (cin >> d && d)
       {
          cout << "您输入的日期是: " << d << endl; // 回显输入的日期
          cout << "请继续输入 (0 0 0 退出): ";      // 提示继续
       }
    }

    return 0;
}