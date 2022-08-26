#ifndef BTREE_CLASS_H
#define BTREE_CLASS_H
#include <iostream>
#include <iomanip>
#include "../../../Cs8/!includes/BTREE_array_functions/btree_array_funcions.h"
using namespace std;

template <class T>
class BTree
{
public:
    BTree(bool dups = false){
        data_count=0;
        child_count=0;
        dups_ok=dups;
    }

    //big three:
    BTree(const BTree<T>& other){
        copy_tree(other);
    }

    ~BTree(){
       if(!is_leaf())
           clear_tree();
       else{
           data_count=0;
           child_count=0;
       }

    }

    BTree<T>& operator =(const BTree<T>& RHS){
        copy_tree(RHS);
        dups_ok=RHS.dups_ok;
        return *this;
    }

    void insert(const T& entry);                //insert entry into the tree
    void remove(const T& entry);                //remove entry from the tree

    void clear_tree();                          //clear this object (delete all nodes etc.)
    void copy_tree(const BTree<T>& other);      //copy other into this object

    bool contains(const T& entry);              //true if entry can be found in the array
    T& get(const T& entry);                     //return a reference to entry in the tree
    T* find(const T& entry);                    //return a pointer to this key. NULL if not there.

    int size() const;                           //count the number of elements in the tree
    bool empty() const;                         //true if the tree is empty
    bool is_valid();

    void print_tree(int level = 0, ostream &outs=cout) const; //print a readable version of the tree

    friend ostream& operator<<(ostream& outs, const BTree<T>& print_me){
        print_me.print_tree(0, outs);
        return outs;
    }
    void dups(bool b){
        dups_ok=b;
    }

private:
    static const int MINIMUM = 1;
    static const int MAXIMUM = 2 * MINIMUM;

    bool dups_ok;                                   //true if duplicate keys may be inserted
    int data_count;                                 //number of data elements
    T data[MAXIMUM + 1];                            //holds the keys
    int child_count=0;                                //number of children
    BTree* subset[MAXIMUM + 2];                     //subtrees

    bool is_leaf() const {return child_count==0;}   //true if this is a leaf node

    //insert element functions
    void loose_insert(const T& entry);              //allows MAXIMUM+1 data elements in the root
    void fix_excess(int i);                         //fix excess of data elements in child i

    //remove element functions:
    void loose_remove(const T& entry);              //allows MINIMUM-1 data elements in the root
    void fix_shortage(int i);                       //fix shortage of data elements in child i

    void remove_biggest(T& entry);                  //remove the biggest child of this tree->entry
    void rotate_left(int i);                        //transfer one element LEFT from child i
    void rotate_right(int i);                       //transfer one element RIGHT from child i
    void merge_with_next_subset(int i);             //merge subset i with subset i+1

    //for get
    T& get_existing(const T& entry);
};

template <class T>
void BTree<T>::print_tree(int level, ostream &outs) const{
    if(data){
        for(int i=child_count-1; i>(child_count/2)-1; i--){
            subset[i]->print_tree(level+1);
        }
        outs<<setw(4*level); print_array(data,data_count);
        for(int i=child_count/2-1; i>=0; i--){
            subset[i]->print_tree(level+1);
        }
    }
}

template <class T>
T* BTree<T>::find(const T& entry){
    int index=first_ge(data,data_count,entry);
    if(data[index]==entry){
        T* it=&data[index];
        return it;
    }else if(is_leaf()){
        return NULL;
    }else
        return subset[index]->find(entry);
}

template <class T>
void BTree<T>:: insert(const T& entry){
    loose_insert(entry);
    if(data_count>MAXIMUM){
        BTree* shadow=new BTree;
        copy_array(shadow->data,data,shadow->data_count,data_count);
        copy_array(shadow->subset,subset,shadow->child_count,child_count);
        for(int i=0; i<child_count; i++)
            subset[i]=NULL;

        child_count=1; data_count=0;
        subset[0]=shadow;
        fix_excess(0);
    }
}

template <typename T>
void BTree<T>::loose_insert(const T& entry){
    //cout<<"BTree loose insert"<<endl;
    int i = first_ge(data, data_count, entry);
    bool found = (i<data_count && data[i] == entry);

    //if(found && dups_ok){
    if(found){
        //if dups are ok data[i]=data[i]+entry for vectors later on
        data[i]=data[i]+entry;
    }else{
        if(is_leaf())
            ordered_insert(data,data_count,entry);
        else{
            subset[i]->loose_insert(entry);
            fix_excess(i);
        }
    }
}

