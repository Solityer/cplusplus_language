#pragma once
#include <assert.h>

#include <algorithm>  // for std::swap
#include <cstring>
#include <iterator>  // for std::reverse_iterator

namespace pzh
{
/**
 * @brief 自定义的字符串类，模拟 C++ 标准库中的 std::string
 *
 * 该类实现了动态字符串管理，包括构造、析构、赋值、迭代器、
 * 容量操作、元素访问、修改器、字符串操作和运算符重载。
 */
class string
{
   public:
    // 类型定义
    typedef char* iterator;                                    // 正向迭代器类型
    typedef const char* const_iterator;                        // 常量正向迭代器类型
    typedef std::reverse_iterator<iterator> reverse_iterator;  // 反向迭代器类型
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;  // 常量反向迭代器类型
    static const size_t npos;  // 静态常量，表示“未找到”或“直到末尾”的特殊值

    /* ========================================================================
     * 1. 构造、析构与赋值 (Canonical Form)
     * ========================================================================
     */
   public:
    /**
     * @brief 构造函数：从 C 风格字符串构造
     * @param str 初始字符串，默认为空串
     */
    string(const char* str = "")
        : _size(strlen(str))  // 计算字符串长度
        , _capacity(_size)    // 容量初始化为大小（无额外空间）
    {
        _str = new char[_capacity + 1];  // 分配内存（+1 用于存储 '\0'）
        strcpy(_str, str);               // 复制字符串内容
    }

    /**
     * @brief 构造函数：构造包含 n 个字符 ch 的字符串
     * @param n  字符个数
     * @param ch 要重复的字符
     */
    string(size_t n, char ch) : _size(n), _capacity(n)
    {
        _str = new char[_capacity + 1];
        for (size_t i = 0; i < n; ++i)
            _str[i] = ch;
        _str[n] = '\0';  // 添加字符串结束符
    }

    /**
     * @brief 构造函数：从另一个字符串的子串构造
     * @param str 源字符串
     * @param pos 开始位置
     * @param len 要复制的长度，默认为 npos（表示直到源字符串末尾）
     */
    string(const string& str, size_t pos, size_t len = npos)
    {
        assert(pos <= str._size);  // 确保pos不越界，允许 pos == size（空串）

        // 确定实际要复制的长度
        size_t copy_len = len;
        if (len == npos || pos + len > str._size)
        {
            copy_len = str._size - pos;  // 复制到末尾
        }

        _size = copy_len;
        _capacity = _size;
        _str = new char[_capacity + 1];
        // 使用 strncpy 复制指定长度的字符
        strncpy(_str, str._str + pos, copy_len);
        _str[_size] = '\0';  // 手动添加结束符（strncpy 可能不添加）
    }

    /**
     * @brief 拷贝构造函数
     * @param s 要复制的源字符串
     */
    string(const string& s)
        : _size(s._size)
        , _capacity(s._capacity)
    {
        _str = new char[_capacity + 1];
        strcpy(_str, s._str);  // 复制整个字符串内容
    }

    /**
     * @brief 赋值运算符重载（使用拷贝交换技巧）
     * @param tmp 通过值传递的临时对象（已经拷贝了源对象）
     * @return    *this 的引用
     *
     * 注意：参数使用值传递，会自动调用拷贝构造或移动构造。
     * 然后通过 swap 交换当前对象和临时对象的内容。
     * 临时对象在函数结束时自动析构，释放原来的资源。
     */
    string& operator=(string tmp)
    {
        swap(tmp);
        return *this;
    }

    /**
     * @brief 析构函数：释放动态分配的内存
     */
    ~string()
    {
        if (_str)
        {
            delete[] _str;
            _str = nullptr;
            _size = _capacity = 0;
        }
    }

    /* ========================================================================
     * 2. 迭代器 (Iterators)
     * ========================================================================
     */
    iterator begin()
    {
        return _str;  // 返回指向第一个字符的迭代器
    }

    iterator end()
    {
        return _str + _size;  // 返回指向末尾字符之后位置的迭代器
    }

