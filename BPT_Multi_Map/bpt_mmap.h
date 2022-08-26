#ifndef BPT_MMAP_H
#define BPT_MMAP_H
#include <iostream>
#include "../../../Cs8/!includes/BPTree_class/bptree_class.h"
//#include "../../../CS3A/11P1_Vector_class/vector.h"
#include <vector>

using namespace std;

//template <typename K, typename V>
//struct MPair{
//    K key;
//    my_vector<V> value_list;


//    //--------------------------------------------------------------------------------
//    /*      MPair CTORs:
//     *  must have these CTORs:
//     *  - default CTOR / CTOR with a key and no value: this will create an empty vector
//     *  - CTOR with a key AND a value: pushes the value into the value_list
//     *  _ CTOR with a key and a vector of values (to replace value_list)
//     */
//    //--------------------------------------------------------------------------------
//    MPair(const K& k=K()){
//        key=k;
//    }
//    MPair(const K& k, const V& v){
//        key=k;
//        value_list.push_back(v);
//    }

//    MPair(const K& k, const my_vector<V>& vlist){
//        key=k;
//        value_list=vlist;
//    }

//    //--------------------------------------------------------------------------------

//    //You'll need to overlod << for your vector:
//    friend ostream& operator <<(ostream& outs, const MPair<K, V>& print_me){
//        outs<<"("<<print_me.key<<")"<<print_me.value_list<<"!";
//        return outs;
//    }

//    friend bool operator ==(const MPair<K, V>& lhs, const MPair<K, V>& rhs){
//        return lhs.key==rhs.key;
//    }
//    friend bool operator < (const MPair<K, V>& lhs, const MPair<K, V>& rhs){
//        return lhs.key<rhs.key;
//    }
//    friend bool operator <= (const MPair<K, V>& lhs, const MPair<K, V>& rhs){
//        return lhs.key<=rhs.key;
//    }
//    friend bool operator >= (const MPair<K, V>& lhs, const MPair<K, V>& rhs){
//        return lhs.key>=rhs.key;
//    }
//    friend bool operator > (const MPair<K, V>& lhs, const MPair<K, V>& rhs){
//        return lhs.key>rhs.key;
//    }
//    friend MPair<K, V> operator + (const MPair<K, V>& lhs, const MPair<K, V>& rhs){
//         MPair<K, V> result(lhs.key, lhs.value_list+rhs.value_list);
//         return result;
//    }
//};

//template <typename K, typename V>
//class MMap
//{
//public:
//    typedef BPlusTree< MPair<K, V> > map_base;
//        class Iterator{
//        public:
//            friend class MMap;
//            Iterator(typename map_base::Iterator it=typename map_base::Iterator()):
//                _it(it){}
//            Iterator operator ++(int unused){
//                return _it++;
//            }
//            Iterator operator ++(){
//                return ++_it;
//            }
//            MPair<K, V> operator *(){
//                 return *_it;
//            }
//            friend bool operator ==(const Iterator& lhs, const Iterator& rhs){
//                return lhs._it==rhs._it;
//            }
//            friend bool operator !=(const Iterator& lhs, const Iterator& rhs){
//                return lhs._it!=rhs._it;
//            }
//        private:
//            typename map_base::Iterator _it;
//        };

//    MMap():mmap(true){
//    }
////  Iterators
//    Iterator begin(){
//        return mmap.begin();
//    }
//    Iterator end(){
//        return mmap.end();
//    }
//    Iterator lower_bound(const K& key);
//    Iterator upper_bound(const K& key);

////  Capacity
//    int size() const{
//        return mmap.size();
//    }
//    bool empty() const{
//        return mmap.empty();
//    }

////  Element Access
//    const my_vector<V> operator[](const K& key) const;
//    my_vector<V> operator[](const K& key);


////  Modifiers
//    void insert(const K& k, const V& v);
//    void erase(const K& key);
//    void clear();

////  Operations:
//    bool contains(const K& key);
//    my_vector<V> &get(const K& key);


//    Iterator find(const K& key);
//    int count(const K& key);
//    // I have not writtent hese yet, but we will need them:
//    //    int count(const K& key);
//    //    lower_bound
//    //    upper_bound
//    //    equal_range:

//    bool is_valid(){
//        return mmap.is_valid();
//    }

//    friend ostream& operator<<(ostream& outs, const MMap<K, V>& print_me){
//        outs<<print_me.mmap<<endl;
//        return outs;
//    }

//private:
//    BPlusTree< MPair<K, V> > mmap;
//};


//template <typename K, typename V>
//void MMap<K,V>::insert(const K& k, const V& v){
//    MPair<K, V> p(k,v);
//    mmap.insert(p);
//}

//template <typename K, typename V>
//const my_vector<V> MMap<K,V>::operator[](const K& key) const{
//    MPair<K,V> temp(key);
//    typename map_base::Iterator it= mmap.find(temp);
//    MPair<K,V> p=*it;
//    return p.value_list;
//}

//template <typename K, typename V>
//my_vector<V> MMap<K,V>::operator[](const K& key){
//    MPair<K,V> temp(key);
//    typename map_base::Iterator it= mmap.find(temp);
//    MPair<K,V> p=*it;
//    return p.value_list;
//}






