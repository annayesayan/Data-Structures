#ifndef QUEUE_H
#define QUEUE_H
#include "../../../Cs8/!includes/linked_list/linked_list_functions.h"

template <class T>
class Queue  //first in first out
{
public:
    class Iterator{
    public:
        friend class Queue;                              //give access to list to access _ptr
        Iterator(): _ptr(NULL) {}                                   //default ctor
        Iterator(node<T>* p): _ptr(p){}
        operator bool();                                //casting operator: true if _ptr not NULL
                                       //casting operator: true if _ptr not NULL


        T operator *(){
            return *_ptr;
        }

        T* operator ->(){ //address
            T* p=&_ptr;
            return p;
        }

        bool is_null(){
            return _ptr==NULL;
        }                                 //true if _ptr is NULL

        friend bool operator !=(const Iterator& left,const Iterator& right){
            return left._ptr!=right._ptr;
        }

        friend bool operator ==(const Iterator& left,const Iterator& right){
            return left._ptr==right._ptr;
        }

        Iterator& operator++(){
            _ptr=++_ptr;
            return *this;
        }//++it;

        friend Iterator operator++(Iterator& it,int unused){
            Iterator p=it;
            it._ptr=++it._ptr;
            return p;
        }//it++

    private:
       node<T>* _ptr;
    };

    Queue();
    ~Queue();
    Queue(const Queue<T> &copyThis);
    Queue& operator =(const Queue& RHS){
        head=CopyList(RHS.head);
        return *this;
    }

    bool empty();
    typename Queue<T>::Iterator push(const T item);
    T pop();
    Iterator front();
    Iterator end();

    template <class U>
    friend ostream& operator <<(ostream& outs, const Queue<U>& l){
        _print_list(l.head, outs);
        return outs;
    }

private:
    node<T>* head;
    node<T>* tail;
};

template <class T>
Queue<T>:: Queue():head(NULL), tail(NULL){
}

template <class T>
Queue<T>::~Queue(){
    ClearList(head);
}

template <class T>
Queue<T>::Queue(const Queue<T> &copyThis){
    head=CopyList(copyThis.head);
}

template <class T>
bool Queue<T>:: empty(){
    return (head==NULL);
}

template <class T>
typename Queue<T>::Iterator Queue<T>::push(const T item){
    if(empty())
        tail = _insert_head(head, item);
    else
        tail = _insertAfter(head,tail,item);
    return tail;
}

template <class T>
T Queue<T>::pop(){
    T element = DeleteNode(head,head);
    return element;
}

template <class T>
typename Queue<T>::Iterator Queue<T>::front(){
    return Iterator(head);
}

template <class T>
typename Queue<T>::Iterator Queue<T>::end(){
    return Iterator(tail);
}

#endif // QUEUE_H
