#include<vector>    // 向量容器，提供动态数组功能
#include<list>      // 链表容器，提供双向链表功能
#include<deque>     // 双端队列容器，提供首尾高效插入/删除功能

namespace pzh
{
    // 队列类模板定义
    // T: 队列中元素的类型
    // Container: 底层容器类型，默认为 deque<T>
    template<class T, class Container = deque<T>>
    class queue
    {
    public:
        void push(const T& x)
        {
            _con.push_back(x);
        }

        void pop()
        {
            _con.pop_front();
        }

        // 获取队列头部元素的常引用（不可修改）
        const T& front()
        {
            return _con.front();
        }

        // 获取队列尾部元素的常引用（不可修改）
        const T& back()
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
        // 底层容器对象，用于实际存储队列元素
        // 支持任何提供 front(), back(), push_back(), pop_front(), empty(), size() 接口的容器
        Container _con;
    };
};