////  Modifiers                         //finish after bpt is done
////template <typename K, typename V>
////void MMap<K,V>::erase(const K& key){
////    MPair<K,V> p(key);
////    mmap.remove(p);
////}

//template <typename K, typename V>
//void MMap<K,V>::clear(){
//    mmap.clear_tree();
//}


////  Operations:
//template <typename K, typename V>
//bool MMap<K,V>::contains(const K& key){
//    MPair<K,V> p(key);
//    return mmap.contains(p);
//}

//template <typename K, typename V>
//my_vector<V>& MMap<K,V>::get(const K& key){
//    MPair<K,V> temp(key);
//    MPair<K,V> p= mmap.get(temp);
//    return p.value_list;
//}

//template <typename K, typename V>
//typename MMap<K,V>::Iterator MMap<K,V>::find(const K& key){
//    MPair<K,V> temp(key);
//    return  mmap.find(temp);
//}

//template <typename K, typename V>
//int MMap<K,V>:: count(const K& key){
//    MPair<K,V> temp(key);
//    temp=mmap.get(temp);
//    return temp.value_list.size();
//}

//template <typename K, typename V>
//typename MMap<K,V>:: Iterator MMap<K,V>:: lower_bound(const K& key){
//    return mmap.lower_bound(key);
//}

//template <typename K, typename V>
//typename MMap<K,V>:: Iterator MMap<K,V>::upper_bound(const K& key){
//    return mmap.upper_bound(key);
//}




































template <typename K, typename V>
struct MPair{
    K key;
    vector<V> value_list;

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

    MPair(const K& k, const vector<V>& vlist){
        key=k;
        value_list=vlist;
    }
    //--------------------------------------------------------------------------------

    //You'll need to overlod << for your vector:
    friend ostream& operator <<(ostream& outs, const MPair<K, V>& print_me){
        outs<<"("<<print_me.key<<")"<<print_me.value_list<<"!";
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
    typedef BPlusTree< MPair<K, V> > map_base;
        class Iterator{
        public:
            friend class MMap;
            Iterator(typename map_base::Iterator it=typename map_base::Iterator()):
                _it(it){}
            Iterator operator ++(int unused){
                return _it++;
            }
            Iterator operator ++(){
                return ++_it;
            }
            MPair<K, V> operator *(){
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

    MMap():mmap(true){
    }
//  Iterators
    Iterator begin(){
        return mmap.begin();
    }
    Iterator end(){
        return mmap.end();
    }
    Iterator lower_bound(const K& key);
    Iterator upper_bound(const K& key);

//  Capacity
    int size() const{
        return mmap.size();
    }
    bool empty() const{
        return mmap.empty();
    }

//  Element Access
    const vector<V> operator[](const K& key) const; //changed w/o &
    vector<V>& operator[](const K& key); //changed with, was other way around

//  Modifiers
    void insert(const K& k, const V& v);
    void erase(const K& key);
    void clear();

//  Operations:
    bool contains(const K& key);
    vector<V> &get(const K& key);

    Iterator find(const K& key);
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
    BPlusTree< MPair<K, V> > mmap;
};


template <typename K, typename V>
void MMap<K,V>::insert(const K& k, const V& v){
    MPair<K, V> p(k,v);
    mmap.insert(p);
}


template <typename K, typename V>
const vector<V> MMap<K,V>::operator[](const K& key) const{
//    MPair<K,V> temp(key);
//    typename map_base::Iterator it= mmap.find(temp);
//    MPair<K,V> p=*it;
//    return p.value_list;
    return mmap.find_ptr(MPair<K, V>(key, V()))->value_list;
}

//template <typename K, typename V>
//vector<V>& MMap<K,V>::operator[](const K& key){ //added &
//    MPair<K,V> temp(key);
//    typename map_base::Iterator it= mmap.find(temp);
//    MPair<K,V> p=*it;
//    return p.value_list;
//}

template <typename K, typename V>
vector<V>& MMap<K, V>::operator[](const K& key)
{
   if(!contains(key))
   {
       mmap.insert(MPair<K, V>(key));

   }
   return mmap.find_ptr(MPair<K, V>(key, V()))->value_list;

}


//  Modifiers                         //finish after bpt is done
//template <typename K, typename V>
//void MMap<K,V>::erase(const K& key){
//    MPair<K,V> p(key);
//    mmap.remove(p);
//}

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
vector<V>& MMap<K,V>::get(const K& key){
    MPair<K,V> temp(key);
    MPair<K,V> p= mmap.get(temp);
    return p.value_list;
}

template <typename K, typename V>
typename MMap<K,V>::Iterator MMap<K,V>::find(const K& key){
    MPair<K,V> temp(key);
    return  mmap.find(temp);
}

template <typename K, typename V>
int MMap<K,V>:: count(const K& key){
    MPair<K,V> temp(key);
    temp=mmap.get(temp);
    return temp.value_list.size();
}

template <typename K, typename V>
typename MMap<K,V>:: Iterator MMap<K,V>:: lower_bound(const K& key){
    return mmap.lower_bound(key);
}

template <typename K, typename V>
typename MMap<K,V>:: Iterator MMap<K,V>::upper_bound(const K& key){
    return mmap.upper_bound(key);
}

#endif // BPT_MMAP_H
