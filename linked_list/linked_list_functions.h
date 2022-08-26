#ifndef LINKED_LIST_FUNCTIONS_H
#define LINKED_LIST_FUNCTIONS_H
#include <iostream>
using namespace std;


template <class T>
struct node{
    node(T item=T(), node<T>* next = NULL, node<T>* prev = NULL):_item(item), _next(next), _prev(prev){
    }

    friend std::ostream& operator <<(std::ostream& outs,const node<T> &printMe){
        outs<<"["<<printMe._item<<"]->";
        return outs;
    }
    T _item;
    node<T>* _next;
    node<T>* _prev;
};


template <typename T>
node<T>* _insert_head (node<T>*& head, const T& insertThis){
    bool debug = false;
    if (debug){
        cout<<"in insert head function"<<endl;
        cout<<"item to be inserted is: "<<insertThis<<endl;
    }
    node<T>* temp = new node<T>(insertThis,head);

    if(head!=NULL){ //&& head->_next!=NULL
        head->_prev=temp;
        //cout<<"prev is: "<<*head->_prev<<endl;
    }
    head=temp;

    return temp;
}


template <typename T>
node<T>* _insertAfter(node<T>*& head, node<T>* afterThis, T insertThis){
    node<T>* temp = _insert_head(afterThis->_next,insertThis);
    temp->_prev=afterThis;
    //cout<<"prev is: "<<*temp->_prev<<endl;
    return temp;
}


template <typename T>
node<T>* PreviousNode(node<T>* head, node<T>* prevToThis){
//    node<T>* walker=head;
//    assert(walker!=NULL);
//    if(walker==prevToThis)
//        return NULL;
//    while(walker!=NULL && walker->_next!=NULL){
//        if(walker->_next==prevToThis)
//            return walker;
//        walker=walker->_next;
//    }
//    assert(!walker);
    assert(head!=NULL);
    return prevToThis->_prev;
}


template <typename T>
node<T>* _insertBefore(node<T>*& head, node<T>* beforeThis, T insertThis){
    node<T>* p=PreviousNode(head, beforeThis);
    if(p==NULL){
        return _insert_head(head,insertThis);
    }
    else
        return _insertAfter(head,p,insertThis);
}

template <typename T>
node<T>* SearchList(node<T>* head, T key){
    node<T>* walker=head;
    while(walker!=NULL){
        if(walker->_item==key)
            return walker;
        walker=walker->_next;
    }
    return NULL;
}


template <typename T>
T DeleteNode(node<T>*&head, node<T>* deleteThis){
    if(head!=NULL){
        node<T>* p=PreviousNode(head, deleteThis);
        T item=deleteThis->_item;
        if(p==NULL){
            //cout<<"prev node is null"<<endl;
            head=deleteThis->_next;
            if(deleteThis->_next!=NULL)
                deleteThis->_next->_prev=NULL;
        }else{
            //cout<<"prev node is not null"<<endl;
            p->_next=deleteThis->_next;
            if(deleteThis->_next)
                deleteThis->_next->_prev=p;
        }

        delete deleteThis;
        return item;
    }
}

template <typename T>
node<T>* CopyList(node<T>* head){
    node<T>* walker=head;
    node<T>* head2=new node<T>(walker->_item);
    node<T>* temp=head2;
    walker=walker->_next;
    while(walker!=NULL){
        temp=_insertAfter(head2, temp,walker->_item);
        walker=walker->_next;
    }
    return head2;
}

template <typename T>
void ClearList(node<T>*& head){
    node<T>* walker;
    while(head!=NULL){
        walker=head;
        head = walker->_next;
        delete walker;
    }
}

template <typename T>
node<T>* _LastNode(node<T>* head){
    node<T>* walker=head;
    while(walker!=NULL){
        if(walker->_next==NULL)
            return walker;
        else
            walker=walker->_next;
    }
    assert(head==NULL);
    return walker;
}

template <typename T>
T& At(node<T>* head, int pos){
    assert(head!=NULL);
    node<T>* walker=head;
    for(int i=1; i<pos; i++){
        walker=walker->_next;
        assert(walker!=NULL);
    }
    return walker->_item;
}

template <class T>
void _print_list(node<T>* head_ptr, ostream& outs){
    node<T>* walker = head_ptr;
    outs<<"H->";
//    if(walker==NULL){
//        cout<<"walker = null in print list func"<<endl;
//    }
    while (walker!=NULL){
        outs<<*walker;
        walker = walker->_next;
    }
    outs<<"|||"<<endl;
}

template <class T>
void print_backwards(node<T>* head_ptr, ostream& outs){
    node<T>* walker = _LastNode(head_ptr);
    cout<< "last node is: "<< *walker<<endl;
    outs<<"||| ";
    if(walker==NULL){
        cout<<"walker = null in print list func"<<endl;
    }
    while (walker!=NULL){
        outs<<*walker;
        walker = walker->_prev;
        cout<<"here"<<endl;
        //outs<<"walker prev:"<<*walker;
    }
}

#endif // LINKED_LIST_FUNCTIONS_H
