#include<vector>
using namespace std;

namespace pzh
{
    // 定义枚举状态，表示哈希表中每个位置的状态
    enum Status
    {
        EMPTY, // 空槽位，表示该位置未被使用
        EXIST, // 存在元素，表示该位置有有效数据
        DELETE // 已删除元素，表示该位置数据已被删除（伪删除）
    };

    // 哈希表中每个元素的数据结构模板
    template <class K, class V>
    struct HashData
    {
        pair<K, V> _kv; // 存储键值对，K为键类型，V为值类型
        Status _s;      // 存储当前位置的状态（EMPTY, EXIST, DELETE）
    };

    // 哈希函数模板类（默认用于整数类型）
    template <class K>
    struct HashFunc
    {
        // 重载函数调用运算符，将键转换为哈希值
        size_t operator()(const K& key)
        {
            return (size_t)key; // 默认哈希函数，直接将键转换为size_t类型
        }
    };

    // 哈希函数模板特化（用于字符串类型）
    template <>
    struct HashFunc<string>
    {
        // 重载函数调用运算符，专门处理字符串哈希
        size_t operator()(const string& key)
        {
            // BKDR Hash算法实现
            size_t hash = 0; // 初始化哈希值为0
            for (auto e : key) // 遍历字符串中的每个字符
            {
                hash *= 31;  // 乘以质数31（BKDR算法的常用乘数）
                hash += e;   // 加上当前字符的ASCII值
            }
            cout << key << ":" << hash << endl; // 输出字符串和对应的哈希值（调试用）
            return hash; // 返回计算得到的哈希值
        }
    };

    // 哈希表模板类定义
    template <class K, class V, class Hash = HashFunc<K>>
    class HashTable
    {
    public:
        // 默认构造函数
        HashTable()
        {
            _tables.resize(10); // 初始化哈希表大小为10个位置
        }

        // 插入键值对函数
        bool Insert(const pair<K, V>& kv)
        {
            if (Find(kv.first)) // 如果键已经存在，返回false
                return false;
            // 检查负载因子，当负载因子达到0.7时进行扩容
            if (_n * 10 / _tables.size() == 7)
            {
                size_t newSize = _tables.size() * 2; // 新大小为原来的2倍
                HashTable<K, V, Hash> newHT;         // 创建新的哈希表对象
                newHT._tables.resize(newSize);       // 设置新表的大小
                // 遍历旧表中的所有元素
                for (size_t i = 0; i < _tables.size(); i++)
                {
                    if (_tables[i]._s == EXIST) // 如果当前位置有有效元素
                    {
                        newHT.Insert(_tables[i]._kv); // 将元素插入到新表中
                    }
                }
                _tables.swap(newHT._tables); // 交换新旧表的向量内容
            }

            Hash hf; // 创建哈希函数对象
            // 线性探测解决哈希冲突
            size_t hashi = hf(kv.first) % _tables.size(); // 计算初始哈希位置
            while (_tables[hashi]._s == EXIST) // 如果该位置已被占用
            {
                hashi++;              // 线性向后探测下一个位置
                hashi %= _tables.size(); // 取模确保位置在表范围内（循环）
            }
            _tables[hashi]._kv = kv;      // 将键值对存储到找到的位置
            _tables[hashi]._s = EXIST;    // 设置该位置状态为存在
            ++_n;                         // 增加元素计数器
            return true;                  // 返回插入成功
        }

        // 查找键对应的元素
        HashData<K, V>* Find(const K& key)
        {
            Hash hf; // 创建哈希函数对象
            size_t hashi = hf(key) % _tables.size(); // 计算键的哈希位置
            while (_tables[hashi]._s != EMPTY) // 当位置不为空时继续查找
            {
                if (_tables[hashi]._s == EXIST      // 位置状态为存在
                    && _tables[hashi]._kv.first == key) // 且键匹配
                {
                    return &_tables[hashi]; // 返回找到元素的指针
                }
                hashi++;              // 键不匹配，继续线性探测
                hashi %= _tables.size(); // 取模确保位置在表范围内
            }
            return NULL; // 未找到，返回空指针
        }

        // 删除指定键的元素（伪删除法）
        bool Erase(const K& key)
        {
            HashData<K, V>* ret = Find(key); // 查找要删除的元素
            if (ret) // 如果找到了
            {
                ret->_s = DELETE; // 将状态设置为删除（伪删除）
                --_n;            // 减少元素计数器
                return true;     // 返回删除成功
            }
            else // 如果没找到
            {
                return false;    // 返回删除失败
            }
        }

        // 打印哈希表内容（用于调试）
        void Print()
        {
            for (size_t i = 0; i < _tables.size(); i++) // 遍历整个哈希表
            {
                if (_tables[i]._s == EXIST) // 如果位置有有效元素
                {
                    // 输出位置和键值对信息
                    cout << "[" << i << "]->" << _tables[i]._kv.first << ":" << _tables[i]._kv.second << endl;
                }
                else if (_tables[i]._s == EMPTY) // 如果位置为空
                {
                    printf("[%d]->\n", i); // 输出空位置
                }
                else // 如果位置状态为删除
                {
                    printf("[%d]->D\n", i); // 输出删除标记
                }
            }
            cout << endl; // 输出空行
        }

    private:
        vector<HashData<K, V>> _tables; // 存储哈希表数据的向量
        size_t _n = 0; // 存储当前哈希表中有效元素的个数
    };

