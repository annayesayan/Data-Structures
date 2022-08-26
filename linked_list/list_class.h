#ifndef LIST_CLASS_H
#define LIST_CLASS_H
#include <iostream>
#include "../../!includes/linked_list/linked_list_functions.h"

using namespace std;

template <class T>
class List
{
public:
    class Iterator{
    public:
        friend class List;                              //give access to list to access _ptr
        Iterator(): _ptr(NULL) {}                                   //default ctor
        Iterator(node<T>* p): _ptr(p){}                    //Point Iterator to where p is pointing to
        operator bool();                                //casting operator: true if _ptr not NULL
                                                        //      this turned out to be a pain!

        T& operator *(){
            return _ptr->_item;
        }

        T* operator ->(){ //address
            T* p=&(_ptr->_item);
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
            _ptr=_ptr->_next;
            return *this;
        }                         //member operator: ++it; or ++it = new_value

        friend Iterator operator++(Iterator& it,int unused){
            Iterator p=it;
            it._ptr=it._ptr->_next;
            return p;
        }         //it++

    private:
        node<T>* _ptr;                          //pointer being encapsulated
    };

    List();                                                     //CTOR
                                                                //BIG 3:
    ~List();
    List(const List<T> &copyThis);
    List& operator =(const List& RHS){
        head=CopyList(RHS.head);
        return *this;
    }

    Iterator InsertHead(T i);                           //insert at the head of list
    Iterator InsertAfter(T i, Iterator iMarker);        //insert after marker
    Iterator InsertBefore(T i, Iterator iMarker);       //insert before marker

    T Delete(List<T>::Iterator iMarker);        //delete node pointed to by marker
    void Print() const;
    Iterator Search(const T &key) const;                      //return Iterator to node [key]
    Iterator Prev(Iterator iMarker);                            //previous node to marker

    T& operator[](int index);                           //return item at position index
    Iterator Begin() const;                                     //Iterator to the head node
    Iterator End() const;                                     //Iterator to NULL

    Iterator LastNode() const;                                  //Iterator to the last node

    template <class U>                                          //Note the template arg U
    friend ostream& operator <<(ostream& outs, const List<U>& l){
        _print_list(l.head, outs);
        //print_backwards(l.head, outs);
        return outs;
    }
private:
    node<T>* head;
};

template <class T>
List<T>:: List():head(NULL){
}

template <class T>
List<T>::~List(){
    ClearList(head);
}

template <class T>
List<T>::List(const List<T> &copyThis){
    head=CopyList(copyThis.head);
}

template <class T>
typename List<T>::Iterator List<T>::Begin() const{
    return Iterator(head);
}

template <class T>
typename List<T>::Iterator List<T>::LastNode() const{
    return _LastNode(head);
}

template <class T>
typename List<T>::Iterator List<T>::End() const{
    return NULL;
}

template <class T>
typename List<T>::Iterator List<T>::InsertHead(T i){
    return _insert_head(head,i);
}

template <class T>
typename List<T>::Iterator List<T>:: InsertAfter(T i, Iterator iMarker){
     return _insertAfter(head,iMarker._ptr,i);
}

template <class T>
typename List<T>::Iterator List<T>:: InsertBefore(T i, Iterator iMarker){
    return _insertBefore(head,iMarker._ptr, i);
}

template <class T>
typename List<T>::Iterator List<T>:: Search(const T &key) const{
    return SearchList(head, key);
}

template <class T>
typename List<T>::Iterator List<T>:: Prev(Iterator iMarker){
    return PreviousNode(head,iMarker._ptr);
}

template <class T>
T List<T>:: Delete(List<T>::Iterator iMarker){
    return DeleteNode(head, iMarker._ptr);
}
template <class T>
T& List<T>::operator[](int index){
    return At(head, index);
}

template <class T>
void List<T>::Print() const{
    Iterator a=Begin();
    while (a!=End()) {
        cout<< *a<<" ";
        a++;
    }
}
#endif // LIST_CLASS_H