    const_iterator begin() const
    {
        return _str;  // 常量版本
    }

    const_iterator end() const
    {
        return _str + _size;  // 常量版本
    }

    // 反向迭代器适配
    reverse_iterator rbegin()
    {
        return reverse_iterator(end());  // 返回指向最后一个字符的反向迭代器
    }

    reverse_iterator rend()
    {
        return reverse_iterator(begin());  // 返回指向第一个字符之前的反向迭代器
    }

    const_reverse_iterator rbegin() const
    {
        return const_reverse_iterator(end());  // 常量版本
    }

    const_reverse_iterator rend() const
    {
        return const_reverse_iterator(begin());  // 常量版本
    }

    /* ========================================================================
     * 3. 容量操作 (Capacity)
     * ========================================================================
     */
    size_t size() const
    {
        return _size;
    }  // 返回当前字符串长度

    size_t length() const
    {
        return _size;
    }  // 返回长度

    size_t max_size() const
    {
        return (size_t)-1;
    }  // 理论最大长度（size_t 最大值）

    size_t capacity() const
    {
        return _capacity;
    }  // 返回当前分配的存储容量

    const char* c_str() const
    {
        return _str;
    }  // 返回 C 风格字符串指针

    /**
     * @brief 清空字符串内容
     */
    void clear()
    {
        _size = 0;
        _str[0] = '\0';  // 只设置结束符，不释放内存
    }

    /**
     * @brief 交换两个字符串对象的内容
     * @param s 要交换的另一个字符串
     */
    void swap(string& s)
    {
        std::swap(_str, s._str);
        std::swap(_size, s._size);
        std::swap(_capacity, s._capacity);
    }

    /**
     * @brief 预留存储空间（不改变字符串大小）
     * @param n 要预留的容量
     */
    void reserve(size_t n)
    {
        if (n > _capacity)  // 只在 n 大于当前容量时扩容
        {
            char* tmp = new char[n + 1];
            strcpy(tmp, _str);
            delete[] _str;
            _str = tmp;
            _capacity = n;
        }
        // 如果 n <= _capacity，不做任何操作（不缩容）
    }

    /**
     * @brief 调整字符串大小
     * @param n  新的大小
     * @param ch 当需要扩展时，用来填充新增字符的字符，默认为 '\0'
     */
    void resize(size_t n, char ch = '\0')
    {
        if (n <= _size)  // 缩小
        {
            _size = n;
            _str[_size] = '\0';  // 截断
        }
        else  // 扩大
        {
            reserve(n);  // 确保容量足够
            // 从当前末尾开始填充
            while (_size < n)
            {
                _str[_size] = ch;
                _size++;
            }
            _str[_size] = '\0';
        }
    }

    /* ========================================================================
     * 4. 元素访问 (Element Access)
     * ========================================================================
     */
    /**
     * @brief 下标运算符重载
     * @param pos 位置索引
     * @return    该位置的字符引用
     */
    char& operator[](size_t pos)
    {
        assert(pos < _size);  // 确保下标不越界
        return _str[pos];
    }

    /**
     * @brief 下标运算符重载（只读版本）
     * @param pos 位置索引
     * @return    该位置的字符常量引用
     */
    const char& operator[](size_t pos) const
    {
        assert(pos < _size);
        return _str[pos];
    }

    /* ========================================================================
     * 5. 修改器 (Modifiers)
     * ========================================================================
     */
    /**
     * @brief 在字符串末尾添加一个字符
     * @param ch 要添加的字符
     */
    void push_back(char ch)
    {
        if (_size == _capacity)  // 如果容量已满，扩容
        {
            // 扩容策略：初始为 0 时分配 4，否则翻倍
            reserve(_capacity == 0 ? 4 : _capacity * 2);
        }
        _str[_size] = ch;
        _size++;
        _str[_size] = '\0';  // 保持结束符
    }

