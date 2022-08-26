#ifndef PQUEUE_H
#define PQUEUE_H
#include "../../../Cs8/!includes/pq_info/info.h"
#include "../../../Cs8/!includes/heap_class/heap_class.h"
#include "../../../Cs8/!includes/templated_compare/compare.h"

template <class T >
class PQueue
{
public:
    PQueue();
    PQueue& operator =(const PQueue& RHS){
        heap=RHS.heap;
        return *this;
    }

    bool insert(const T& value, int p);
    T Pop();
    bool is_empty() const;
    int size() const;
    void print_tree() const;
    bool is_valid();

    friend ostream& operator <<(ostream& outs, PQueue<T>& print_me){
        //cout<<"pq <<"<<endl;
        print_me.heap.print_array();
        return outs;
    }

private:
    HEAP<info<T>> heap;
};

//--------------------------------DEFINITIONS-----------------------------------

template <class T >
PQueue<T>:: PQueue(){}

template <class T>
bool PQueue<T>:: insert(const T& value, int p){
    heap.insert(info<T>(value,p));
}

template <class T>
T PQueue<T>:: Pop(){
    assert(!is_empty());
    info<T> popped=heap.Pop();
    return popped.item;
}

template <class T >
bool PQueue<T>:: is_empty() const{
    return heap.is_empty();
}

template <class T>
int PQueue<T>:: size() const{
    return heap.size();
}

template <class T>
void PQueue<T>:: print_tree() const{
    cout<<heap;
}

template <class T>
bool PQueue<T>:: is_valid(){
    return heap.is_valid();
}

#endif // PQUEUE_H
