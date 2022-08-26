#ifndef RECORD_H
#define RECORD_H
#include <iostream>
using namespace std;

template <typename T>
struct record
{
    int _key;
    T _value;

    record(int k=0, T v=T()):_key(k), _value(v){}
    friend bool operator ==(const record& left, const record& right){
        return (left._key==right._key);
    }
    friend ostream& operator <<(ostream& outs, const record& print_me){
        if(print_me._key>=0)
            outs<<print_me._key<<":"<<print_me._value;
        else if(print_me._key==-2)
            outs<<"----------------"<<endl;
        else
            outs<<endl;
        return outs;
    }
};
#endif // RECORD_H