    /**
     * @brief 在字符串末尾追加一个 C 风格字符串
     * @param str 要追加的字符串
     */
    void append(const char* str)
    {
        size_t len = strlen(str);
        if (_size + len > _capacity)  // 需要扩容
        {
            reserve(_size + len);
        }
        strcpy(_str + _size, str);  // 从当前末尾开始复制
        _size += len;
    }

    /**
     * @brief 运算符 += 重载（追加字符）
     * @param ch 要追加的字符
     * @return   *this 的引用
     */
    string& operator+=(char ch)
    {
        push_back(ch);
        return *this;
    }

    /**
     * @brief 运算符 += 重载（追加字符串）
     * @param str 要追加的字符串
     * @return   *this 的引用
     */
    string& operator+=(const char* str)
    {
        append(str);
        return *this;
    }

    /**
     * @brief 在指定位置插入字符串
     * @param pos 插入位置
     * @param str 要插入的字符串
     * @return    *this 的引用
     */
    string& insert(size_t pos, const char* str)
    {
        assert(pos <= _size);  // 允许在末尾插入
        size_t len = strlen(str);

        // 如果容量不足，扩容
        if (_size + len > _capacity)
        {
            reserve(_size + len);
        }

        // 从后向前移动字符，为新插入的字符腾出空间
        size_t end = _size + len;
        // 当 end 大于 pos+len-1 时，继续移动
        while (end > pos + len - 1)
        {
            _str[end] = _str[end - len];
            --end;
        }
        // 插入新字符
        strncpy(_str + pos, str, len);
        _size += len;
        return *this;
    }

    /**
     * @brief 在指定位置插入 n 个字符 ch
     * @param pos 插入位置
     * @param n   要插入的字符个数
     * @param ch  要插入的字符
     * @return    *this 的引用
     */
    string& insert(size_t pos, size_t n, char ch)
    {
        assert(pos <= _size);
        if (_size + n > _capacity)
        {
            reserve(_size + n);
        }

        // 后移字符
        size_t end = _size + n;
        while (end > pos + n - 1)
        {
            _str[end] = _str[end - n];
            --end;
        }
        // 填充字符
        for (size_t i = 0; i < n; ++i)
        {
            _str[pos + i] = ch;
        }
        _size += n;
        return *this;
    }

    /**
     * @brief 从指定位置删除字符
     * @param pos 起始删除位置，默认为 0
     * @param len 要删除的长度，默认为 npos（表示删除到末尾）
     * @return    *this 的引用
     */
    string& erase(size_t pos, size_t len = npos)
    {
        assert(pos <= _size);
        if (len == npos || pos + len >= _size)  // 删除到末尾
        {
            _str[pos] = '\0';
            _size = pos;
        }
        else  // 删除中间部分
        {
            size_t begin = pos + len;
            // 将后面的字符向前移动
            while (begin <= _size)
            {
                _str[begin - len] = _str[begin];
                ++begin;
            }
            _size -= len;
        }
        return *this;
    }

    /**
     * @brief 删除迭代器指向的字符
     * @param p 指向要删除字符的迭代器
     * @return  指向被删除字符之后位置的迭代器
     */
    iterator erase(iterator p)
    {
        size_t pos = p - _str;  // 计算索引
        erase(pos, 1);          // 调用索引版本删除
        return _str + pos;      // 返回删除后的位置
    }

    /**
     * @brief 替换子串
     * @param pos 起始位置
     * @param len 要替换的长度
     * @param str 替换字符串
     * @return    *this 的引用
     */
    string& replace(size_t pos, size_t len, const char* str)
    {
        assert(pos < _size);
        erase(pos, len);   // 先删除指定部分
        insert(pos, str);  // 再插入新字符串
        return *this;
    }

    /* ========================================================================
     * 6. 字符串操作 (String Operations)
     * ========================================================================
     */
    /**
     * @brief 查找字符
     * @param ch  要查找的字符
     * @param pos 开始查找的位置，默认为 0
     * @return    找到的位置，否则返回 npos
     */
    size_t find(char ch, size_t pos = 0) const
    {
        for (size_t i = pos; i < _size; ++i)
        {
            if (_str[i] == ch)
                return i;
        }
        return npos;
    }

