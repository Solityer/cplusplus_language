#pragma once
#include <algorithm>
#include <cassert>
#include <iostream>
#include <string>
#include <vector>

namespace pzh
{
    template <class T>
    class vector
    {
    public:
        // 类型定义
        typedef T* iterator;
        typedef const T* const_iterator;

        // =========================================================
        // 迭代器接口 (Iterator Interface)
        // =========================================================
        iterator begin()
        {
            return _start;
        }

        iterator end()
        {
            return _finish;
        }

        const_iterator begin() const
        {
            return _start;
        }

        const_iterator end() const
        {
            return _finish;
        }

        // =========================================================
        // 构造与析构 (Construction & Destruction)
        // =========================================================

        // 默认构造
        vector() 
            : _start(nullptr)
            , _finish(nullptr)
            , _end_of_storage(nullptr)
        {}

        // 迭代器区间构造
        template <class InputIterator>
        vector(InputIterator first, InputIterator last)
            : _start(nullptr)
            , _finish(nullptr)
            , _end_of_storage(nullptr)
        {
            // 考虑效率，若已知距离可提前 reserve，但 InputIterator 不一定支持断言距离
            while (first != last)
            {
                push_back(*first);
                ++first;
            }
        }

        // size_t 初始化构造
        vector(size_t n, const T& val = T())
            : _start(nullptr)
            , _finish(nullptr)
            , _end_of_storage(nullptr)
        {
            reserve(n);
            for (size_t i = 0; i < n; i++)
            {
                push_back(val);
            }
        }

        // int 初始化构造
        vector(int n, const T& val = T())
            : _start(nullptr)
            , _finish(nullptr)
            , _end_of_storage(nullptr)
        {
            reserve(n);
            for (int i = 0; i < n; i++)
            {
                push_back(val);
            }
        }

        // 拷贝构造 (Deep Copy)
        vector(const vector<T>& v)
            : _start(nullptr)
            , _finish(nullptr)
            , _end_of_storage(nullptr)
        {
            reserve(v.capacity());
            for (const auto& e : v)
            {
                push_back(e);
            }
        }

        // 赋值重载 (Copy-and-Swap idiom)
        // 传值传参触发拷贝构造，复用 swap 实现深拷贝
        vector<T>& operator=(vector<T> tmp)
        {
            swap(tmp);
            return *this;
        }

        // 析构函数
        ~vector()
        {
            if (_start)
            {
                delete[] _start;
                _start = _finish = _end_of_storage = nullptr;
            }
        }

        // =========================================================
        // 容量操作 (Capacity Operations)
        // =========================================================

        size_t size() const
        {
            return _finish - _start;
        }

        size_t capacity() const
        {
            return _end_of_storage - _start;
        }

        bool empty() const
        {
            return _start == _finish;
        }

        // 预留空间
        // 核心逻辑：开辟新空间 -> 元素赋值(避免浅拷贝) -> 释放旧空间 -> 更新指针
        void reserve(size_t n)
        {
            if (n > capacity())
            {
                size_t old_size = size();
                T* tmp = new T[n];

                if (_start)
                {
                    // 使用循环赋值而非 memcpy，确保自定义类型(如 std::string)执行深拷贝
                    for (size_t i = 0; i < old_size; i++)
                    {
                        tmp[i] = _start[i];
                    }
                    delete[] _start;
                }

                _start = tmp;
                _finish = _start + old_size;
                _end_of_storage = _start + n;
            }
        }

        // 调整大小
        void resize(size_t n, const T& val = T())
        {
            if (n < size())
            {
                _finish = _start + n;
            }
            else
            {
                reserve(n);
                while (_finish != _start + n)
                {
                    *_finish = val;
                    ++_finish;
                }
            }
        }

        // =========================================================
        // 元素访问 (Element Access)
        // =========================================================

        T& operator[](size_t pos)
        {
            assert(pos < size());
            return _start[pos];
        }

        const T& operator[](size_t pos) const
        {
            assert(pos < size());
            return _start[pos];
        }

        T& front()
        {
            assert(!empty());
            return *_start;
        }

        const T& front() const
        {
            assert(!empty());
            return *_start;
        }

        T& back()
        {
            assert(!empty());
            return *(_finish - 1);
        }

        const T& back() const
        {
            assert(!empty());
            return *(_finish - 1);
        }

        // =========================================================
        // 修改器 (Modifiers)
        // =========================================================

        void push_back(const T& x)
        {
            // 扩容逻辑集成在 insert 中，此处直接复用
            insert(end(), x);
        }

        void pop_back()
        {
            assert(!empty());
            --_finish;
        }

        void swap(vector<T>& v)
        {
            std::swap(_start, v._start);
            std::swap(_finish, v._finish);
            std::swap(_end_of_storage, v._end_of_storage);
        }

        // 任意位置插入
        // 返回指向新插入元素的迭代器
        iterator insert(iterator pos, const T& x)
        {
            assert(pos >= _start);
            assert(pos <= _finish);

            // 检查扩容
            if (_finish == _end_of_storage)
            {
                size_t len = pos - _start;  // 记录相对偏移量
                size_t newCapacity = capacity() == 0 ? 4 : capacity() * 2;
                reserve(newCapacity);

                // 扩容后旧空间被释放，需要更新 pos，否则会导致迭代器失效
                pos = _start + len;
            }

            // 元素挪动
            iterator end_it = _finish - 1;
            while (end_it >= pos)
            {
                *(end_it + 1) = *end_it;
                --end_it;
            }

            *pos = x;
            ++_finish;
            return pos;
        }

        // 任意位置删除
        // 返回被删除元素之后位置的迭代器
        iterator erase(iterator pos)
        {
            assert(pos >= _start);
            assert(pos < _finish);

            iterator it = pos + 1;
            while (it < _finish)
            {
                *(it - 1) = *it;
                ++it;
            }

            --_finish;
            return pos;
        }

    private:
        iterator _start = nullptr;
        iterator _finish = nullptr;
        iterator _end_of_storage = nullptr;
    };
}