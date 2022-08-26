#ifndef INFO_H
#define INFO_H
#include <iostream>
#include <iomanip>
#include "../../../Cs8/!includes/templated_compare/compare.h"

using namespace std;

template <typename T>
struct info{
    T item;
    int priority;

    info(T i=T(), int p=0): item(i), priority(p){}

    friend ostream& operator <<(ostream& outs, const info<T>& print_me){
        const bool debug=true;
        if(debug){
        outs<< left<<setfill(' ')<<setw(10)<< print_me.item
            <<right<<setfill(' ')<<right<<setw(2)<<"( "<<print_me.priority<<" )"<<endl;
        }
        else
            outs<<print_me.item;
        return outs;
    }

    friend bool operator <(const info<T>& lhs, const info<T>& rhs){
        if(lhs.priority<rhs.priority)
            return true;
        else
            return false;
    }

    friend bool operator >(const info<T>& lhs, const info<T>& rhs){
        if(lhs.priority>rhs.priority)
            return true;
        else
            return false;
    }

    friend bool operator ==(const info<T>& lhs, const info<T>& rhs){
        if(lhs.priority==rhs.priority && lhs.item==rhs.item)
            return true;
        else
            return false;
    }
};

#endif // INFO_H
