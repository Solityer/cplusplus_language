// 防止头文件重复包含
#include<vector>
#include<string>

// 哈希函数模板 - 默认版本（适用于整数类型）
template<class K>
struct HashFunc
{
	// 重载函数调用运算符，将key转换为size_t类型
	size_t operator()(const K& key)
	{
		return (size_t)key;  // 直接类型转换
	}
};

// 哈希函数特化版本 - 针对string类型
// 注意：原代码缺少template<>，已修正
template<>
struct HashFunc<string>
{
	size_t operator()(const string& key)
	{
		// BKDR哈希算法
		size_t hash = 0;
		for (auto e : key)  // 遍历字符串每个字符
		{
			hash *= 31;      // 乘以质数31
			hash += e;       // 加上字符值
		}
		return hash;
	}
};

// 开放定址法命名空间
namespace open_address
{
	// 枚举类型，表示哈希表中每个位置的状态
	enum Status
	{
		EMPTY,    // 空位置
		EXIST,    // 有数据
		DELETE    // 已删除（惰性删除）
	};

	// 哈希表存储的数据结构
	template<class K, class V>
	struct HashData
	{
		pair<K, V> _kv;  // 键值对
		Status _s;       // 位置状态
	};

	// 哈希表类模板
	// Hash：哈希函数类型，默认为HashFunc<K>
	template<class K, class V, class Hash = HashFunc<K>>
	class HashTable
	{
	public:
		// 构造函数，初始化哈希表大小为10
		HashTable()
		{
			_tables.resize(10);
		}

		// 插入键值对
		bool Insert(const pair<K, V>& kv)
		{
			// 如果键已存在，返回false
			if (Find(kv.first))
				return false;

			// 负载因子达到0.7时扩容
			// _n是元素个数，_tables.size()是表大小
			if (_n * 10 / _tables.size() == 7)  // 等价于_n/_tables.size() >= 0.7
			{
				// 新表大小为原来的2倍
				size_t newSize = _tables.size() * 2;
				// 创建新的哈希表
				HashTable<K, V, Hash> newHT;
				newHT._tables.resize(newSize);

				// 遍历旧表，将所有存在的元素重新插入到新表
				for (size_t i = 0; i < _tables.size(); i++)
				{
					if (_tables[i]._s == EXIST)  // 只处理存在的元素
					{
						newHT.Insert(_tables[i]._kv);
					}
				}

				// 交换新旧表的存储空间
				_tables.swap(newHT._tables);
			}

			// 创建哈希函数对象
			Hash hf;

			// 计算初始哈希位置
			size_t hashi = hf(kv.first) % _tables.size();

			// 线性探测：如果位置被占用，就向后查找
			while (_tables[hashi]._s == EXIST)
			{
				hashi++;            // 向后移动
				hashi %= _tables.size();  // 循环到表头
			}

			// 找到空位，插入数据
			_tables[hashi]._kv = kv;
			_tables[hashi]._s = EXIST;  // 标记为存在
			++_n;  // 元素计数加1

			return true;
		}

		// 查找键对应的数据
		HashData<K, V>* Find(const K& key)
		{
			Hash hf;  // 哈希函数对象

			// 计算哈希位置
			size_t hashi = hf(key) % _tables.size();

			// 线性探测查找
			// 只要位置不是空（可能是存在或删除状态），就继续查找
			while (_tables[hashi]._s != EMPTY)
			{
				// 如果位置是存在状态，且键匹配，返回该位置指针
				if (_tables[hashi]._s == EXIST &&
					_tables[hashi]._kv.first == key)
				{
					return &_tables[hashi];
				}

				// 不匹配，继续向后查找
				hashi++;
				hashi %= _tables.size();  // 循环到表头
			}

			// 未找到，返回空指针
			return NULL;
		}

