#include <iostream>
using namespace std;
#include <vector>  //数组
#include <list>  //链表
#include <deque>  //双向队列
namespace pzh
{
    template <class T, class Container = deque<T>>
    class stack
    {
    public:
        // 向栈顶压入元素
        // 参数：x - 要压入的元素，使用const引用避免拷贝
        void push(const T& x)
        {
            _con.push_back(x);
        }

        void pop()
        {
            _con.pop_back();
        }

        const T& top()
        {
            return _con.back();
        }

        bool empty()
        {
            return _con.empty();
        }

        size_t size()
        {
            return _con.size();
        }

    private:
        // 私有成员：底层容器对象, 用于实际存储栈的所有元素
        Container _con;
    };
}