#ifndef MAP_H
#define MAP_H
#include <iostream>
#include "../../../Cs8/!includes/BTree_Class/btree_class.h"
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
    Map(){
        key_count=0;
    }

//  Capacity
    int size() const{
        return key_count;
    }
    bool empty() const{
        return map.empty();
    }

//  Element Access
    V& operator[](const K& key){
       Pair<K,V> temp(key);
       Pair<K,V>* p= map.find(temp);
       return p->value;
    }
    V& at(const K& key);
    const V& at(const K& key) const;

//  Modifiers
    void insert(const K& k, const V& v);
    void erase(const K& key);
    void clear();
    V get(const K& key);

//  Operations:
    bool find(const K& key);
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
    int key_count;
    BTree< Pair<K, V> > map;
};

template <typename K, typename V>
V& Map<K,V>::at(const K& key){
    Pair<K,V> temp(key);
    Pair<K,V>* p= map.find(temp);
    return p->value;
}

template <typename K, typename V>
const V& Map<K,V>::at(const K& key) const{
    Pair<K,V> temp(key);
    Pair<K,V>* p= map.find(temp);
    return p->value;
}


template <typename K, typename V>
void Map<K,V>:: insert(const K& k, const V& v){
    Pair<K,V> p(k,v);
    map.insert(p);
    key_count++;
}

template <typename K, typename V>
void Map<K,V>::erase(const K& key){
    Pair<K,V> p(key);
    map.remove(p);
    key_count--;
}

template <typename K, typename V>
void Map<K,V>::clear(){
    map.clear_tree();
    key_count=0;
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
bool Map<K,V>::find(const K& key){
    Pair<K,V> temp(key);
    Pair<K,V>* p= map.find(temp);
    if(p)
        return true;
    else
        return false;
}
#endif // MAP_H