template <typename T>
void BTree<T>::fix_excess(int i){  //i is index of subset
    if(subset[i]->data_count>subset[i]->MAXIMUM){
        //1. add new subset at loc i+1
//        BTree* temp=subset[i+1];
//        for(int j=i+2; j<child_count+1; j++){
//            BTree* t=subset[j];
//            subset[j]=temp;
//            temp=t;
//        }
//        subset[i+1]= new BTree;
//        child_count++;
        BTree* temp= new BTree;
        insert_item(subset,i+1,child_count,temp);

        //2. split subset[i] (both the subset array and the data array) and move half into
        // subset[i+1] (this is the subset we created in step 1.)
        split(subset[i]->data,subset[i]->data_count,subset[i+1]->data,subset[i+1]->data_count);
        split(subset[i]->subset,subset[i]->child_count,subset[i+1]->subset,subset[i+1]->child_count);

        //3. detach the last data item of subset[i] and bring it and insert it into this node's data[]
        T entry;
        detach_item(subset[i]->data,subset[i]->data_count,entry);
        ordered_insert(data,data_count,entry);
    }
}

template <typename T>
void BTree<T>::clear_tree(){
    for(int i=0; i<child_count; i++){
        subset[i]->clear_tree();
        delete subset[i];
    }
    child_count=0; data_count=0;
}

template <typename T>
void BTree<T>:: copy_tree(const BTree<T>& other){
    copy_array(data,other.data,data_count,other.data_count);
    for(int i=0; i<other.child_count; i++){
        subset[i]= new BTree;
        subset[i]->copy_tree(*other.subset[i]);
        child_count++;
    }
}

template <typename T>
bool BTree<T>::empty() const{
    return (data_count==0 && child_count==0);
}

template <typename T>
int BTree<T>::size() const{
    int temp= data_count;
    for(int i=0; i<child_count; i++){
        temp+=subset[i]->size();
    }
    return temp;
}


template <typename T>
bool BTree<T>::contains(const T& entry){
    int index=first_ge(data,data_count,entry);
    if(data[index]==entry){
        return true;
    }else if(is_leaf()){
        return false;
    }else
        return subset[index]->find(entry);
}

template <typename T>
T& BTree<T>::get(const T& entry){
    if (!contains(entry))
             insert(entry);
    return get_existing(entry);
}

template <typename T>
T& BTree<T>::get_existing(const T& entry){
    T* it=find(entry);
    assert(it!=NULL);
    return *it;
}

template <typename T>
void BTree<T>::remove(const T& entry){
    //Loose_remove the entry from this tree.
    //  point a temporary pointer (shrink_ptr) and point it to this root's only subset
    //  copy all the data and subsets of this subset into the root (through shrink_ptr)
    //  now, the root contains all the data and poiners of it's old child.
    //  now, simply delete shrink_ptr (blank out child), and the tree has shrunk by one level.
    //  Note, the root node of the tree will always be the same, it's the child node we delete

    loose_remove(entry);

    //cout<<"before if"<<endl;
    if(data_count<MINIMUM && !is_leaf()){
        //cout<<"in if"<<endl;
        BTree* shrink_ptr=subset[0];
        copy_array(data,shrink_ptr->data,data_count,shrink_ptr->data_count);
        copy_array(subset,shrink_ptr->subset,child_count,shrink_ptr->child_count);
        shrink_ptr->data_count=0; shrink_ptr->child_count=0;
        delete shrink_ptr;
    }

}
template <typename T>
void BTree<T>::loose_remove(const T& entry){
     /* four cases:
          a. leaf && not found target: there is nothing to do
          b. leaf && found target: just remove the target
          c. not leaf and not found target: recursive call to loose_remove
          d. not leaf and found: replace target with largest child of subset[i]
     */
    //cout<<"BTree loose remove"<<endl;

    int i = first_ge(data, data_count, entry);
    //cout<<"f_g: "<<i<<endl;
    bool found = (i<data_count && data[i] == entry);

    if(is_leaf()&&!found){
        //nothing
    }
    if(is_leaf()&&found){
        T temp;
        delete_item(data,i,data_count,temp);
        //cout<<"after delete"<<endl;
    }else if(!is_leaf() && !found){
        subset[i]->loose_remove(entry);
        fix_shortage(i);
    }else if(!is_leaf() && found){
        subset[i]->remove_biggest(data[i]);
        fix_shortage(i);
    }
}

template <typename T>
void BTree<T>::fix_shortage(int i){   //i is index of subset    
        /* fix shortage in subtree i:
         * if child i+1 has more than MINIMUM, rotate left
         * elif child i-1 has more than MINIMUM, rotate right
         * elif there is a right child, merge child i with next child
         * else merge child i with left child
         */

    if(subset[i]->data_count<MINIMUM){
        //cout<<"in fix shortage"<<endl;
        if (i+1<child_count && subset[i+1]->data_count>MINIMUM)
            rotate_left(i+1);
        else if (i-1>-1 && subset[i-1]->data_count>MINIMUM)
            rotate_right(i-1);        
        else if(i+1<child_count)
            merge_with_next_subset(i);       
        else
            merge_with_next_subset(i-1);       
    }
}