    /**
     * @brief 查找子串
     * @param sub 要查找的子串
     * @param pos 开始查找的位置，默认为 0
     * @return    找到的位置，否则返回 npos
     */
    size_t find(const char* sub, size_t pos = 0) const
    {
        const char* ptr = strstr(_str + pos, sub);  // 使用 C 库函数查找
        if (ptr)
            return ptr - _str;  // 返回索引
        else
            return npos;
    }

    /**
     * @brief 查找第一个出现在指定字符集合中的字符
     * @param args 字符集合字符串
     * @param pos  开始查找的位置，默认为 0
     * @return     找到的位置，否则返回 npos
     */
    size_t find_first_of(const char* args, size_t pos = 0) const
    {
        if (pos >= _size)
            return npos;
        for (size_t i = pos; i < _size; ++i)
        {
            for (size_t j = 0; args[j] != '\0'; ++j)
            {
                if (_str[i] == args[j])
                    return i;
            }
        }
        return npos;
    }

    /**
     * @brief 提取子串
     * @param pos 起始位置
     * @param len 要提取的长度，默认为 npos（表示提取到末尾）
     * @return    提取出的子串
     */
    string substr(size_t pos, size_t len = npos) const
    {
        assert(pos < _size);
        size_t real_len = len;
        if (len == npos || pos + len > _size)
        {
            real_len = _size - pos;
        }

        string sub;
        sub.reserve(real_len);
        for (size_t i = 0; i < real_len; ++i)
        {
            sub += _str[pos + i];  // 逐个添加字符
        }
        return sub;
    }

    /* ========================================================================
     * 7. 运算符重载 (Operators)
     * ========================================================================
     */
    bool operator<(const string& s) const
    {
        return strcmp(_str, s._str) < 0;
    }

    bool operator==(const string& s) const
    {
        return strcmp(_str, s._str) == 0;
    }

    bool operator<=(const string& s) const
    {
        return *this < s || *this == s;
    }

    bool operator>(const string& s) const
    {
        return !(*this <= s);
    }

    bool operator>=(const string& s) const
    {
        return !(*this < s);
    }

    bool operator!=(const string& s) const
    {
        return !(*this == s);
    }

   private:
    char* _str;        ///< 指向动态分配的字符数组的指针
    size_t _size;      ///< 当前字符串的长度（不包括 '\0'）
    size_t _capacity;  ///< 当前分配的存储容量（不包括 '\0'）
};

// 静态常量定义
const size_t string::npos = -1;

/**
 * @brief 输出运算符重载
 * @param out 输出流
 * @param s   要输出的字符串
 * @return    输出流的引用
 */
inline std::ostream& operator<<(std::ostream& out, const string& s)
{
    for (auto ch : s)
        out << ch;
    return out;
}

/**
 * @brief 输入运算符重载（读取单词，跳过前导空白）
 * @param in  输入流
 * @param s   要存储输入的字符串
 * @return    输入流的引用
 *
 * 实现说明：
 * 1. 清空目标字符串
 * 2. 跳过前导空白字符
 * 3. 使用缓冲区读取，减少多次分配
 * 4. 遇到空白或 EOF 停止
 */
inline std::istream& operator>>(std::istream& in, string& s)
{
    s.clear();
    char ch;
    ch = in.get();
    // 跳过前导空白
    while (isspace(ch))
    {
        ch = in.get();
    }
    // 读取内容
    char buff[128];
    size_t i = 0;
    while (!isspace(ch) && ch != EOF)
    {
        buff[i++] = ch;
        if (i == 127)  // 缓冲区满
        {
            buff[i] = '\0';
            s += buff;
            i = 0;
        }
        ch = in.get();
    }
    if (i > 0)  // 剩余内容
    {
        buff[i] = '\0';
        s += buff;
    }
    return in;
}
}  // namespace pzh