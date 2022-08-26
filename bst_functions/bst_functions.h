#ifndef BST_FUNCTIONS_H
#define BST_FUNCTIONS_H
#include <iostream>
#include <iomanip>
using namespace  std;


template <typename T>
struct tree_node{
    T _item;
    tree_node<T>* _left;
    tree_node<T>* _right;
    int _height;
    int balance_factor(){
        //balance factor = height of the right subtree
        //                        - the height of the left subtree
        //a NULL child has a height of -1
        //a leaf has a height of 0
        return (_right->_height-_left->_height);
    }

    int height(){
        // Height of a node is 1 + height of the "taller" child
        //A leaf node has a height of zero: 1 + max(-1,-1)
        if(!_right && !_left)
            return 0;
        else if((_right && !_left) || (!_right && _left))
            return -1;
        else
            return 1 + max(_right->_height,_left->_height);
    }

    int update_height(){
        _height=height();
    }

    tree_node(T item=T(), tree_node* left=NULL,
                          tree_node* right=NULL):
                    _item(item), _left(left), _right(right)
    {

        //don't forget to set the _height.
    }
    friend ostream& operator <<(ostream& outs,
                                const tree_node<T>& t_node){
        outs<<"|"<<t_node._item<<"|";
    }
};


template <typename T>
void tree_insert(tree_node<T>* &root, const T& insert_me);
template <typename T>
tree_node<T>* tree_search(tree_node<T>* root, const T& target);
template<typename T>
void tree_print(tree_node<T>* root, int level=0, ostream& outs=cout);
template <typename T>       //return copy of tree pointed to by root
tree_node<T>* tree_copy(tree_node<T>* root);
template <typename T>       //clear the tree
void tree_clear(tree_node<T>* &root);
template <typename T>       //erase target from the tree
bool tree_erase(tree_node<T>*& root, const T& target);
template <typename T>       //erase rightmost node from the tree
                            // store the item in max_value
void tree_remove_max(tree_node<T>* &root, T& max_value);
template<typename T>       //prints detailes info about each node
void tree_print_debug(tree_node<T>* root, int level=0, ostream& outs=cout);
template <typename T>       //Add tree src to dest
void tree_add(tree_node<T>* & dest, const tree_node<T>* src);
template <typename T>       // sorted array -> tree
tree_node<T>* tree_from_sorted_list( T* a, int size,tree_node<T>* &root);

/*----------------------DEFINITIONS-------------------------------------------*/

template <typename T>
void tree_insert(tree_node<T>* &root, const T& insert_me){
    if(root==NULL){
        root= new tree_node<T>(insert_me);
    }else if(insert_me < root->_item){
        tree_insert(root->_left, insert_me);
    }else{
        tree_insert(root->_right, insert_me);
    }
    root->update_height();
}

template <typename T>
tree_node<T>* tree_search(tree_node<T>* root, const T& target){
    const bool debug = false;
    if(!root){
        if(debug)
            cout<<"tree-search: "<<"Not found root is NULL"<<endl;
        return root;
    }
    if(root->_item ==target){
        if(debug)
            cout<<"tree-search: "<<"found target: ["<< root->_item<< "]"<<endl;
        return root;
    }
    if(target< root->_item){
        if(debug)
            cout<<"tree search: going left"<<endl;
        return tree_search(root->_left, target);
    }
    if(target> root->_item){
        if(debug)
            cout<<"tree search: going right"<<endl;
        return tree_search(root->_right, target);
    }
    cout<<"How did i get here??!!"<<endl;
}


template<typename T>
void tree_print(tree_node<T>* root, int level, ostream& outs){
    if(root){
        tree_print(root->_right, level+1);
        outs<<setw(4*level)<<"["<<root->_item<<"]"<<endl;
        tree_print(root->_left,level+1);
    }
}

template <typename T>       //return copy of tree pointed to by root
tree_node<T>* tree_copy(tree_node<T>* root){
    if(root){
        return new tree_node<T>(root->_item, tree_copy(root->_left), tree_copy(root->_right));
    }else
        return NULL;
}

template <typename T>       //clear the tree
void tree_clear(tree_node<T>* &root){
    if(root){
        tree_clear(root->_left);
        tree_clear(root->_right);
        delete root;
        root=NULL;
    }
}


template <typename T>       //erase target from the tree
bool tree_erase(tree_node<T>*& root, const T& target){
    const bool debug = true;
    if(!root){
        if(debug)
            cout<<"Target doesnt exist, root is null"<<endl;
        return false;
    }
    if(target< root->_item){
        if(debug)
            cout<<"tree erase: going left"<<endl;
        return tree_erase(root->_left, target);
    }
    if(target> root->_item){
        if(debug)
            cout<<"tree erase: going right"<<endl;
        return tree_erase(root->_right, target);
    }
    if(target==root->_item){
        if(debug)
            cout<<"tree erase: found target"<<endl;
        if(!root->_left){
            tree_node<T>* temp = root;
            root=root->_right;
            delete temp;

        }else{
            T it;
            tree_remove_max(root->_left,it);
            root->_item=it;
        }
        return true;
    }
}


template <typename T>       //erase rightmost node from the tree
                            // store the item in max_value
void tree_remove_max(tree_node<T>* &root, T& max_value){
    const bool debug = false;
    if(!root){
        if(debug)
            cout<<"max value doesnt exist, root is null"<<endl;
    }
    if(root->_right){
        tree_remove_max(root->_right, max_value);
    }
    else{
        max_value=root->_item;
        tree_erase(root,max_value);
    }
}


template<typename T>       //prints detailes info about each node
void tree_print_debug(tree_node<T>* root, int level, ostream& outs){
    if(root){
        tree_print_debug(root->_right, level+1);
        outs<<setw(4*level)<<"["<<root->_item<<"]"<< "h: "<<root->_height<<endl;
        tree_print_debug(root->_left,level+1);
    }
}

template <typename T>       //Add tree src to dest
void tree_add(tree_node<T>* & dest, const tree_node<T>* src){

}

template <typename T>       // sorted array -> tree
tree_node<T>* tree_from_sorted_list( T* a, int size,tree_node<T>* &root){
//    T* walker1 = a;
//    walker1+=size/2-1;
//    cout<<*walker1<<endl;
//    tree_insert(root,*walker1);
//    tree_print(root);
//    walker1--; T* walker2=a; walker2+=size/2;
//    while (walker1!=a) {
//        tree_insert(root,*walker1);
//        tree_print(root);
//        walker1--;
//    }

//    while (walker2!=NULL) {
//        tree_insert(root,*walker2);
//        walker2--;
//    }
//    return root;
}

#endif // BST_FUNCTIONS_H
