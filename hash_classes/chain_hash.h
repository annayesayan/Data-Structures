#ifndef CHAIN_HASH_H
#define CHAIN_HASH_H
#include "../../../Cs8/!includes/hash_classes/record.h"
#include "../../!includes/linked_list/list_class.h"

#include <iostream>
using namespace std;

template <typename T>
class Chain_Hash
{
public:
    //COTR
    Chain_Hash();

    // MEMBER CONSTANT -- See Appendix E if this fails to compile.
    static const int CAPACITY = 10;

    // MODIFICATION MEMBER FUNCTIONS
    void insert(const int key, const T& entry);
    void remove(int key);

    void print() const;
    // CONSTANT MEMBER FUNCTIONS
    bool is_present(int key) const;
    void find(int key, bool& found, T& result) const;
    int size( ) const { return _used; }

    friend ostream& operator <<(ostream& outs, const Chain_Hash& print_me){
        print_me.print();
        return outs;
    }

private:
    // MEMBER VARIABLES
    List<record<T>> _data[CAPACITY];
    int _used;

    // HELPER FUNCTIONS
    int hash(int key) const;
    void find_index(record<T> rec, bool& found, int& index) const;
};

//-------------------------------------------------------------[ FUNCTIONS ]---

template <typename T>
Chain_Hash<T>::Chain_Hash(){
    _used=0;
}

// MODIFICATION MEMBER FUNCTIONS
template <typename T>
void Chain_Hash<T>::insert(const int key, const T& entry){
    bool already_present; // True if entry.key is already in the table
    int index; // data[index] is location for the new entry

    assert(key >= 0);

    record<T> rec(key,entry);
    find_index(rec, already_present, index);

    if (!already_present){
        index = hash(key);
        _data[index].InsertHead(rec);
        ++_used;
    }else{
        typename List< record<T> > :: Iterator it=_data[index].Search(rec);
        *it=rec;
    }
}

template <typename T>
void Chain_Hash<T>::remove(int key){
    bool found;
    int index;
    assert(key >= 0);
    record<T> rec(key);
    find_index(rec, found, index);
    if (found) {
        typename List< record<T> > :: Iterator it=_data[index].Search(rec);
        _data[index].Delete(it);
        --_used;
    }
}

// CONSTANT MEMBER FUNCTIONS
template <typename T>
bool Chain_Hash<T>::is_present(int key) const{
    bool found;
    int index;
    assert(key >= 0);
    record<T> rec(key);
    find_index(rec, found, index);
    return found;
}

template <typename T>
void Chain_Hash<T>::find(int key, bool& found, T& result) const{
    int index;
    assert(key>=0);
    record<T> rec(key);
    find_index(rec, found, index);
    if (found){
        typename List< record<T> > :: Iterator it=_data[index].Search(rec);
        result = it->_value;
    }
}

template <typename T>
int Chain_Hash<T>:: hash(int key) const{
    return key % CAPACITY;
}

template <typename T>
void Chain_Hash<T>:: find_index(record<T> rec, bool& found, int& index) const{
    index = hash(rec._key);
    typename List< record<T> > :: Iterator it=_data[index].Search(rec);
    if(it!=typename List< record<T> > :: Iterator())
        found=true;
    else
        found=false;
}

template <typename T>
void Chain_Hash<T>:: print() const{
    for(int i=0; i<CAPACITY; i++){
        cout<<"[" << i<<"] "<<_data[i];
    }
}
#endif // CHAIN_HASH_H
