#pragma once
#define _CRT_SECURE_NO_WARNINGS 
#include <iostream>
#include <assert.h>
#include <cstring>
#include <algorithm> // for std::swap
#include <iterator>  // for std::reverse_iterator

namespace pzh
{
    class string
    {
    public:
        // 类型定义 (Type Definitions)
        typedef char* iterator;
        typedef const char* const_iterator;
        typedef std::reverse_iterator<iterator> reverse_iterator;
        typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
        
        static const size_t npos; 

        /* ========================================================================
         * 1. 构造、析构与赋值 (Canonical Form)
         * ======================================================================== */
    public:
        string(const char* str = "")
            : _size(strlen(str))
            , _capacity(_size)
        {
            _str = new char[_capacity + 1];
            strcpy(_str, str);
        }

        string(size_t n, char ch)
            : _size(n)
            , _capacity(n)
        {
            _str = new char[_capacity + 1];
            for (size_t i = 0; i < n; ++i) _str[i] = ch;
            _str[n] = '\0';
        }

        string(const string& str, size_t pos, size_t len = npos)
        {
            assert(pos <= str._size); // 修正：允许 pos == size (即空串)
            size_t copy_len = len;
            if (len == npos || pos + len > str._size)
            {
                copy_len = str._size - pos;
            }

            _size = copy_len;
            _capacity = _size;
            _str = new char[_capacity + 1];
            strncpy(_str, str._str + pos, copy_len);
            _str[_size] = '\0';
        }

        string(const string& s)
            : _size(s._size)
            , _capacity(s._capacity)
        {
            _str = new char[_capacity + 1];
            strcpy(_str, s._str);
        }

        string& operator=(string tmp) 
        {
            swap(tmp);
            return *this;
        }

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
         * ======================================================================== */
        iterator begin() { return _str; }
        iterator end() { return _str + _size; }
        const_iterator begin() const { return _str; }
        const_iterator end() const { return _str + _size; }

        // 反向迭代器适配
        reverse_iterator rbegin() { return reverse_iterator(end()); }
        reverse_iterator rend() { return reverse_iterator(begin()); }
        const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }
        const_reverse_iterator rend() const { return const_reverse_iterator(begin()); }

        /* ========================================================================
         * 3. 容量操作 (Capacity)
         * ======================================================================== */
        size_t size() const { return _size; }
        size_t length() const { return _size; } // 同义词
        size_t max_size() const { return (size_t)-1; } // 理论最大值
        size_t capacity() const { return _capacity; }
        const char* c_str() const { return _str; }
        
        void clear()
        {
            _size = 0;
            _str[0] = '\0';
        }

        void swap(string& s)
        {
            std::swap(_str, s._str);
            std::swap(_size, s._size);
            std::swap(_capacity, s._capacity);
        }

        void reserve(size_t n)
        {
            if (n > _capacity)
            {
                char* tmp = new char[n + 1];
                strcpy(tmp, _str);
                delete[] _str;
                _str = tmp;
                _capacity = n;
            }
        }

