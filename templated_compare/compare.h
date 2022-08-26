#ifndef COMPARE_H
#define COMPARE_H
#include <iostream>
#include <iomanip>
using namespace std;

template <typename T>
class Bigger{
public:
    bool operator() (const T& left, const T& right){
        return right<left;
    }
};

template <typename T>
class Smaller{
public:
    bool operator() (const T& left, const T& right){
        return left<right;
    }
};

template <typename T>
struct entry{
    entry(T i=T(), int s=0): _item(i), _serial_num(s){}
    T _item;
    int _serial_num;
    int current_serial(){
        return _serial_num;
    }
    entry& operator =(const entry& RHS){
        _item=RHS._item;
        _serial_num=RHS._serial_num;
        return *this;
    }
    friend ostream& operator <<(ostream& outs, const entry<T>& print_me){
        outs<<"#"<<print_me._serial_num<<"     "<<print_me._item;
        return outs;
    }

    friend bool operator <(const entry<T>& lhs, const entry<T>& rhs){
        if(lhs._item<rhs._item)
            return true;
        else if (lhs._item>rhs._item)
            return false;
        else
            return lhs._serial_num>rhs._serial_num;

    }

    friend bool operator >(const entry<T>& lhs, const entry<T>& rhs){
        if(lhs._item>rhs._item)
            return true;
        else if (lhs._item<rhs._item)
            return false;
        else
            return lhs._serial_num<rhs._serial_num;
    }

    friend bool operator ==(const entry<T>& lhs, const entry<T>& rhs){
        if(lhs._item==rhs._item && lhs._serial_num==rhs._serial_num)
            return true;
        else
            return false;
    }
};


#endif // COMPARE_H
