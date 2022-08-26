#ifndef BST_CLASS_H
#define BST_CLASS_H
#include <iostream>
#include "../../Cs8/!includes/bst_functions/bst_functions.h"
using namespace  std;

template <typename T>
class BST{
    public:
     BST();
     BST(const T* sorted_list, int size=-1);

     //. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
     BST(const BST<T>& copy_me);
     BST<T>& operator =(const BST<T>& rhs){
         root=tree_copy(rhs.root);
         return *this;
     }
     ~BST();
     //. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
     void insert(const T& insert_me);
     void erase(const T& target);
     bool search(const T& target, tree_node<T>* & found_ptr);

     friend ostream& operator <<(ostream& outs, const BST<T>& tree){
         tree_print(tree.root);
         return outs;
     }
     BST<T>& operator +=(const BST<T>& rhs);
    private:
     tree_node<T>* root;
};


template <typename T>
BST<T>::BST(){
    root=NULL;
}

template <typename T>
BST<T>:: BST(const T* sorted_list, int size){

}

template <typename T>
BST<T>:: BST(const BST<T>& copy_me){
    root=tree_copy(copy_me.root);
}

template <typename T>
BST<T>::~BST(){
    tree_clear(root);
}

template <typename T>
void BST<T>:: insert(const T& insert_me){
    tree_insert(root,insert_me);
}

template <typename T>
void BST<T>::erase(const T& target){
    if(tree_erase(root,target))
        cout<<"item erasing!"<<endl;
    else
        cout<<"item does not exist!"<<endl;
}

template <typename T>
bool BST<T>:: search(const T& target, tree_node<T>* & found_ptr){
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

#endif // BST_CLASS_H
