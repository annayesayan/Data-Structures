#ifndef STACK_H
#define STACK_H
#include "../../../Cs8/!includes/linked_list/linked_list_functions.h"
template <class T>
class Stack  //first in last out
{
public:
    class Iterator{
    public:
        friend class Stack;                              //give access to list to access _ptr
        Iterator(): _ptr(NULL) {}                                   //default ctor
        Iterator(node<T>* p): _ptr(p){}
        operator bool();                                //casting operator: true if _ptr not NULL


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

    Stack();
    ~Stack();
    Stack(const Stack<T> &copyThis);
    Stack& operator =(const Stack& RHS){
        head=CopyList(RHS.head);
        return *this;
    }

    bool empty();
    Iterator push(const T item);
    T pop();
    T front();
    Iterator end();

    template <class U>
    friend ostream& operator <<(ostream& outs, const Stack<U>& l){
       _print_list(l.head, outs);
        return outs;
    }
private:
   node<T>* head;
};


//DEFINITIONS
template <class T>
Stack<T>:: Stack():head(NULL){}

template <class T>
Stack<T>::~Stack(){
    ClearList(head);
}

template <class T>
Stack<T>::Stack(const Stack<T> &copyThis){
    head=CopyList(copyThis.head);
}

template <class T>
bool Stack<T>:: empty(){
    return (head==NULL);
}

template <class T>
typename Stack<T>::Iterator Stack<T>:: push(const T item){
   return _insert_head(head,item);
}

template <class T>
T Stack<T>::pop(){
    T element = DeleteNode(head,head);
    return element;
}

template <class T>
T Stack<T>::front(){
    T item = head->_item;
    return item;
}







#endif // STACK_H
