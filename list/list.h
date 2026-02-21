#pragma once
#include <iostream>
#include <string>

namespace pzh
{
// ---------------------------------------------------------
// 1. 节点定义 (Node Definition)
// ---------------------------------------------------------
template <class T>
struct list_node
{
    T _data;
    list_node<T>* _next;
    list_node<T>* _prev;

    // 匿名对象初始化机制，支持泛型默认构造
    // T不一定是int，需要给个匿名对象
    // 匿名对象是指不具名的临时对象，它的生命周期通常仅限于创建它的表达式
    list_node(const T& x = T())  // 构造函数，参数为const T&，默认值为T类型的默认构造值
        : _data(x)
        , _next(nullptr)
        , _prev(nullptr)
    {}
};

// ---------------------------------------------------------
// 2. 迭代器实现 (Iterator Implementations)
// ---------------------------------------------------------
// 多模板参数泛型迭代器
template <class T, class Ref, class Ptr>
struct __list_iterator
{
    typedef list_node<T> Node;
    typedef __list_iterator<T, Ref, Ptr> self;
    Node* _node;

    __list_iterator(Node* node)
        : _node(node)
    {}

    self& operator++()
    {
        _node = _node->_next;
        return *this;
    }

    self& operator--()
    {
        _node = _node->_prev;
        return *this;
    }

    self operator++(int)
    {
        self tmp(*this);
        _node = _node->_next;
        return tmp;
    }

    self operator--(int)
    {
        self tmp(*this);
        _node = _node->_prev;
        return tmp;
    }

    Ref operator*()
    {
        return _node->_data;
    }

    Ptr operator->()
    {
        return &_node->_data;
    }

    bool operator!=(const self& s) const
    {
        return _node != s._node;
    }

    bool operator==(const self& s) const
    {
        return _node == s._node;
    }
};

// 独立的常量迭代器
template <class T>
struct __list_const_iterator
{
    typedef list_node<T> Node;
    typedef __list_const_iterator<T> self;
    Node* _node;

    __list_const_iterator(Node* node)
        : _node(node)
    {}

    self& operator++()
    {
        _node = _node->_next;
        return *this;
    }

    self& operator--()
    {
        _node = _node->_prev;
        return *this;
    }

    self operator++(int)
    {
        self tmp(*this);
        _node = _node->_next;
        return tmp;
    }

    self operator--(int)
    {
        self tmp(*this);
        _node = _node->_prev;
        return tmp;
    }

    const T& operator*()
    {
        return _node->_data;
    }

    const T* operator->()
    {
        return &_node->_data;
    }

    bool operator!=(const self& s) const
    {
        return _node != s._node;
    }

    bool operator==(const self& s) const
    {
        return _node == s._node;
    }
};

// ---------------------------------------------------------
// 3. 链表主类 (List Class Definition)
// ---------------------------------------------------------
template <class T>
class list
{
    typedef list_node<T> Node;

   public:
    typedef __list_iterator<T, T&, T*> iterator;
    typedef __list_iterator<T, const T&, const T*> const_iterator;

    iterator begin()
    {
        return _head->_next;
    }

    iterator end()
    {
        return _head;
    }

    const_iterator begin() const
    {
        return const_iterator(_head->_next);
    }

    const_iterator end() const
    {
        return const_iterator(_head);
    }

    void empty_init()
    {
        _head = new Node;
        _head->_next = _head;
        _head->_prev = _head;
        _size = 0;
    }

    list()
    {
        empty_init();
    }

    list(const list<T>& lt)
    {
        empty_init();
        for (auto e : lt)
        {
            push_back(e);
        }
    }

    void swap(list<T>& lt)
    {
        std::swap(_head, lt._head);
        std::swap(_size, lt._size);
    }

    // 赋值运算符重载（已修正：将 list<int> 纠正为泛型 list<T> 以支持任意类型拷贝）
    list<T>& operator=(list<T> lt)
    {
        swap(lt);
        return *this;
    }

    ~list()
    {
        clear();
        delete _head;
        _head = nullptr;
    }

    void clear()
    {
        iterator it = begin();
        while (it != end())
        {
            it = erase(it);
        }
    }

    void push_back(const T& x)
    {
        insert(end(), x);
    }

    void push_front(const T& x)
    {
        insert(begin(), x);
    }

    void pop_front()
    {
        erase(begin());
    }

    void pop_back()
    {
        erase(--end());
    }

    iterator insert(iterator pos, const T& x)
    {
        Node* cur = pos._node;
        Node* newnode = new Node(x);
        Node* prev = cur->_prev;

        prev->_next = newnode;
        newnode->_prev = prev;
        newnode->_next = cur;
        cur->_prev = newnode;

        ++_size;
        return iterator(newnode);
    }

    iterator erase(iterator pos)
    {
        Node* cur = pos._node;
        Node* prev = cur->_prev;
        Node* next = cur->_next;

        delete cur;
        prev->_next = next;
        next->_prev = prev;

        --_size;
        return iterator(next);
    }

    size_t size() const
    {
        return _size;
    }

   private:
    Node* _head;
    size_t _size;
};

// ---------------------------------------------------------
// 4. 辅助打印函数 (Helper Print Functions)
// ---------------------------------------------------------

// 仅支持打印整型链表
inline void print_list(const list<int>& lt)
{
    list<int>::const_iterator it = lt.begin();
    while (it != lt.end())
    {
        std::cout << *it << " ";
        ++it;
    }
    std::cout << std::endl;
}

// 泛型打印链表，利用 typename 声明内嵌类型
template <typename T>
void print_list(const list<T>& lt)
{
    typename list<T>::const_iterator it = lt.begin();
    while (it != lt.end())
    {
        std::cout << *it << " ";
        ++it;
    }
    std::cout << std::endl;
}

// 泛型打印任意容器
template <typename Container>
void print_Container(const Container& con)
{
    typename Container::const_iterator it = con.begin();
    while (it != con.end())
    {
        std::cout << *it << " ";
        ++it;
    }
    std::cout << std::endl;
}
}