		// 删除键对应的数据（伪删除法）
		bool Erase(const K& key)
		{
			// 先查找键是否存在
			HashData<K, V>* ret = Find(key);
			if (ret)
			{
				// 标记为删除状态，而不是真正移除数据
				ret->_s = DELETE;
				--_n;  // 元素计数减1
				return true;
			}
			else
			{
				return false;  // 键不存在
			}
		}

		// 打印哈希表内容（用于调试）
		void Print()
		{
			for (size_t i = 0; i < _tables.size(); i++)
			{
				if (_tables[i]._s == EXIST)
				{
					// 打印存在的数据
					cout << "[" << i << "]->" << _tables[i]._kv.first
						 << ":" << _tables[i]._kv.second << endl;
				}
				else if (_tables[i]._s == EMPTY)
				{
					// 打印空位置
					printf("[%d]->\n", i);
				}
				else  // DELETE状态
				{
					// 打印删除位置
					printf("[%d]->D\n", i);
				}
			}
			cout << endl;
		}

	private:
		vector<HashData<K, V>> _tables;  // 哈希表存储数组
		size_t _n = 0;  // 存储的关键字的个数
	};

	// 测试函数1：测试整数类型
	void TestHT1()
	{
		HashTable<int, int> ht;
		int a[] = { 4,14,24,34,5,7,1 };
		for (auto e : a)
		{
			ht.Insert(make_pair(e, e));
		}

		ht.Insert(make_pair(3, 3));
		ht.Insert(make_pair(3, 3));  // 重复插入，应该失败
		ht.Insert(make_pair(-3, -3));
		ht.Print();

		ht.Erase(3);
		ht.Print();

		if (ht.Find(3))
		{
			cout << "3存在" << endl;
		}
		else
		{
			cout << "3不存在" << endl;
		}

		ht.Insert(make_pair(3, 3));
		ht.Insert(make_pair(23, 3));
		ht.Print();
	}

	// 测试函数2：测试字符串类型
	void TestHT2()
	{
		string arr[] = { "香蕉", "甜瓜","苹果", "西瓜", "苹果", "西瓜", "苹果", "苹果", "西瓜", "苹果", "香蕉", "苹果", "香蕉" };

		// 使用字符串特化的哈希函数
		HashTable<string, int> ht;

		for (auto& e : arr)
		{
			// 查找元素是否存在
			HashData<string, int>* ret = ht.Find(e);
			if (ret)
			{
				// 存在，计数器加1
				ret->_kv.second++;
			}
			else
			{
				// 不存在，插入新元素
				ht.Insert(make_pair(e, 1));
			}
		}

		ht.Print();

		// 测试更多插入
		ht.Insert(make_pair("apple", 1));
		ht.Insert(make_pair("sort", 1));
		ht.Insert(make_pair("abc", 1));
		ht.Insert(make_pair("acb", 1));
		ht.Insert(make_pair("aad", 1));

		ht.Print();
	}
}

// 哈希桶（拉链法）命名空间
namespace hash_bucket
{
	// 哈希桶节点
	template<class T>
	struct HashNode
	{
		HashNode<T>* _next;  // 指向下一个节点的指针
		T _data;             // 存储的数据

		// 构造函数
		HashNode(const T& data)
			:_data(data)
			,_next(nullptr)
		{}
	};

	// 前置声明，因为迭代器中需要用到HashTable
	template<class K, class T, class KeyOfT, class Hash>
	class HashTable;

	// 哈希表迭代器
	template<class K, class T, class Ref, class Ptr, class KeyOfT, class Hash>
	struct __HTIterator
	{
		// 类型别名定义
		typedef HashNode<T> Node;
		typedef __HTIterator<K, T, Ref, Ptr, KeyOfT, Hash> Self;

		// 迭代器成员变量
		Node* _node;       // 当前节点指针
		const HashTable<K, T, KeyOfT, Hash>* _pht;  // 指向哈希表的指针
		size_t _hashi;     // 当前桶的索引

