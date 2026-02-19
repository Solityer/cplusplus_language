#pragma once
#include <assert.h>
#include <iostream>

// 通用加法
template <class T>
T Add(T left, T right)
{
    return left + right;
}

template <class T>
class Vector
{
public:
    Vector(size_t capacity = 10)
        : _pData(new T[capacity])
        , _size(0)
        , _capacity(capacity)
    {}

    // 析构函数声明
    ~Vector();

    void PushBack(const T& data)
    {
        // 简单实现：暂不包含动态扩容逻辑
        // 在实际工程中应在此处检查 _size == _capacity 并调用 reserve
        if (_size < _capacity)
        {
            _pData[_size++] = data;
        }
        else
        {
            // 简单的错误提示，实际应扩容
            std::cerr << "Vector Capacity Full!" << std::endl;
        }
    }

    size_t Size()
    {
        return _size;
    }

    T& operator[](size_t pos)
    {
        assert(pos < _size);
        return _pData[pos];
    }

   private:
    T* _pData;
    size_t _size;
    size_t _capacity;
};

// 析构函数类外实现
template <class T>
Vector<T>::~Vector()
{
    if (_pData)
    {
        delete[] _pData;
        _pData = nullptr;
    }
    _size = 0;
    _capacity = 0;
}