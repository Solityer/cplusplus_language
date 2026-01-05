#pragma once
#include<vector>

template<class K>
// 默认的仿函数功能，可以直接进行转换
struct HashFunc
{
    size_t operator()(const K& key)
    {
        // 如果key值可以转换成size_t整型，就直接进行转换
        return (size_t)key;
    }
};

// 针对string类型进行转换（针对string的特化版本）
template<>
struct HashFunc<string>
{
    size_t operator()(const string& key)
    {
        size_t hash = 0;
        for (auto e : key)
        {
            hash *= 31;
            hash += e;
        }
        cout << key << ":" << hash << endl;
        return hash;
    }
};

// 开放地址法
namespace pzh_open_address
{
    enum Status
    {
        EMPTY,
        EXIST,
        DELETE
    };

    template<class K, class V>
    struct HashData
    {
        pair<K, V> _kv;
        Status _s; //状态
    };

    //因为不是所有的key都可以正常的继续取模运算，所以需要加入仿函数Hash
    template<class K, class V, class Hash = HashFunc<K>>
    class HashTable_pre
    {
    public:
        HashTable_pre()
        {
            _tables.resize(10);  //一开始就给哈希表开一点空间
        }

        bool Insert(const pair<K, V> &kv)
        {
            if (Find(kv.first))
                return false;
            // 负载因子0.7就扩容
            if (_n * 10 / _tables.size() == 7)
            {
                size_t newSize = _tables.size() * 2;  //将旧表扩容原来的2倍
                HashTable_pre<K, V, Hash> newHT;  // 扩容需要重新映射
                newHT._tables.resize(newSize);
                // 遍历旧表
                for (size_t i = 0; i < _tables.size(); i++)
                {
                    if (_tables[i]._s == EXIST)
                    {
                        newHT.Insert(_tables[i]._kv);
                    }
                }
                // 交换新旧表的存储空间
                _tables.swap(newHT._tables);
            }
            Hash hf;
            size_t hashi = hf(kv.first) % _tables.size();  // 算起始的第一个位置
            while (_tables[hashi]._s == EXIST)  //起始位置的状态是“存在”
            {
                hashi++;
                hashi %= _tables.size();
            }
            _tables[hashi]._kv = kv;
            _tables[hashi]._s = EXIST;
            ++_n;
            return true;
        }

        HashData<K, V>* Find(const K& key)
        {
            Hash hf;
            size_t hashi = hf(key) % _tables.size();
            while (_tables[hashi]._s != EMPTY)
            {
                if (_tables[hashi]._s == EXIST && _tables[hashi]._kv.first == key)
                {
                    return &_tables[hashi];
                }
                hashi++;
                hashi %= _tables.size();
            }
            return NULL;
        }

        // 伪删除法
        /*删除3后，再插入3，由于只是改变了状态，并没有真正删除这个值*/
        bool Erase(const K& key)
        {
            HashData<K, V>* ret = Find(key);
            if (ret)
            {
                ret->_s = DELETE;
                --_n;
                return true;
            }
            else
            {
                return false;
            }
        }

        void Print()
        {
            for (size_t i = 0; i < _tables.size(); i++)
            {
                if (_tables[i]._s == EXIST)
                {
                    //printf("[%d]->%d\n", i, _tables[i]._kv.first);
                    cout << "[" << i << "]->" << _tables[i]._kv.first <<":" << _tables[i]._kv.second<< endl;
                }
                else if (_tables[i]._s == EMPTY)
                {
                    printf("[%d]->\n", i);
                }
                else
                {
                    printf("[%d]->D\n", i);
                }
            }
            cout << endl;
        }

    private:
        vector<HashData<K, V>> _tables;
        size_t _n = 0; // 存储的关键字的个数
    };

    void TestHT1()  //没加仿函数参数之前的测试
    {
        // 如果没有传第三个模板参数，就默认按照HashFun功能进行转换
        HashTable_pre<int, int> ht;
        int a[] = { 4,14,24,34,5,7,1 };
        for (auto e : a)
        {
            ht.Insert(make_pair(e, e));
        }
        ht.Insert(make_pair(3, 3));
        ht.Insert(make_pair(3, 3));
        ht.Insert(make_pair(-3, -3));
        ht.Print();
        ht.Erase(3);
        ht.Print();
        if (ht.Find(3))
        {
            cout << "3 exist" << endl;
        }
        else
        {
            cout << "3 not exist " << endl;
        }
        ht.Insert(make_pair(3, 3));
        ht.Insert(make_pair(23, 3));
        ht.Print();
    }

