#ifndef MULTI_MAP_H
#define MULTI_MAP_H
#include <iostream>
#include "../../../Cs8/!includes/BTree_Class/btree_class.h"
#include "../../../CS3A/11P1_Vector_class/vector.h"
using namespace std;

template <typename K, typename V>
struct MPair{
    K key;
    my_vector<V> value_list;

    //--------------------------------------------------------------------------------
    /*      MPair CTORs:
     *  must have these CTORs:
     *  - default CTOR / CTOR with a key and no value: this will create an empty vector
     *  - CTOR with a key AND a value: pushes the value into the value_list
     *  _ CTOR with a key and a vector of values (to replace value_list)
     */
    //--------------------------------------------------------------------------------
    MPair(const K& k=K()){
        key=k;
    }
    MPair(const K& k, const V& v){
        key=k;
        value_list.push_back(v);
    }
    MPair(const K& k, const my_vector<V>& vlist){
        key=k;
        value_list=vlist;
    }
    //--------------------------------------------------------------------------------

    //You'll need to overlod << for your vector:
    friend ostream& operator <<(ostream& outs, const MPair<K, V>& print_me){
        outs<<"("<<print_me.key<<")"<<print_me.value_list;
        return outs;
    }

    friend bool operator ==(const MPair<K, V>& lhs, const MPair<K, V>& rhs){
        return lhs.key==rhs.key;
    }
    friend bool operator < (const MPair<K, V>& lhs, const MPair<K, V>& rhs){
        return lhs.key<rhs.key;
    }
    friend bool operator <= (const MPair<K, V>& lhs, const MPair<K, V>& rhs){
        return lhs.key<=rhs.key;
    }
    friend bool operator >= (const MPair<K, V>& lhs, const MPair<K, V>& rhs){
        return lhs.key>=rhs.key;
    }
    friend bool operator > (const MPair<K, V>& lhs, const MPair<K, V>& rhs){
        return lhs.key>rhs.key;
    }
    friend MPair<K, V> operator + (const MPair<K, V>& lhs, const MPair<K, V>& rhs){
         MPair<K, V> result(lhs.key, lhs.value_list+rhs.value_list);
         return result;
    }
};

template <typename K, typename V>
class MMap
{
public:
    MMap(): mmap(true){
    }

//  Capacity
    int size() const{
        return mmap.size();
    }
    bool empty() const{
        return mmap.empty();
    }

//  Element Access
    const my_vector<V>& operator[](const K& key) const;
    my_vector<V>& operator[](const K& key);

//  Modifiers
    void insert(const K& k, const V& v);
    void erase(const K& key);
    void clear();

//  Operations:
    bool contains(const K& key);
    my_vector<V> &get(const K& key);

    bool find(const K& key);
    int count(const K& key);
    // I have not writtent hese yet, but we will need them:
    //    int count(const K& key);
    //    lower_bound
    //    upper_bound
    //    equal_range:

    bool is_valid(){
        return mmap.is_valid();
    }

    friend ostream& operator<<(ostream& outs, const MMap<K, V>& print_me){
        outs<<print_me.mmap<<endl;
        return outs;
    }

private:

    BTree< MPair<K, V> > mmap;

};


template <typename K, typename V>
void MMap<K,V>::insert(const K& k, const V& v){

    MPair<K, V> p(k,v);
    mmap.insert(p);
}

template <typename K, typename V>
const my_vector<V>& MMap<K,V>::operator[](const K& key) const{
    MPair<K,V> temp(key);
    MPair<K,V>* p= mmap.find(temp);
    return p->value_list;
}

template <typename K, typename V>
my_vector<V>& MMap<K,V>::operator[](const K& key){
    MPair<K,V> temp(key);
    MPair<K,V>* p= mmap.find(temp);
    return p->value_list;
}

//  Modifiers
template <typename K, typename V>
void MMap<K,V>::erase(const K& key){
    MPair<K,V> p(key);
    mmap.remove(p);
}

template <typename K, typename V>
void MMap<K,V>::clear(){
    mmap.clear_tree();
}

//  Operations:
template <typename K, typename V>
bool MMap<K,V>::contains(const K& key){
    MPair<K,V> p(key);
    return mmap.contains(p);
}

template <typename K, typename V>
my_vector<V>& MMap<K,V>::get(const K& key){
    MPair<K,V> temp(key);
    MPair<K,V> p= mmap.get(temp);
    return p.value_list;
}

template <typename K, typename V>
bool MMap<K,V>::find(const K& key){
    MPair<K,V> temp(key);
    MPair<K,V>* p= mmap.find(temp);
    if(p)
        return true;
    else
        return false;
}
template <typename K, typename V>
int MMap<K,V>:: count(const K& key){
    MPair<K,V> temp(key);
    temp=mmap.get(temp);
    return temp.value_list.size();
}

#endif // MULTI_MAP_H
