#pragma once
#include <iostream>
#include <cstdlib> // for malloc/free, though new/delete is preferred in C++
#include <cstring> // for memcpy
#include <cstdio>  // for perror

typedef int DataType;

/**
 * @brief Stack 类
 * @details 该类主要用于演示“资源管理”和“深拷贝”原理。
 * 核心知识点：
 * 1. 动态内存管理（构造函数申请，析构函数释放）。
 * 2. 深拷贝（Deep Copy）vs 浅拷贝（Shallow Copy）。
 * 3. 防止内存泄漏与悬空指针。
 */
class Stack
{
public:
    // 构造函数
    explicit Stack(size_t capacity = 4)
        : _capacity(capacity), _size(0)
    {
        // 建议在C++中使用 new 替代 malloc，此处为了保持与原代码逻辑一致并演示底层内存操作
        _array = new DataType[capacity];
        // _array = (DataType *) malloc(sizeof(DataType) * capacity);
        if (nullptr == _array)
        {
            perror("malloc failed");
            exit(-1);
        }
    }

    // 析构函数 (Destructor)
    // 负责清理资源，防止内存泄漏
    ~Stack()
    {
        if (_array)
        {
            free(_array);
            // delete[] _array; // 如果使用 new[] 申请，则用 delete[]
            _array = nullptr;
            _capacity = _size = 0;
            std::cout << "Stack destroyed and memory freed." << std::endl;
        }
    }

    // 拷贝构造函数 (Copy Constructor)
    // 实现深拷贝的关键
    Stack(const Stack &s)
    {
        std::cout << "Stack(const Stack& s) - Deep Copying" << std::endl;
        // 1. 申请新空间
        _array = (DataType *) malloc(sizeof(DataType) * s._capacity);
        if (nullptr == _array)
        {
            perror("malloc failed");
            exit(-1);
        }
        // 2. 拷贝数据
        // 如果DataType是C++对象，应当使用循环赋值而不是memcpy，因为memcpy是浅拷贝行为
        // 这里假设DataType是int，memcpy是安全的
        memcpy(_array, s._array, sizeof(DataType) * s._size);

        // 3. 拷贝属性
        _size = s._size;
        _capacity = s._capacity;
    }

    // 赋值运算符 (Assignment Operator)
    // 同样需要深拷贝
    Stack &operator=(const Stack &s)
    {
        if (this != &s)
        {
            // 简单实现：先释放旧空间，再申请新空间
            free(_array);

            _array = (DataType *) malloc(sizeof(DataType) * s._capacity);
            memcpy(_array, s._array, sizeof(DataType) * s._size);
            _size = s._size;
            _capacity = s._capacity;
        }
        return *this;
    }

    void Push(DataType x)
    {
        if (_size == _capacity)
        {
            // 扩容逻辑省略，仅做演示
            std::cout << "Stack full" << std::endl;
            return;
        }
        _array[_size++] = x;
    }

    void Print() const
    {
        for (int i = 0; i < _size; ++i)
        {
            std::cout << _array[i] << " ";
        }
        std::cout << std::endl;
    }

private:
    DataType *_array;
    int _size;
    int _capacity;
};