		// 构造函数（可修改哈希表版本）
		__HTIterator(Node* node, HashTable<K, T, KeyOfT, Hash>* pht, size_t hashi)
			:_node(node)
			,_pht(pht)
			,_hashi(hashi)
		{}

		// 构造函数（const哈希表版本）
		__HTIterator(Node* node, const HashTable<K, T, KeyOfT, Hash>* pht, size_t hashi)
			:_node(node)
			, _pht(pht)
			, _hashi(hashi)
		{}

		// 前缀++运算符重载
		Self& operator++()
		{
			// 如果当前桶还有下一个节点
			if (_node->_next)
			{
				// 移动到当前桶的下一个节点
				_node = _node->_next;
			}
			else
			{
				// 当前桶已遍历完，找下一个非空桶
				++_hashi;  // 移动到下一个桶

				// 循环查找下一个非空桶
				while (_hashi < _pht->_tables.size())
				{
					if (_pht->_tables[_hashi])  // 桶不为空
					{
						_node = _pht->_tables[_hashi];  // 指向桶的第一个节点
						break;
					}
					++_hashi;  // 继续查找下一个桶
				}

				// 如果遍历完所有桶，将节点设为nullptr
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

		// 箭头运算符重载
		Ptr operator->()
		{
			return &_node->_data;
		}

		// 不等于运算符重载
		bool operator!=(const Self& s)
		{
			return _node != s._node;
		}
	};

	// 哈希表类模板（哈希桶实现）
	// K: 键类型
	// T: 值类型（对于unordered_map是pair<K,V>，对于unordered_set是K）
	// KeyOfT: 从T中提取键的仿函数
	// Hash: 哈希函数类型
	template<class K, class T, class KeyOfT, class Hash>
	class HashTable
	{
		typedef HashNode<T> Node;

		// 声明迭代器为友元，使其能访问私有成员
		template<class K, class T, class Ref, class Ptr, class KeyOfT, class Hash>
		friend struct __HTIterator;

	public:
		// 迭代器类型定义
		typedef __HTIterator<K, T, T&, T*, KeyOfT, Hash> iterator;
		typedef __HTIterator<K, T, const T&, const T*, KeyOfT, Hash> const_iterator;

		// 返回指向第一个元素的迭代器
		iterator begin()
		{
			// 查找第一个非空桶
			for (size_t i = 0; i < _tables.size(); i++)
			{
				if (_tables[i])  // 桶不为空
				{
					return iterator(_tables[i], this, i);
				}
			}

			// 没有元素，返回end()
			return end();
		}

		// 返回尾后迭代器
		iterator end()
		{
			return iterator(nullptr, this, -1);
		}

		// const版本的begin()
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

		// const版本的end()
		const_iterator end() const
		{
			return const_iterator(nullptr, this, -1);
		}

		// 构造函数，初始化桶数组大小为10
		HashTable()
		{
			_tables.resize(10);
		}

		// 析构函数，释放所有节点
		~HashTable()
		{
			for (size_t i = 0; i < _tables.size(); i++)
			{
				Node* cur = _tables[i];
				while (cur)
				{
					Node* next = cur->_next;  // 保存下一个节点
					delete cur;               // 删除当前节点
					cur = next;               // 移动到下一个节点
				}
				_tables[i] = nullptr;  // 桶指针置空
			}
		}

		// 插入数据
		pair<iterator, bool> Insert(const T& data)
		{
			// 创建哈希函数和键提取函数对象
			Hash hf;
			KeyOfT kot;

			// 如果数据已存在，返回false
			iterator it = Find(kot(data));
			if (it != end())
				return make_pair(it, false);

			// 负载因子达到1时扩容（元素个数等于桶的数量）
			if (_n == _tables.size())
			{
				// 创建新的桶数组，大小为原来的2倍
				vector<Node*> newTables;
				newTables.resize(_tables.size() * 2, nullptr);

				// 遍历旧表的所有桶
				for (size_t i = 0; i < _tables.size(); i++)
				{
					Node* cur = _tables[i];
					while(cur)
					{
						Node* next = cur->_next;  // 保存下一个节点

						// 计算节点在新表中的位置
						size_t hashi = hf(kot(cur->_data)) % newTables.size();

						// 头插法插入新表
						cur->_next = newTables[hashi];
						newTables[hashi] = cur;

						// 处理下一个节点
						cur = next;
					}
					_tables[i] = nullptr;  // 旧桶置空
				}

				// 交换新旧桶数组
				_tables.swap(newTables);
			}

			// 计算数据应该插入的桶位置
			size_t hashi = hf(kot(data)) % _tables.size();

			// 创建新节点
			Node* newnode = new Node(data);

			// 头插法插入到对应桶中
			newnode->_next = _tables[hashi];
			_tables[hashi] = newnode;
			++_n;  // 元素计数加1

			// 返回插入的迭代器和true
			return make_pair(iterator(newnode, this, hashi), true);
		}

		// 查找键对应的数据
		iterator Find(const K& key)
		{
			Hash hf;
			KeyOfT kot;

			// 计算键所在的桶
			size_t hashi = hf(key) % _tables.size();
			Node* cur = _tables[hashi];

			// 在桶中线性查找
			while (cur)
			{
				if (kot(cur->_data) == key)  // 找到匹配的键
				{
					return iterator(cur, this, hashi);
				}
				cur = cur->_next;  // 检查下一个节点
			}

			// 未找到，返回end()
			return end();
		}

		// 删除键对应的数据
		bool Erase(const K& key)
		{
			Hash hf;
			KeyOfT kot;

			// 计算键所在的桶
			size_t hashi = hf(key) % _tables.size();
			Node* prev = nullptr;  // 前驱节点
			Node* cur = _tables[hashi];

			// 在桶中查找要删除的节点
			while (cur)
			{
				if (kot(cur->_data) == key)  // 找到要删除的节点
				{
					if (prev == nullptr)  // 要删除的是桶的第一个节点
					{
						_tables[hashi] = cur->_next;  // 更新桶头指针
					}
					else  // 要删除的是中间节点
					{
						prev->_next = cur->_next;  // 跳过当前节点
					}
					delete cur;  // 释放节点内存
					return true;
				}

				// 继续查找
				prev = cur;
				cur = cur->_next;
			}

			// 未找到要删除的键
			return false;
		}

		// 统计哈希表性能信息（用于调试）
		void Some()
		{
			size_t bucketSize = 0;     // 非空桶数量
			size_t maxBucketLen = 0;   // 最大桶长度
			size_t sum = 0;           // 所有桶中节点总数
			double averageBucketLen = 0;  // 平均桶长度

			// 遍历所有桶
			for (size_t i = 0; i < _tables.size(); i++)
			{
				Node* cur = _tables[i];

				// 统计非空桶
				if (cur)
				{
					++bucketSize;
				}

				// 统计当前桶的长度
				size_t bucketLen = 0;
				while (cur)
				{
					++bucketLen;
					cur = cur->_next;
				}

				sum += bucketLen;  // 累加总节点数

				// 更新最大桶长度
				if (bucketLen > maxBucketLen)
				{
					maxBucketLen = bucketLen;
				}
			}

			// 计算平均桶长度
			averageBucketLen = (double)sum / (double)bucketSize;

			// 输出统计信息
			printf("all bucketSize:%d\n", _tables.size());
			printf("bucketSize:%d\n", bucketSize);
			printf("maxBucketLen:%d\n", maxBucketLen);
			printf("averageBucketLen:%lf\n\n", averageBucketLen);
		}

	private:
		vector<Node*> _tables;  // 桶数组，每个元素是链表头指针
		size_t _n = 0;          // 存储的元素个数
	};
}