    void TestHT2()  //没加仿函数参数之前的测试
    {
        string arr[] = { "香蕉", "甜瓜","苹果", "西瓜", "苹果", "西瓜", "苹果", "苹果", "西瓜", "苹果", "香蕉", "苹果", "香蕉" };
        HashTable_pre<string, int> ht;
        for (auto& e : arr)
        {
            //auto ret = ht.Find(e);
            HashData<string, int>* ret = ht.Find(e);
            if (ret)
            {
                ret->_kv.second++;
            }
            else
            {
                ht.Insert(make_pair(e, 1));
            }
        }
        ht.Print();
        ht.Insert(make_pair("apple", 1));
        ht.Insert(make_pair("sort", 1));
        ht.Insert(make_pair("abc", 1));
        ht.Insert(make_pair("acb", 1));
        ht.Insert(make_pair("aad", 1));
        ht.Print();
    }
}

namespace pzh_hash_bucket
{
	template<class T>
	struct HashNode
	{
		HashNode<T>* _next;
		T _data;

		HashNode(const T& data)
			:_data(data)
			,_next(nullptr)
		{}
	};

	// 前置声明哈希表类，因为迭代器需要用到
	template<class K, class T, class KeyOfT, class Hash>
	class HashTable;

	// 哈希表迭代器模板类
	template<class K, class T, class Ref, class Ptr, class KeyOfT, class Hash>
	struct __HTIterator
	{
		typedef HashNode<T> Node;
		typedef __HTIterator<K, T, Ref, Ptr, KeyOfT, Hash> Self;  // 迭代器自身类型别名
		Node* _node;
		const HashTable<K, T, KeyOfT, Hash>* _pht;  // 指向所属哈希表的指针
		// vector<Node*> * _ptb;
		size_t _hashi;   // 当前桶的索引

		// 构造函数（非const版本）
		__HTIterator(Node* node, HashTable<K, T, KeyOfT, Hash>* pht, size_t hashi)
			:_node(node)
			,_pht(pht)
			,_hashi(hashi)
		{}

		// 构造函数（const版本）
		__HTIterator(Node* node, const HashTable<K, T, KeyOfT, Hash>* pht, size_t hashi)
			:_node(node)
			, _pht(pht)
			, _hashi(hashi)
		{}

		// 前置++运算符重载
		Self& operator++()
		{
			if (_node->_next)
			{
				// 当前桶还有节点，走到下一个节点
				_node = _node->_next;
			}
			else
			{
				// 当前桶已经走完了，找下一个桶开始
				//KeyOfT kot;
				//Hash hf;
				//size_t hashi = hf(kot(_node->_data)) % _pht._tables.size();
				++_hashi;
				while (_hashi < _pht->_tables.size())
				{
					if (_pht->_tables[_hashi])
					{
						_node = _pht->_tables[_hashi];
						break;
					}

					++_hashi;
				}
				if (_hashi == _pht->_tables.size())
				{
					_node = nullptr;
				}
			}
			return *this;
		}

		// 解引用运算符重载
		Ref operator*()
		{
			return _node->_data;
		}

		Ptr operator->()
		{
			return &_node->_data;
		}

		bool operator!=(const Self& s)
		{
			return _node != s._node;
		}
	};

	// 哈希表模板类
	// unordered_set -> Hashtable<K, K>
	// unordered_map -> Hashtable<K, pair<K, V>>
	template<class K, class T, class KeyOfT, class Hash>
	class HashTable
	{
		typedef HashNode<T> Node;

		// 声明迭代器为友元类，使其可以访问私有成员
		template<class K1, class T1, class Ref, class Ptr, class KeyOfT1, class Hash1>
		friend struct __HTIterator;

	public:
		typedef __HTIterator<K, T, T&, T*, KeyOfT, Hash> iterator;  // 迭代器类型别名
		typedef __HTIterator<K, T, const T&, const T*, KeyOfT, Hash> const_iterator;

