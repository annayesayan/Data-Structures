#ifndef DOUBLE_HASH_H
#define DOUBLE_HASH_H
#include <iostream>
#include "../../../Cs8/!includes/hash_classes/record.h"
using namespace std;

template <typename T>
class Double_Hash
{
public:
    //COTR
    Double_Hash();

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

    friend ostream& operator <<(ostream& outs, const Double_Hash& print_me){
        print_me.print();
        return outs;
    }

private:
    // MEMBER CONSTANTS -- These are used in the key field of special records.
    static const int NEVER_USED = -1;
    static const int PREVIOUSLY_USED = -2;

    // MEMBER VARIABLES
    record<T> _data[CAPACITY];
    int _used;

    // HELPER FUNCTIONS
    int hash(int key) const;
    int hash2(int key) const;
    int next_index(int index) const;
    void find_index(int key, bool& found, int& index) const;
    bool never_used(int index) const;
    bool is_vacant(int index) const;
};

template <typename T>
Double_Hash<T>::Double_Hash(){
    for(int i=0; i<CAPACITY; i++){
        _data[i]._key=NEVER_USED;
    }
    _used=0;
}

// MODIFICATION MEMBER FUNCTIONS
template <typename T>
void Double_Hash<T>::insert(const int key, const T& entry){
    bool already_present; // True if entry.key is already in the table
    int index; // data[index] is location for the new entry

    assert(key >= 0);
    // Set index so that data[index] is the spot to place the new entry.

    find_index(key, already_present, index);
    // If the key wasn’t already there, then find the location for the new entry.

    if (!already_present){
        assert(size( ) < CAPACITY);
        index = hash(key);
        while (!is_vacant(index)){
            int temp=hash2(key)+index;
            index=hash(temp);
        }
        ++_used;
    }
    _data[index]._key=key; _data[index]._value=entry;
}

template <typename T>
void Double_Hash<T>::remove(int key){
    bool found; // True if key occurs somewhere in the table
    int index; // Spot where data[index].key == key
    assert(key >= 0);
    find_index(key, found, index);
    if (found) {
        // The key was found, so remove this record and reduce used by 1.
        _data[index]._key = PREVIOUSLY_USED; // Indicates a spot that’s no longer in use.
        _data[index]._value =T();
        --_used;
    }
}

// CONSTANT MEMBER FUNCTIONS
template <typename T>
bool Double_Hash<T>::is_present(int key) const{
    bool found;
    int index;
    assert(key >= 0);
    find_index(key, found, index);
    return found;
}

template <typename T>
void Double_Hash<T>::find(int key, bool& found, T& result) const{
    int index;
    assert(key>=0);
    find_index(key, found, index);
    if (found){
        result = _data[index]._value;
    }
}

template <typename T>
int Double_Hash<T>:: hash(int key) const{
    return key % CAPACITY;
}

template <typename T>
int Double_Hash<T>:: hash2(int key) const{
    return 1 + (key % (CAPACITY-2));
}

template <typename T>
int Double_Hash<T>:: next_index(int index) const{
    if(index+1<CAPACITY)
        return index+1;
    else
        return 0;
}

template <typename T>
void Double_Hash<T>:: find_index(int key, bool& found, int& index) const{
    int count = 0;
    index = hash(key);
    while((count < CAPACITY) && (!never_used(index)) && (_data[index]._key != key)) {
        ++count;
        int temp=hash2(key)+index;
        index=hash(temp);
    }
    found = (_data[index]._key == key);
}

template <typename T>
bool Double_Hash<T>:: never_used(int index) const{
    return (_data[index]._key==NEVER_USED);
}

template <typename T>
bool Double_Hash<T>:: is_vacant(int index) const{
    return (_data[index]._key==NEVER_USED);
}

template <typename T>
void Double_Hash<T>:: print() const{
    for(int i=0; i<CAPACITY; i++){
        cout<<"[" << i<<"] "<<_data[i]<<endl;
    }
}



















#endif // DOUBLE_HASH_H
