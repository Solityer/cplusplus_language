#include<vector>

// 仿函数类模板：定义小于比较规则（实现小根堆/从小到大排序）
template<class T>
class Less  //从小到大
{
public:
    // 重载函数调用运算符，用于比较两个元素
    bool operator()(const T& x, const T& y)
    {
        return x < y;  //满足输出1, 否则输出0
    }
};

// 仿函数类模板：定义大于比较规则（实现大根堆/从大到小排序）
template<class T>
class Greater  //从大到小
{
public:
    bool operator()(const T& x, const T& y)
    {
        return x > y;
    }
};

namespace pzh
{
    // 优先队列类模板定义
    // 模板参数：
    // T - 队列中元素的类型
    // Container - 底层容器类型，默认为vector<T>
    // Compare - 比较器类型，默认为Less<T>（小根堆）
    template<class T, class Container = vector<T>, class Compare = Less<T>>
    class priority_queue
    {
    public:
        priority_queue()
        {}

        // 范围构造函数
        // 用迭代器范围[first, last)中的元素构造优先队列
        template <class InputIterator>
        priority_queue(InputIterator first, InputIterator last)
            :_con(first, last)  // 初始化列表：用范围初始化底层容器
        {
            // 从最后一个非叶子节点开始向下调整，构建堆结构
            // 完全二叉树性质：最后一个非叶子节点索引 = (size-2)/2
            for (int i = (_con.size()-2)/2; i>=0; --i)
            {
                adjust_down(i);
            }
        }

        // 向上调整函数（堆化操作）
        // 参数：child - 需要向上调整的节点索引
        void adjust_up(int child)
        {
            Compare com;
            int parent = (child - 1) / 2;
            // 循环向上调整，直到到达根节点或满足堆性质
            while (child > 0)
            {
                // 使用比较器判断是否需要交换
                // 对于小根堆：如果父节点小于子节点，需要交换
                // 对于大根堆：如果父节点大于子节点，需要交换
                if(com(_con[parent], _con[child]))
                    //if (_con[parent] < _con[child])
                {
                    swap(_con[child], _con[parent]);
                    child = parent;
                    parent = (child - 1) / 2;
                }
                else
                {
                    break;
                }
            }
        }

        // 向下调整函数（堆化操作）
        // 参数：parent - 需要向下调整的节点索引
        void adjust_down(int parent)
        {
            Compare com;
            size_t child = parent * 2 + 1;  // 计算左孩子节点索引
            // 循环向下调整，直到到达叶子节点或满足堆性质
            while (child < _con.size())
            {
                // 如果有右孩子，并且右孩子更符合堆性质（更小或更大）
                // 则选择右孩子作为要比较的孩子
                if (child + 1 < _con.size()
                    && com(_con[child], _con[child + 1]))
                {
                    ++child;  // 选择右孩子
                }
                // 检查父节点和孩子节点是否需要交换
                if (com(_con[parent], _con[child]))
                {
                    swap(_con[child], _con[parent]);
                    parent = child;
                    child = parent * 2 + 1;
                }
                else
                {
                    break;
                }
            }
        }

        void push(const T& x)
        {
            _con.push_back(x);
            adjust_up(_con.size()-1);  // 从新插入的位置向上调整
        }

        void pop()
        {
            // 交换根节点（最高优先级）和最后一个节点
            swap(_con[0], _con[_con.size()-1]);
            _con.pop_back();  // 移除原来的根节点（现在的最后一个元素）
            adjust_down(0); // 从根节点开始向下调整
        }

        // 获取最高优先级元素的引用（只读）
        // 返回值：根节点的常量引用
        const T& top()
        {
            return _con[0];
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
        Container _con;
    };
}