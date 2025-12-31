#include"RBTree.h"

namespace pzh
{
    template<class K>
    class set
    {
    public:
        struct SetKeyOfT
        {
            const K& operator()(const K& key)
            {
                return key;
            }
        };

        typedef typename RBTree<K, K, SetKeyOfT>::iterator iterator;

        iterator begin()
        {
            return _t.begin();
        }

        iterator end()
        {
            return _t.end();
        }

        pair<iterator, bool> insert(const K& key)
        {
            return _t.Insert(key);
        }

    private:
        RBTree<K, K, SetKeyOfT> _t;
    };
}