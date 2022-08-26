#ifndef BPT_MAP_H
#define BPT_MAP_H
#include <iostream>
#include "../../../Cs8/!includes/BPTree_class/bptree_class.h"
using namespace std;

template <typename K, typename V>
struct Pair{
    K key;
    V value;

    Pair(const K& k=K(), const V& v=V()){
        key=k;
        value=v;
    }
    Pair<K,V>& operator =(const Pair<K,V>& RHS){
        key=RHS.key;
        value=RHS.value;
        return *this;
    }
    friend ostream& operator <<(ostream& outs, const Pair<K, V>& print_me){
       outs<<"("<<print_me.key<<")"<<print_me.value;
       return outs;
    }
    friend bool operator ==(const Pair<K, V>& lhs, const Pair<K, V>& rhs){
        return lhs.key==rhs.key;
    }
    friend bool operator < (const Pair<K, V>& lhs, const Pair<K, V>& rhs){
        return lhs.key<rhs.key;
    }
    friend bool operator > (const Pair<K, V>& lhs, const Pair<K, V>& rhs){
        return lhs.key>rhs.key;
    }
    friend bool operator <= (const Pair<K, V>& lhs, const Pair<K, V>& rhs){
        return lhs.key<=rhs.key;
    }
    friend bool operator >= (const Pair<K, V>& lhs, const Pair<K, V>& rhs){
        return lhs.key>=rhs.key;
    }
    friend Pair<K, V> operator + (const Pair<K, V>& lhs, const Pair<K, V>& rhs){
        assert(false);
    }
};

template <typename K, typename V>
class Map
{
public:
    typedef BPlusTree<Pair<K, V> > map_base;

    class Iterator{
        public:
            friend class Map;
            Iterator(typename map_base::Iterator it=typename map_base::Iterator()):
            _it(it){}
            Iterator operator ++(int unused){
                return _it++;
            } //it++

            Iterator operator ++(){
                return ++_it;
            }
            Pair<K, V> operator *(){
                return *_it;
            }
            friend bool operator ==(const Iterator& lhs, const Iterator& rhs){
                return lhs._it==rhs._it;
            }
            friend bool operator !=(const Iterator& lhs, const Iterator& rhs){
                return lhs._it!=rhs._it;
            }
        private:
            typename map_base::Iterator _it;

    };

    Map(){  //: map(false)
    }
//  Iterators
    Iterator begin(){
        return map.begin();
    }
    Iterator end(){
        return map.end();
    }

    Iterator lower_bound(const K& key);
    Iterator upper_bound(const K& key);

//  Capacity
    int size() const{
        return map.size();
    }
    bool empty() const{
        return map.empty();
    }

//  Element Access
    V& operator[](const K& key);

    V& at(const K& key);
    const V& at(const K& key) const;

//  Modifiers
    void insert(const K& k, const V& v);
    void erase(const K& key);
    void clear();
    V get(const K& key);

//  Operations:
    Iterator find(const K& key);
    bool contains(const Pair<K, V>& target);

    // I have not writtent hese yet, but we will need them:
    //    int count(const K& key);
    //    lower_bound
    //    upper_bound
    //    equal_range:

    bool is_valid(){return map.is_valid();}

    friend ostream& operator<<(ostream& outs, const Map<K, V>& print_me){
        outs<<print_me.map<<endl;
        return outs;
    }
private:
    BPlusTree< Pair<K, V> > map;
};
template <typename K, typename V>
V& Map<K,V>::operator[](const K& key){
//   Pair<K,V> temp(key);
//   typename map_base::Iterator it= map.find(temp);
//   Pair<K,V> p=*it;
//   return p.value;
    return map.find_ptr(Pair<K, V>(key, V()))->value;
}

template <typename K, typename V>
V& Map<K,V>::at(const K& key){
    Pair<K,V> temp(key);
    typename map_base::Iterator it= map.find(temp);
    Pair<K,V> p=*it;
    return p.value;
}

template <typename K, typename V>
const V& Map<K,V>::at(const K& key) const{
    Pair<K,V> temp(key);
    typename map_base::Iterator it= map.find(temp);
    Pair<K,V> p=*it;
    return p.value;
}


template <typename K, typename V>
void Map<K,V>:: insert(const K& k, const V& v){
    Pair<K,V> p(k,v);
    map.insert(p);
}

//template <typename K, typename V>
//void Map<K,V>::erase(const K& key){
//    Pair<K,V> p(key);
//    map.remove(p);
//}

template <typename K, typename V>
void Map<K,V>::clear(){
    map.clear_tree();
}

template <typename K, typename V>
V Map<K,V>:: get(const K& key){
    Pair<K,V> temp(key);
    Pair<K,V> p= map.get(temp);
    return p.value;
}

template <typename K, typename V>
bool Map<K,V>::contains(const Pair<K, V>& target){
    return map.contains(target);
}

template <typename K, typename V>
typename Map<K,V>:: Iterator Map<K,V>::find(const K& key){
    Pair<K,V> temp(key);
    return map.find(temp);
}

template <typename K, typename V>
typename Map<K,V>:: Iterator Map<K,V>:: lower_bound(const K& key){
    return map.lower_bound(key);
}

template <typename K, typename V>
typename Map<K,V>:: Iterator Map<K,V>::upper_bound(const K& key){
    return map.upper_bound(key);
}
#endif // BPT_MAP_H