template <typename T>
void BTree<T>::remove_biggest(T& entry){
    if(is_leaf())
        detach_item(data,data_count,entry);   
    else{
        subset[child_count-1]->remove_biggest(entry);
        fix_shortage(child_count-1);
    }
}

template <typename T>
void BTree<T>::rotate_left(int i){  //transfer one element LEFT from child i
        /* (0 < i < child_count) and (subset[i]->data_count > MINIMUM)
         * subset[i-1] has only MINIMUM - 1 entries.
         *
         * item transfers from child[i] to child [i-1]
         *
         * FIRST item of subset[i]->data moves up to data to replace data[i-1],
         * data[i-1] moves down to the RIGHT of subset[i-1]->data
         *
         *  i = 1:
         *              [50 100]
         *  [  ]        [65 75]       ....
         *            [a]  [b]  [c]
         *
         *  65 move up to replace 50 (data[i])
         *  65's child (its child 0) moves over to be the child of 50
         *  50 moves down to the right of subset[i]->data
         *
         *              [65 100]
         *  [50]         [ 75 ]       ....
         *     [a]      [b]  [c]        
         */

    T temp=data[i-1];
    ordered_insert(subset[i-1]->data,subset[i-1]->data_count,temp);
    delete_item(subset[i]->data,0,subset[i]->data_count,data[i-1]);
    if(subset[i]->child_count>0){
        BTree<T>* b=subset[i]->subset[0];
        insert_item(subset[i-1]->subset,subset[i-1]->child_count,subset[i-1]->child_count,b);
        delete_item(subset[i]->subset,0,subset[i]->child_count,b);
    }
}

template <typename T>
void BTree<T>::rotate_right(int i){ //transfer one element RIGHT from child i
    /* (i < child_count - 1) and (subset[i]->data_count > MINIMUM)
         * subset[i+ 1] has only MINIMUM - 1 entries.
         *
         * item transfers from child[i] to child [i+1]
         *
         * LAST item of subset[i]->data moves up to data to replace data[i],
         * data[i] moves down to the LEFT of subset[i+1]->data
         *
         * i = 1
         *                     [50 100]
         *      [20 30]        [65 75]          [ ]
         *  [..] [..] [..]   [a] [b] [c]        [..]
         *
         *  75 moves up to replace 100 (data[i])
         *  75's child (its last child) moves over to be the (child 0) child of 100
         *  100 moves down to subset[i]->data
         *
         *                     [50 75]
         *      [20 30]          [65]          [100]
         *  [..] [..] [..]     [a] [b]        [c] [..]
         */
    T temp=data[i];
    ordered_insert(subset[i+1]->data,subset[i+1]->data_count,temp);
    detach_item(subset[i]->data,subset[i]->data_count,data[i]);
    if(subset[i]->child_count>0){
        BTree* b=subset[i]->subset[subset[i]->child_count-1];
        insert_item(subset[i+1]->subset,0,subset[i+1]->child_count,b);
        delete_item(subset[i]->subset,subset[i]->child_count-1,subset[i]->child_count,b);
    }
}

template <typename T>
void BTree<T>::merge_with_next_subset(int i){  //merge subset i with subset i+1
        /*  Merge subset[i] with subset [i+1] with data[i] in the middle
         *
         *   1. remove data[i] from this object
         *   2. append it to child[i]->data
         *   3. Move all data items from subset[i+1]->data to subset[i]->data
         *   4. Move all subset pointers from subset[i+1]->subset to subset[i]->subset
         *   5. delete subset[i+1] (store in a temp ptr)
         *   6. delete temp ptr
         */
    //cout<<"merge with next subset"<<endl;
    T temp;
    delete_item(data,i,data_count,temp);
    ordered_insert(subset[i]->data,subset[i]->data_count,temp);
    for(int j=0; j<subset[i+1]->data_count; j++){
        ordered_insert(subset[i]->data,subset[i]->data_count,subset[i+1]->data[j]);
    }
    for(int j=0; j<subset[i+1]->child_count; j++){
        insert_item(subset[i]->subset,subset[i]->child_count,
                    subset[i]->child_count,subset[i+1]->subset[j]);
    }
    BTree* t;
    delete_item(subset,i+1,child_count,t);
}

template <typename T>
bool BTree<T>::is_valid(){
    if(empty())
        return true;
    if(data_count>=MINIMUM && data_count<=MAXIMUM && is_leaf())
        return true;
    else if (data_count>=MINIMUM && data_count<=MAXIMUM && !is_leaf() && child_count==data_count+1) {
        for(int i=0; i<child_count; i++)
            return subset[i]->is_valid();
    }
    else
        return false;
}

#endif // BTREE_CLASS_H