		iterator begin()
		{
			for (size_t i = 0; i < _tables.size(); i++)
			{
				if (_tables[i])
				{
					return iterator(_tables[i], this, i);
				}
			}
			return end();
		}

		iterator end()
		{
			// 返回空节点指针，索引为-1
			return iterator(nullptr, this, -1);
		}

		const_iterator begin() const
		{
			for (size_t i = 0; i < _tables.size(); i++)
			{
				if (_tables[i])
				{
					return const_iterator(_tables[i], this, i);
				}
			}
			return end();
		}

		// this-> const HashTable<K, T, KeyOfT, Hash>*
		const_iterator end() const
		{
			return const_iterator(nullptr, this, -1);
		}

		HashTable()
		{
			_tables.resize(10);
		}

		~HashTable()
		{
			for (size_t i = 0; i < _tables.size(); i++)
			{
				Node* cur = _tables[i];
				while (cur)
				{
					Node* next = cur->_next;
					delete cur;
					cur = next;
				}
				_tables[i] = nullptr;
			}
		}

		// 插入元素
		pair<iterator, bool> Insert(const T& data)
		{
			Hash hf;
			KeyOfT kot;   // 提取键的函数对象
			iterator it = Find(kot(data));  // 先查找是否已存在相同键
			if (it != end())
				return make_pair(it, false);
			// 负载因子最大到1
			if (_n == _tables.size())
			{
				vector<Node*> newTables;
				newTables.resize(_tables.size() * 2, nullptr);
				// 遍历旧表
				for (size_t i = 0; i < _tables.size(); i++)
				{
					Node* cur = _tables[i];
					while(cur)
					{
						Node* next = cur->_next;
						// 挪动到映射的新表
						size_t hashi = hf(kot(cur->_data)) % newTables.size();
						cur->_next = newTables[i];
						newTables[hashi] = cur;
						cur = next;
					}
					_tables[i] = nullptr;
				}
				_tables.swap(newTables);
			}
			size_t hashi = hf(kot(data)) % _tables.size();
			Node* newnode = new Node(data);
			// 头插
			newnode->_next = _tables[hashi];
			_tables[hashi] = newnode;
			++_n;
			return make_pair(iterator(newnode, this, hashi), true);
		}

		iterator Find(const K& key)
		{
			Hash hf;     // 哈希函数对象
			KeyOfT kot;  // 提取键的函数对象
			size_t hashi = hf(key) % _tables.size();
			Node* cur = _tables[hashi];
			while (cur)
			{
				if (kot(cur->_data) == key)
				{
					return iterator(cur, this, hashi);
				}
				cur = cur->_next;
			}
			return end();
		}

		bool Erase(const K& key)
		{
			Hash hf;
			KeyOfT kot;
			size_t hashi = hf(key) % _tables.size();
			Node* prev = nullptr;
			Node* cur = _tables[hashi];
			while (cur)
			{
				if (kot(cur->_data) == key)
				{
					if (prev == nullptr)
					{
						_tables[hashi] = cur->_next;
					}
					else
					{
						prev->_next = cur->_next;
					}
					delete cur;
					return true;
				}
				prev = cur;
				cur = cur->_next;
			}
			return false;
		}

		// 统计哈希表信息
		void Some()
		{
			size_t bucketSize = 0;
			size_t maxBucketLen = 0;
			size_t sum = 0;
			double averageBucketLen = 0;
			for (size_t i = 0; i < _tables.size(); i++)
			{
				Node* cur = _tables[i];
				if (cur)
				{
					++bucketSize;
				}
				size_t bucketLen = 0;
				while (cur)
				{
					++bucketLen;
					cur = cur->_next;
				}
				sum += bucketLen;
				if (bucketLen > maxBucketLen)
				{
					maxBucketLen = bucketLen;
				}
			}
			averageBucketLen = (double)sum / (double)bucketSize;
			printf("all bucketSize:%d\n", _tables.size());
			printf("bucketSize:%d\n", bucketSize);
			printf("maxBucketLen:%d\n", maxBucketLen);
			printf("averageBucketLen:%lf\n\n", averageBucketLen);
		}

	private:
		vector<Node*> _tables;
		size_t _n = 0;
	};
}