// vector::iterator  -> ReverseIterator
// list::iterator    -> ReverseIterator

// 反向迭代器模板类
// 它是一个适配器，将正向迭代器转换为反向迭代器
// 模板参数说明：
// Iterator: 正向迭代器类型（如vector<int>::iterator）
// Ref: 引用类型（如int&）
// Ptr: 指针类型（如int*）
template<class Iterator, class Ref, class Ptr>
class ReverseIterator
{
public:
    typedef ReverseIterator<Iterator, Ref, Ptr> Self;

    // 构造函数：用一个正向迭代器初始化反向迭代器
    // 参数it：一个正向迭代器，表示反向迭代器的当前位置
    ReverseIterator(Iterator it)
        :_it(it)  // 用正向迭代器初始化成员变量_it
    {}

    Self& operator++()
    {
        --_it;
        return *this;
    }

    Self& operator--()
    {
        ++_it;
        return *this;
    }

    // 解引用运算符重载：返回当前迭代器指向的元素的引用
    // 注意：反向迭代器指向的是前一个元素
    Ref operator*()
    {
        Iterator cur = _it;
        return *(--cur);
    }

    // 箭头运算符重载：用于通过迭代器访问成员
    // 返回指向当前元素的指针
    // 例如：it->member 相当于 (*it).member
    Ptr operator->()
    {
        return &(operator*());
    }

    bool operator!=(const Self& s)
    {
        return _it != s._it;
    }

    bool operator==(const Self& s)
    {
        return _it == s._it;
    }
private:
    Iterator _it;
};

/*
反向迭代器的工作原理图示：

正向迭代器位置：
begin()    元素1    元素2    元素3    end()
   ↓        ↓        ↓        ↓        ↓
   [0]      [1]      [2]      [3]      [4]
    ↑                  ↑
    |                  |
rbegin()             rend()

反向迭代器位置：
rbegin() = ReverseIterator(end())    指向元素3
rend() = ReverseIterator(begin())    指向begin()之前

反向迭代器的解引用（operator*）：
当 _it = end() 时（rbegin()）：
  1. cur = _it (cur指向end())
  2. --cur (cur指向最后一个元素[3])
  3. *cur 返回元素3的引用

遍历顺序：
反向迭代器从rbegin()（最后一个元素）开始，到rend()（第一个元素之前）结束
rbegin() → rend() 对应正向的：end() → begin()
*/