    // 测试函数1：测试整数类型的哈希表
    void TestHT1()
    {
        HashTable<int, int> ht; // 创建整数哈希表
        int a[] = {4, 14, 24, 34, 5, 7, 1}; // 测试数据（会产生哈希冲突）
        for (auto e : a) // 遍历测试数组
        {
            ht.Insert(make_pair(e, e)); // 插入键值对
        }
        ht.Insert(make_pair(3, 3));     // 插入新元素
        ht.Insert(make_pair(3, 3));     // 插入重复元素（应该失败）
        ht.Insert(make_pair(-3, -3));   // 插入负数 转换为size_t(-3) = -3 + 2^64 = 18446744073709551613 % 20 = 13
        ht.Print();                     // 打印当前哈希表状态
        ht.Erase(3);                    // 删除元素3
        ht.Print();                     // 打印删除后的状态
        if (ht.Find(3))                 // 查找元素3
        {
            cout << "3存在" << endl;    // 如果找到
        }
        else
        {
            cout << "3不存在" << endl;  // 如果没找到
        }
        ht.Insert(make_pair(3, 3));     // 重新插入元素3
        ht.Insert(make_pair(23, 3));    // 插入新元素
        ht.Print();                     // 打印最终状态
    }

    // 测试函数2：测试字符串类型的哈希表和词频统计
    void TestHT2()
    {
        string arr[] = {"香蕉", "甜瓜", "苹果", "西瓜", "苹果", "西瓜", "苹果", "苹果",
            "西瓜","苹果", "香蕉", "苹果", "香蕉"};
        HashTable<string, int> ht; // 创建字符串哈希表
        for (auto& e : arr) // 遍历字符串数组
        {
            // 查找当前字符串是否已在哈希表中
            //auto ret = ht.Find(e);
            HashData<string, int>* ret = ht.Find(e);
            if (ret) // 如果找到了
            {
                ret->_kv.second++; // 增加该字符串的计数（词频统计）
            }
            else // 如果没找到
            {
                ht.Insert(make_pair(e, 1)); // 插入新字符串，初始计数为1
            }
        }

        ht.Print(); // 打印词频统计结果

        // 插入更多测试数据
        ht.Insert(make_pair("apple", 1));
        ht.Insert(make_pair("sort", 1));
        ht.Insert(make_pair("abc", 1));
        ht.Insert(make_pair("acb", 1));
        ht.Insert(make_pair("aad", 1));

        ht.Print(); // 打印最终哈希表状态
    }
}

// 链地址法哈希表命名空间
namespace hash_bucket
{
    // 哈希节点定义（用于链地址法）
    template <class K, class V>
    struct HashNode
    {
        HashNode* _next;   // 指向下一个节点的指针
        pair<K, V> _kv;    // 存储的键值对

        // 构造函数
        HashNode(const pair<K, V>& kv)
            : _kv(kv)      // 初始化键值对
            , _next(nullptr) // 初始化下一个指针为空
        {}
    };

    // 链地址法哈希表模板类
    template <class K, class V>
    class HashTable
    {
        typedef HashNode<K, V> Node; // 类型别名，方便使用
    public:
        // 构造函数
        HashTable()
        {
            _tables.resize(10); // 初始化哈希表大小为10个桶
        }

        // 析构函数
        ~HashTable()
        {
            for (size_t i = 0; i < _tables.size(); i++) // 遍历所有桶
            {
                Node* cur = _tables[i]; // 获取当前桶的头节点
                while (cur) // 遍历当前桶的链表
                {
                    Node* next = cur->_next; // 保存下一个节点
                    delete cur;              // 删除当前节点
                    cur = next;              // 移动到下一个节点
                }
                _tables[i] = nullptr; // 将桶指针置空
            }
        }

        // 插入键值对
        bool Insert(const pair<K, V>& kv)
        {
            if (Find(kv.first)) // 如果键已存在，返回false
                return false;

            // 检查负载因子，当元素数量等于表大小时扩容
            if (_n == _tables.size())
            {
                size_t newSize = _tables.size() * 2; // 新大小为原来的2倍
                HashTable<K, V> newHT;               // 创建新的哈希表
                newHT._tables.resize(newSize);       // 设置新表大小
                
                // 遍历旧表中的所有元素
                for (size_t i = 0; i < _tables.size(); i++)
                {
                    Node* cur = _tables[i]; // 获取当前桶的头节点
                    while (cur) // 遍历当前桶的链表
                    {
                        newHT.Insert(cur->_kv); // 将元素插入到新表中
                        cur = cur->_next;       // 移动到下一个节点
                    }
                }

                _tables.swap(newHT._tables); // 交换新旧表
            }

            size_t hashi = kv.first % _tables.size(); // 计算哈希位置（桶索引）
            Node* newnode = new Node(kv);            // 创建新节点

            // 头插法：将新节点插入到链表头部
            newnode->_next = _tables[hashi]; // 新节点指向原头节点
            _tables[hashi] = newnode;        // 更新头节点为新节点
            ++_n;                            // 增加元素计数器

            return true; // 返回插入成功
        }

        // 查找函数（未完整实现）
        Node* Find(const K& key)
        {
            //.... // 需要实现查找逻辑
            return NULL; // 暂时返回空
        }

    private:
        vector<Node*> _tables; // 存储桶的向量，每个桶是一个链表头指针
        size_t _n = 0;         // 存储当前哈希表中元素的总数
    };

    // 测试链地址法哈希表
    void TestHT()
    {
        HashTable<int, int> ht; // 创建链地址法哈希表
        int a[] = {4, 14, 24, 34, 5, 7, 1, 15, 25, 3}; // 测试数据
        for (auto e : a) // 遍历测试数据
        {
            ht.Insert(make_pair(e, e)); // 插入键值对
        }
        ht.Insert(make_pair(13, 13)); // 插入额外元素
    }
}