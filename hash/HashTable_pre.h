#pragma once
#include <vector>

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

// 拉链法（哈希桶）
namespace pzh_hash_bucket
{
    template<class K, class V>
    struct HashNode
    {
        HashNode* _next;
        pair<K, V> _kv;

        HashNode(const pair<K, V>& kv)
            :_kv(kv)
            ,_next(nullptr)
        {}
    };

    template<class K, class V, class Hash = HashFunc<K>>
    class HashTable_pre
    {
        typedef HashNode<K, V> Node;
    public:
        HashTable_pre()
        {
            _tables.resize(10);
        }

        ~HashTable_pre()
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

        bool Insert(const pair<K, V>& kv)
        {
            if (Find(kv.first))
                return false;
            Hash hf;
            // 负载因子最大到1
            if (_n == _tables.size())
            {
                // size_t newSize = _tables.size() * 2;
                // HashTable<K, V> newHT;
                // newHT._tables.resize(newSize);
                // // 遍历旧表
                // for (size_t i = 0; i < _tables.size(); i++)
                // {
                //     Node* cur = _tables[i];
                //     while(cur)
                //     {
                //         newHT.Insert(cur->_kv);
                //         cur = cur->_next;
                //     }
                // }
                // _tables.swap(newHT._tables);

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
                        size_t hashi = hf(cur->_kv.first) % newTables.size();
                        cur->_next = newTables[hashi];
                        newTables[hashi] = cur;

                        cur = next;
                    }
                    _tables[i] = nullptr;
                }
                _tables.swap(newTables);
            }
            size_t hashi = hf(kv.first) % _tables.size();
            Node* newnode = new Node(kv);
            // 头插
            newnode->_next = _tables[hashi];
            _tables[hashi] = newnode;
            ++_n;
            return true;
        }

        Node* Find(const K& key)
        {
            Hash hf;
            size_t hashi = hf(key) % _tables.size();
            Node* cur = _tables[hashi];
            while (cur)
            {
                if (cur->_kv.first == key)
                {
                    return cur;
                }
                cur = cur->_next;
            }
            return NULL;
        }

        bool Erase(const K& key)
        {
            Hash hf;
            size_t hashi = hf(key) % _tables.size();
            Node* prev = nullptr;
            Node* cur = _tables[hashi];
            while (cur)
            {
                if (cur->_kv.first == key)
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

        // 哈希的一些情况
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

    void TestHT1()
	{
		HashTable_pre<int, int> ht;
		int a[] = { 4,14,24,34,5,7,1,15,25,3 };
		for (auto e : a)
		{
			ht.Insert(make_pair(e, e));
		}

		ht.Insert(make_pair(13, 13));

		cout << ht.Find(4) << endl;
		ht.Erase(4);
		cout << ht.Find(4) << endl;
	}

	void TestHT2()
	{
		string arr[] = { "香蕉", "甜瓜","苹果", "西瓜", "苹果", "西瓜", "苹果", "苹果", "西瓜", "苹果", "香蕉", "苹果", "香蕉" };
		HashTable_pre<string, int> ht;
		for (auto& e : arr)
		{
			//auto ret = ht.Find(e);
			HashNode<string, int>* ret = ht.Find(e);
			if (ret)
			{
				ret->_kv.second++;
			}
			else
			{
				ht.Insert(make_pair(e, 1));
			}
		}
	}

	void TestHT3()
	{
		const size_t N = 1000000;

		unordered_set<int> us;
		set<int> s;
		HashTable_pre<int, int> ht;

		vector<int> v;
		v.reserve(N);
		srand(time(0));
		for (size_t i = 0; i < N; ++i)
		{
			//v.push_back(rand()); // N比较大时，重复值比较多
			v.push_back(rand()+i); // 重复值相对少
			//v.push_back(i); // 没有重复，有序
		}

		// 21:15
		size_t begin1 = clock();
		for (auto e : v)
		{
			s.insert(e);
		}
		size_t end1 = clock();
		cout << "set insert:" << end1 - begin1 << endl;

		size_t begin2 = clock();
		for (auto e : v)
		{
			us.insert(e);
		}
		size_t end2 = clock();
		cout << "unordered_set insert:" << end2 - begin2 << endl;

		size_t begin10 = clock();
		for (auto e : v)
		{
			ht.Insert(make_pair(e, e));
		}
		size_t end10 = clock();
		cout << "HashTbale insert:" << end10 - begin10 << endl << endl;


	    size_t begin3 = clock();
		for (auto e : v)
		{
			s.find(e);
		}
		size_t end3 = clock();
		cout << "set find:" << end3 - begin3 << endl;

		size_t begin4 = clock();
		for (auto e : v)
		{
			us.find(e);
		}
		size_t end4 = clock();
		cout << "unordered_set find:" << end4 - begin4 << endl;

		size_t begin11 = clock();
		for (auto e : v)
		{
			ht.Find(e);
		}
		size_t end11 = clock();
		cout << "HashTable find:" << end11 - begin11 << endl << endl;

		cout << "插入数据个数：" << us.size() << endl << endl;
		ht.Some();

		size_t begin5 = clock();
		for (auto e : v)
		{
			s.erase(e);
		}
		size_t end5 = clock();
		cout << "set erase:" << end5 - begin5 << endl;

		size_t begin6 = clock();
		for (auto e : v)
		{
			us.erase(e);
		}
		size_t end6 = clock();
		cout << "unordered_set erase:" << end6 - begin6 << endl;

		size_t begin12 = clock();
		for (auto e : v)
		{
			ht.Erase(e);
		}
		size_t end12 = clock();
		cout << "HashTable Erase:" << end12 - begin12 << endl << endl;
	}
}