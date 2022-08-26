#ifndef HEAP_CLASS_H
#define HEAP_CLASS_H
#include <iostream>
#include <iomanip>
#include "../../../CS3A/11P1_Vector_class/vector.h"
#include "../../../Cs8/!includes/heap_functions/heap_functions.h"
#include "../../../Cs8/!includes/templated_compare/compare.h"
using namespace std;

template <class T, typename COMPARE = Bigger<entry<T> > >
class HEAP{
    public:
        HEAP();
        HEAP& operator =(const HEAP& RHS){
            _v=RHS._v;
            return *this;
        }
        void insert(const T& insert_me);
        T Pop();
        bool is_empty() const;
        int size() const;
        int capacity() const;
        bool is_valid(int root);
        void print_tree(int root, int level = 0, ostream& outs = cout) const;
        void print_array() const;
        friend ostream& operator << (ostream& outs, const HEAP<T, COMPARE>& print_me){
            const bool debug=false;
            if(debug)               //print the array
                print_me.print_array();
            print_me.print_tree(0);
            return outs;
        }
    private:
        my_vector < entry<T> > _v;
        COMPARE cmp;
        int serial;
        int big_child_index(int i);
        void swap_with_parent(int i);
        void swap_with_child(int i);
        bool is_leaf(int i) const;
};

// ----------------------------DEFINITIONS--------------------------------------

template <class T, typename COMPARE >
HEAP<T, COMPARE>::HEAP(){
    serial=0;
}

template <class T, typename COMPARE >
void HEAP<T, COMPARE>:: insert(const T& insert_me){
    serial+=1;
    entry<T> it(insert_me,serial);    
    _v.push_back(it);
    int index=_v.size()-1;
    swap_with_parent(index);
}

template <class T, typename COMPARE >
T HEAP<T, COMPARE>:: Pop(){
    assert(!_v.empty()); //assert if the tree is empty
    entry<T> item=_v[0]; //T item=_v[0];
    int s=_v.size()-1;
    _v[0]=_v[s];
    entry<T> i; _v.remove_last_entry(i); //T i; _v.remove_last_entry(i);
    if(!is_empty())        //if its not empty
        swap_with_child(0);
    return item._item;
}

template <class T, typename COMPARE >
int HEAP<T, COMPARE>:: big_child_index(int i){
//    if(_v[right_child_index(i)]> _v[left_child_index(i)])

    if(left_child_index(i)>=size())
        return right_child_index(i);
    else if(right_child_index(i)>=size())
        return left_child_index(i);
    else if(cmp(_v[right_child_index(i)],_v[left_child_index(i)]))
        return right_child_index(i);
    else
        return left_child_index(i);
}

template <class T, typename COMPARE >
void HEAP<T, COMPARE>:: swap_with_parent(int i){
    //while(parent_index(i) >-1 && _v[parent_index(i)] < _v[i]){

    while(parent_index(i) >-1 && cmp(_v[i],_v[parent_index(i)])){
        entry<T> temp=_v[parent_index(i)];//T temp=_v[parent_index(i)];
        _v[parent_index(i)]=_v[i];
        _v[i]=temp;
        i=parent_index(i);
    }
}

template <class T, typename COMPARE >
void HEAP<T, COMPARE>:: swap_with_child(int i){
    //while(!is_leaf(i) && _v[i]<_v[big_child_index(i)]){

        while(!is_leaf(i) && cmp(_v[big_child_index(i)], _v[i])){
            int big=big_child_index(i);
            entry<T> temp = _v[i];     //T temp = _v[i];
            _v[i]=_v[big];
            _v[big]=temp;
            i=big;
        }

}

template <class T, typename COMPARE >
bool HEAP<T, COMPARE>:: is_leaf(int i) const{
    if(left_child_index(i)>=size())
        return true;
    else
        return false;
}

template <class T, typename COMPARE >
bool HEAP<T, COMPARE>:: is_empty() const{
    return _v.empty();
}
template <class T, typename COMPARE >
int HEAP<T, COMPARE>::size() const{
    return _v.size();
}

template <class T, typename COMPARE >
int HEAP<T, COMPARE>::capacity() const{
    return _v.capacity();
}

template <class T, typename COMPARE >
bool HEAP<T, COMPARE>::is_valid(int root){


    if(is_leaf(root))
        return true;

//    else if(_v[root] > _v[left_child_index(root)] && _v[root] > _v[right_child_index(root)]
//            && is_valid(left_child_index(root)) && is_valid(right_child_index(root)))

    else if(cmp(_v[root], _v[big_child_index(root)]) && is_valid(left_child_index(root))
            && is_valid(right_child_index(root)))
        return true;   
    else
        return false;
}

template <class T, typename COMPARE >
void HEAP<T, COMPARE>:: print_tree(int root, int level, ostream& outs) const{
    if(root<size()){
        print_tree(right_child_index(root), level+1);
        outs<<setw(4*level)<<"["<< _v[root] <<"]"<<endl;
        print_tree(left_child_index(root),level+1);
    }
}

template <class T, typename COMPARE >
void  HEAP<T, COMPARE>:: print_array() const{
    cout<<_v<<endl;
}

#endif // HEAP_CLASS_H