        void resize(size_t n, char ch = '\0')
        {
            if (n <= _size)
            {
                _size = n;
                _str[_size] = '\0';
            }
            else
            {
                reserve(n);
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
         * ======================================================================== */
        char& operator[](size_t pos)
        {
            assert(pos < _size);
            return _str[pos];
        }

        const char& operator[](size_t pos) const
        {
            assert(pos < _size);
            return _str[pos];
        }

        /* ========================================================================
         * 5. 修改器 (Modifiers)
         * ======================================================================== */
        void push_back(char ch)
        {
            if (_size == _capacity)
            {
                reserve(_capacity == 0 ? 4 : _capacity * 2);
            }
            _str[_size] = ch;
            _size++;
            _str[_size] = '\0';
        }

        void append(const char* str)
        {
            size_t len = strlen(str);
            if (_size + len > _capacity)
            {
                reserve(_size + len);
            }
            strcpy(_str + _size, str);
            _size += len;
        }

        string& operator+=(char ch)
        {
            push_back(ch);
            return *this;
        }

        string& operator+=(const char* str)
        {
            append(str);
            return *this;
        }

        // Insert: pos 位置插入 str
        string& insert(size_t pos, const char* str)
        {
            assert(pos <= _size);
            size_t len = strlen(str);
            if (_size + len > _capacity)
            {
                reserve(_size + len);
            }

            size_t end = _size + len;
            while (end > pos + len - 1)
            {
                _str[end] = _str[end - len];
                --end;
            }
            strncpy(_str + pos, str, len);
            _size += len;
            return *this;
        }

        // Insert: pos 位置插入 n 个 ch
        string& insert(size_t pos, size_t n, char ch)
        {
            assert(pos <= _size);
            if (_size + n > _capacity)
            {
                reserve(_size + n);
            }

            size_t end = _size + n;
            while (end > pos + n - 1)
            {
                _str[end] = _str[end - n];
                --end;
            }
            for (size_t i = 0; i < n; ++i)
            {
                _str[pos + i] = ch;
            }
            _size += n;
            return *this;
        }

        // Erase: 从 pos 位置删除 len 个字符
        string& erase(size_t pos, size_t len = npos)
        {
            assert(pos <= _size); 
            if (len == npos || pos + len >= _size)
            {
                _str[pos] = '\0';
                _size = pos;
            }
            else
            {
                size_t begin = pos + len;
                while (begin <= _size) 
                {
                    _str[begin - len] = _str[begin];
                    ++begin;
                }
                _size -= len;
            }
            return *this;
        }

        // Erase Overload: 迭代器版本 (满足 main.cpp 测试需求)
        iterator erase(iterator p)
        {
            size_t pos = p - _str; // 指针相减获得索引
            erase(pos, 1);
            return _str + pos;
        }

        // Replace: 替换
        string& replace(size_t pos, size_t len, const char* str)
        {
            assert(pos < _size);
            erase(pos, len);
            insert(pos, str);
            return *this;
        }

        /* ========================================================================
         * 6. 字符串操作 (String Operations)
         * ======================================================================== */
        size_t find(char ch, size_t pos = 0) const
        {
            for (size_t i = pos; i < _size; ++i)
            {
                if (_str[i] == ch) return i;
            }
            return npos;
        }

        size_t find(const char* sub, size_t pos = 0) const
        {
            const char* ptr = strstr(_str + pos, sub);
            if (ptr)
                return ptr - _str;
            else
                return npos;
        }

        size_t find_first_of(const char* args, size_t pos = 0) const
        {
            if (pos >= _size) return npos;
            for (size_t i = pos; i < _size; ++i)
            {
                for (size_t j = 0; args[j] != '\0'; ++j)
                {
                    if (_str[i] == args[j]) return i;
                }
            }
            return npos;
        }

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
                sub += _str[pos + i];
            }
            return sub;
        }

        /* ========================================================================
         * 7. 运算符重载 (Operators)
         * ======================================================================== */
        bool operator<(const string& s) const { return strcmp(_str, s._str) < 0; }
        bool operator==(const string& s) const { return strcmp(_str, s._str) == 0; }
        bool operator<=(const string& s) const { return *this < s || *this == s; }
        bool operator>(const string& s) const { return !(*this <= s); }
        bool operator>=(const string& s) const { return !(*this < s); }
        bool operator!=(const string& s) const { return !(*this == s); }

    private:
        char* _str;
        size_t _size;
        size_t _capacity;
    };

    const size_t string::npos = -1;

    inline std::ostream& operator<<(std::ostream& out, const string& s)
    {
        for (auto ch : s) out << ch;
        return out;
    }

    inline std::istream& operator>>(std::istream& in, string& s)
    {
        s.clear();
        char ch;
        ch = in.get();
        // 1. 跳过前导空白
        while (isspace(ch))
        {
            ch = in.get();
        }
        // 2. 读取内容
        char buff[128];
        size_t i = 0;
        while (!isspace(ch) && ch != EOF)
        {
            buff[i++] = ch;
            if (i == 127)
            {
                buff[i] = '\0';
                s += buff;
                i = 0;
            }
            ch = in.get();
        }
        if (i > 0)
        {
            buff[i] = '\0';
            s += buff;
        }
        return in;
    }
}