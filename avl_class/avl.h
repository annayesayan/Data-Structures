#ifndef AVL_H
#define AVL_H
#include <iostream>
#include "../../!includes/avl_functions/avl_functions.h"
using namespace  std;

template <typename T>
class AVL{
    public:
         AVL();
         AVL(const T* sorted_list, int size=-1); //not done

         //. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
         AVL(const AVL<T>& copy_me);
         AVL<T>& operator =(const AVL<T>& rhs){
             root=tree_copy(rhs.root);
             return *this;
         }
         ~AVL();
         //. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
         void insert(const T& insert_me);
         void erase(const T& target);
         void clear();
         bool search(const T& target, tree_node<T>* & found_ptr);
         bool is_valid();

         friend ostream& operator <<(ostream& outs, const AVL<T>& tree){
             tree_print(tree.root);
             return outs;
         }
         AVL<T>& operator +=(const AVL<T>& rhs);
    private:
         tree_node<T>* root;
};


template <typename T>
AVL<T>::AVL(){
    root=NULL;
}

template <typename T>
AVL<T>:: AVL(const T* sorted_list, int size){

}

template <typename T>
AVL<T>:: AVL(const AVL<T>& copy_me){
    root=tree_copy(copy_me.root);
}

template <typename T>
AVL<T>::~AVL(){
    tree_clear(root);
}

template <typename T>
void AVL<T>:: insert(const T& insert_me){
    tree_insert(root,insert_me);
}

template <typename T>
void AVL<T>::erase(const T& target){
    if(tree_erase(root,target)){
        cout<<"item erasing!"<<endl;
    }
    else
        cout<<"item does not exist!"<<endl;
}

template <typename T>
bool AVL<T>:: search(const T& target, tree_node<T>* & found_ptr){
   found_ptr=tree_search(root,target);
   if(found_ptr){
       cout<<"item found: "<<found_ptr->_item<<endl;
       return true;
   }
   else{
       cout<<"item not found "<<endl;
       return false;
   }
}

template <typename T>
void AVL<T>::clear(){
    tree_clear(root);
}

template <typename T>
bool AVL<T>:: is_valid(){
    return avl_isvalid(root);
}

#endif // AVL_H
