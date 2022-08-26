#ifndef BPTREE_CLASS_H
#define BPTREE_CLASS_H
#include <iostream>
#include <iomanip>
#include "../../../Cs8/!includes/BTREE_array_functions/btree_array_funcions.h"
using namespace std;

template <class T>
class BPlusTree
{
public:
    class Iterator{
        public:
            friend class BPlusTree;
            Iterator(BPlusTree<T>* _it=NULL, int _key_ptr = 0):it(_it), key_ptr(_key_ptr){}

            T operator *(){
                assert(key_ptr<it->data_count);
                return it->data[key_ptr];
            }

            Iterator operator++(int un_used){
                Iterator temp(it,key_ptr);
                key_ptr++;
                if(key_ptr==it->data_count){
                    it=it->next;
                    key_ptr=0;
                }
                return temp;
            } //it++

            Iterator operator++(){
                key_ptr++;
                if(key_ptr==it->data_count){
                    it=it->next;
                    key_ptr=0;
                }
                return *this;
            }//++it

            friend bool operator ==(const Iterator& lhs, const Iterator& rhs){
                return (lhs.it==rhs.it && lhs.key_ptr==rhs.key_ptr);
            }

            friend bool operator !=(const Iterator& lhs, const Iterator& rhs){
                if(lhs.it!=rhs.it)
                    return true;
                else
                    return lhs.key_ptr!=rhs.key_ptr;
            }
            void print_Iterator(){
                cout<<it->data[key_ptr];
            }

            bool is_null(){return !it;}
        private:
            BPlusTree<T>* it;
            int key_ptr;
        };

    BPlusTree(bool dups=false){
        data_count=0;
        child_count=0;
        dups_ok=dups;
        //cout<<"in bpt const, dups: "<<dups_ok<<endl;
        next=NULL;
    }

    //big three:
    BPlusTree(const BPlusTree<T>& other){
        copy_tree(other);
    }

    ~BPlusTree(){
       if(!is_leaf())
           clear_tree();
       else{
           data_count=0;
           child_count=0;
       }
    }

    BPlusTree<T>& operator =(const BPlusTree<T>& RHS){
        copy_tree(RHS);
        dups_ok=RHS.dups_ok;
        return *this;
    }

    void insert(const T& entry);                //insert entry into the tree
    void remove(const T& entry);                //remove entry from the tree

    void clear_tree();                          //clear this object (delete all nodes etc.)
    void copy_tree(const BPlusTree<T>& other);      //copy other into this object

    bool contains(const T& entry);              //true if entry can be found in the array
    T& get(const T& entry);                     //return a reference to entry in the tree
    Iterator find(const T& entry);                    //return a pointer to this key. NULL if not there.
    T* find_ptr(const T& entry);

    int size() const;                           //count the number of elements in the tree
    bool empty() const;                         //true if the tree is empty
    bool is_valid();

    void print_tree(int level = 0, ostream &outs=cout) const; //print a readable version of the tree
    void print_leaves(ostream &outs=cout);
    friend ostream& operator<<(ostream& outs, const BPlusTree<T>& print_me){
        print_me.print_tree(0, outs);
        return outs;
    }

    Iterator begin();
    Iterator end();
    Iterator lower_bound(const T& key);  //return first that goes NOT BEFORE
                                            // key entry or next if does not
                                            // exist >= entry
    Iterator upper_bound(const T& key);  //return first that goes AFTER key
                                         //exist or not, the next entry  >entry

private:
    static const int MINIMUM = 1;
    static const int MAXIMUM = 2 * MINIMUM;

    bool dups_ok;                                   //true if duplicate keys may be inserted
    int data_count;                                 //number of data elements
    T data[MAXIMUM + 1];                            //holds the keys
    int child_count=0;                                //number of children
    BPlusTree* subset[MAXIMUM + 2];                     //suBPlusTrees
    BPlusTree* next;

    bool is_leaf() const {return child_count==0;}   //true if this is a leaf node

    //T* find_ptr(const T& entry);

    //insert element functions
    void loose_insert(const T& entry);              //allows MAXIMUM+1 data elements in the root
    void fix_excess(int i);                         //fix excess of data elements in child i

    //remove element functions:
    void loose_remove(const T& entry);              //allows MINIMUM-1 data elements in the root
    void fix_shortage(int i);                       //fix shortage of data elements in child i

    void get_smallest(T& entry);
    void rotate_left(int i);                        //transfer one element LEFT from child i
    void rotate_right(int i);                       //transfer one element RIGHT from child i
    void merge_with_next_subset(int i);             //merge subset i with subset i+1

    //for get
    T& get_existing(const T& entry);

    //for copy
    void copy_tree(const BPlusTree<T>& other, BPlusTree<T>*& s);
};

template <class T>
void BPlusTree<T>::print_tree(int level, ostream &outs) const{
    if(data_count!=0){
        for(int i=child_count-1; i>(child_count/2)-1; i--){
            subset[i]->print_tree(level+1);
        }
        outs<<setw(4*level); print_array(data,data_count); outs<<endl;
        for(int i=child_count/2-1; i>=0; i--){
            subset[i]->print_tree(level+1);
        }
    }
}

template <class T>
void BPlusTree<T>::print_leaves(ostream &outs){
    if(is_leaf()){
        print_array(data,data_count);
        BPlusTree* walker=next;
        while(walker!=NULL){
            cout<<"->";
            print_array(walker->data,walker->data_count);
            walker=walker->next;
        }
    }else
        subset[0]->print_leaves(outs);
}

template <class T>
typename BPlusTree<T>::Iterator BPlusTree<T>::find(const T& entry){
    int i=first_ge(data,data_count,entry);
    bool found = (i<data_count && data[i] == entry);

    if(found){
        if(is_leaf()){
            return BPlusTree<T>::Iterator(this,i);
        }else
            return subset[i+1]->find(entry);
    }else{
        if(is_leaf())
            return NULL;
        else
            return subset[i]->find(entry);
    }
}
template <class T>
T* BPlusTree<T>::find_ptr(const T& entry){
    int i=first_ge(data,data_count,entry);
    bool found = (i<data_count && data[i] == entry);

    if(found){
        if(is_leaf()){
            T* it=&data[i];
            return it;
        }else
            return subset[i+1]->find_ptr(entry);
    }else{
        if(is_leaf())
            return NULL;
        else
            return subset[i]->find_ptr(entry);
    }

}

template <class T>
void BPlusTree<T>:: insert(const T& entry){
    loose_insert(entry);
    if(data_count>MAXIMUM){
        BPlusTree* shadow=new BPlusTree(dups_ok);
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
void BPlusTree<T>::loose_insert(const T& entry){
    //cout<<"BPlusTree loose insert"<<endl;
    /*
           int i = first_ge(data, data_count, entry);
           bool found = (i<data_count && data[i] == entry);

           three cases:
             found
             a. found/leaf: deal with duplicates: call +
             b. found/no leaf: subset[i+1]->loose_insert(entry)
                               fix_excess(i+1) if there is a need

             ! found:
             c. !found / leaf : insert entry in data at position i
             d. !found / !leaf: subset[i]->loose_insert(entry)
                                fix_excess(i) if there is a need

                |   found          |   !found         |
          ------|------------------|------------------|-------
          leaf  |a. dups? +        | c: insert entry  |
                |                  |    at data[i]    |
          ------|------------------|------------------|-------
                | b.               | d.               |
                |subset[i+1]->     | subset[i]->      |
          !leaf | loose_insert(i+1)|  loose_insert(i) |
                |fix_excess(i+1)   | fix_excess(i)    |
                |                  |                  |
          ------|------------------|------------------|-------
        */

    int i = first_ge(data, data_count, entry);
    bool found = (i<data_count && data[i] == entry);

    //cout<<"dups_ok: "<<dups_ok<<endl;
    //if(found && is_leaf() && dups_ok){ //fixxx
    if(found && is_leaf()){
            //cout<<"duplicate"<<endl;
            //cout<<"data[i]"<<data[i]<<endl;
            //cout<<"entry: "<<entry<<endl;
            data[i]=data[i]+entry;
            //cout<<"data[i] after add"<<data[i]<<endl;
    }else if(found && !is_leaf()){
        subset[i+1]->loose_insert(entry);
        fix_excess(i+1);
    }else if(!found && is_leaf()){
            ordered_insert(data,data_count,entry);
    }else if(!found && !is_leaf()){
            subset[i]->loose_insert(entry);
            fix_excess(i);
    }
}

template <typename T>
void BPlusTree<T>::fix_excess(int i){  //i is index of subset

    if(subset[i]->data_count>MAXIMUM){
        //1. add new subset at loc i+1
        BPlusTree* temp= new BPlusTree(dups_ok);
        insert_item(subset,i+1,child_count,temp);

        //2. split subset[i] (both the subset array and the data array) and move half into
        // subset[i+1] (this is the subset we created in step 1.)
        split(subset[i]->data,subset[i]->data_count,subset[i+1]->data,subset[i+1]->data_count);
        split(subset[i]->subset,subset[i]->child_count,subset[i+1]->subset,subset[i+1]->child_count);

        //3. detach the last data item of subset[i] and bring it and insert it into this node's data[]
        T entry;
        detach_item(subset[i]->data,subset[i]->data_count,entry);
        ordered_insert(data,data_count,entry);

        if(subset[i+1]->is_leaf()){
            insert_item(subset[i+1]->data,0,subset[i+1]->data_count,entry);
            subset[i+1]->next=subset[i]->next;
            subset[i]->next=subset[i+1];
        }
    }
}

template <typename T>
void BPlusTree<T>::clear_tree(){
    for(int i=0; i<child_count; i++){
        subset[i]->clear_tree();
        delete subset[i];
    }
    child_count=0; data_count=0; next=NULL;
}


template <typename T>
void BPlusTree<T>:: copy_tree(const BPlusTree<T>& other){
    //assert(empty());
    BPlusTree<T>* pointer=NULL;
    copy_tree(other,pointer);
}

template <typename T>
void BPlusTree<T>::copy_tree(const BPlusTree<T>& other, BPlusTree<T>*& s){
    copy_array(data,other.data,data_count,other.data_count);

    for(int i=0; i<other.child_count; i++){
        subset[i]= new BPlusTree(dups_ok);
        subset[i]->copy_tree(*other.subset[i],s);
        child_count++;
    }
    if(other.is_leaf()){
        if(s){
            s->next=this;
        }
        s=this;
    }
}

template <typename T>
bool BPlusTree<T>::empty() const{
    return (data_count==0);
}

template <typename T>
int BPlusTree<T>::size() const{
    if(is_leaf()){
        int size=data_count;
        BPlusTree* walker=next;
        while(walker!=NULL){
            size+=walker->data_count;
            walker=walker->next;
        }
        return size;
    }else
        return subset[0]->size();
}

template <typename T>
bool BPlusTree<T>::contains(const T& entry){
    int i=first_ge(data,data_count,entry);
    bool found = (i<data_count && data[i] == entry);

    if(found){
        if(is_leaf())
            return true;
        else
            return subset[i+1]->find_ptr(entry);
    }else{
        if(is_leaf())
            return false;
        else
            return subset[i]->find_ptr(entry);
    }
}

template <typename T>
T& BPlusTree<T>::get(const T& entry){
    if (!contains(entry))
             insert(entry);
    return get_existing(entry);
}

template <typename T>
T& BPlusTree<T>::get_existing(const T& entry){
    assert(contains(entry));
    T* it=find_ptr(entry);
    assert(it!=NULL);
    return *it;
}

template <typename T>
void BPlusTree<T>::remove(const T& entry){

    loose_remove(entry);

    //cout<<"before if"<<endl;
    if(data_count<MINIMUM && !is_leaf()){
        //cout<<"in if"<<endl;
        BPlusTree* shrink_ptr=subset[0];
        copy_array(data,shrink_ptr->data,data_count,shrink_ptr->data_count);
        copy_array(subset,shrink_ptr->subset,child_count,shrink_ptr->child_count);
        shrink_ptr->data_count=0; shrink_ptr->child_count=0;
        delete shrink_ptr;
    }

}
template <typename T>
void BPlusTree<T>::loose_remove(const T& entry){

    //cout<<"BPlusTree loose remove"<<endl;

    /* four cases:
               leaves:
                    a. not found: there is nothing to do
                    b. found    : just remove the target
               non leaf:
                    c. not found: subset[i]->loose_remove, fix_shortage(i)
                    d. found    : subset[i+1]->loose_remove, fix_shortage(i+1) [...]
                        (No More remove_biggest)

                 |   !found               |   found                 |
           ------|------------------------|-------------------------|-------
           leaf  |  a: nothing            | b: delete               |
                 |     to do              |    target               |
           ------|------------------------|-------------------------|-------
           !leaf | c:                     | d: B_PLUS_TREE          |
                 |  [i]->  loose_remove   |   [i+1]-> loose_remove  |
                 |  fix_shortage(i)       | fix_shortage(i+1) [...] |
           ------|------------------------|-------------------------|-------


     */
    int i = first_ge(data, data_count, entry);
    bool found = (i<data_count && data[i] == entry);


    if (is_leaf()){
        if(!found){
            //[a.] nothing to do //entry not in this tree
        }
        else{
                //[b.] just delete the item //   my parent will take care of my shortage
            T temp;
            delete_item(data,i,data_count,temp);
        }
    }
    else{ //not leaf:
        if (!found){
                //[c.] Not here: subset[i]->loose_remove
                //not found yet. search in the next level:
            subset[i]->loose_remove(entry);
            fix_shortage(i);
        }
        else{
                //[d.] found key in an inner node:subset[i+1]->loose_remove
                //assert(i < child_count-1);
            subset[i+1]->loose_remove(entry);
            fix_shortage(i+1);


            int ind = first_ge(data, data_count, entry);
            bool found = (ind<data_count && data[ind] == entry);
            if(found){
                //cout<<"found"<<endl;
                subset[i+1]->get_smallest(data[ind]);
            }
            else{
                int n = first_ge(subset[ind]->data, subset[ind]->data_count, entry);
                found = (n<subset[ind]->data_count && subset[ind]->data[n] == entry);
                if(found)
                    subset[ind]->subset[n+1]->get_smallest(subset[ind]->data[n]);
            }
        }
    }
}

template <typename T>
void BPlusTree<T>::get_smallest(T& entry){
    if(is_leaf())
        entry=data[0];
    else
        subset[0]->get_smallest(entry);
}

template <typename T>
void BPlusTree<T>::fix_shortage(int i){   //i is index of subset
    if(subset[i]->data_count<MINIMUM){
        //cout<<"in fix shortage"<<endl;

        if (i-1>-1 && subset[i-1]->data_count>MINIMUM)
            rotate_right(i-1);
        else if (i+1<child_count && subset[i+1]->data_count>MINIMUM)
            rotate_left(i+1);
        else if(i-1>=0)
            merge_with_next_subset(i-1);
        else
            merge_with_next_subset(i);
    }
}

template <typename T>
void BPlusTree<T>::rotate_left(int i){  //transfer one element LEFT from child i

    if(!subset[i]->is_leaf()){  // if not a leaf, same as BTree
        T temp=data[i-1];
        ordered_insert(subset[i-1]->data,subset[i-1]->data_count,temp);
        delete_item(subset[i]->data,0,subset[i]->data_count,data[i-1]);
        if(subset[i]->child_count>0){
            BPlusTree<T>* b=subset[i]->subset[0];
            insert_item(subset[i-1]->subset,subset[i-1]->child_count,subset[i-1]->child_count,b);
            delete_item(subset[i]->subset,0,subset[i]->child_count,b);
        }
    }else{
        T temp;
        delete_item(subset[i]->data,0,subset[i]->data_count,temp);
        ordered_insert(subset[i-1]->data,subset[i-1]->data_count,temp);
        delete_item(subset[i]->data,0,subset[i]->data_count,data[i-1]);
    }

}

template <typename T>
void BPlusTree<T>::rotate_right(int i){ //transfer one element RIGHT from child i
    if(!subset[i]->is_leaf()){  // if not a leaf, same as BTree
        T temp=data[i];
        ordered_insert(subset[i+1]->data,subset[i+1]->data_count,temp);
        detach_item(subset[i]->data,subset[i]->data_count,data[i]);
        if(subset[i]->child_count>0){
            BPlusTree* b=subset[i]->subset[subset[i]->child_count-1];
            insert_item(subset[i+1]->subset,0,subset[i+1]->child_count,b);
            delete_item(subset[i]->subset,subset[i]->child_count-1,subset[i]->child_count,b);
        }
    }else{
        detach_item(subset[i]->data,subset[i]->data_count,data[i]);
        ordered_insert(subset[i+1]->data,subset[i+1]->data_count,data[i]);
    }
}

template <typename T>
void BPlusTree<T>::merge_with_next_subset(int i){  //merge subset i with subset i+1

    //cout<<"merge with next subset"<<endl;
    if(!subset[i]->is_leaf()){  // if not a leaf, same as BTree
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
        BPlusTree* t;
        delete_item(subset,i+1,child_count,t);
    }else{
        for(int j=0; j<subset[i+1]->data_count; j++)
            ordered_insert(subset[i]->data,subset[i]->data_count,subset[i+1]->data[j]);
        subset[i]->next=subset[i+1]->next;
        T temp;
        delete_item(data,i,data_count,temp);
        BPlusTree* t;
        delete_item(subset,i+1,child_count,t);
    }

}

template <typename T>
typename BPlusTree<T>::Iterator BPlusTree<T>::begin(){
    if(is_leaf()){
        return BPlusTree<T>::Iterator(this);
    }else
        return subset[0]->begin();

}

template <typename T>
typename BPlusTree<T>::Iterator BPlusTree<T>::end(){
        //return BPlusTree<T>::Iterator();
    return NULL;
}
template <typename T>
typename BPlusTree<T>::Iterator BPlusTree<T>::lower_bound(const T& key){
    int i=first_ge(data,data_count,key);
    bool found = (i<data_count && data[i] == key);

    if(found){
        if(is_leaf()){
            return BPlusTree<T>::Iterator(this,i);
        }else
            return subset[i+1]->lower_bound(key);
    }else{
        if(is_leaf()){
            if(i<data_count)
                return BPlusTree<T>::Iterator(this,i);
            else
                return BPlusTree<T>::Iterator(next);
        }else
            return subset[i]->lower_bound(key);
    }
}


template <typename T>
typename BPlusTree<T>::Iterator BPlusTree<T>::upper_bound(const T& key){
    int i=first_ge(data,data_count,key);
    bool found = (i<data_count && data[i] == key);

    if(found){
        if(is_leaf()){
            if(i+1<data_count)
                return BPlusTree<T>::Iterator(this,i+1);
            else
                return BPlusTree<T>::Iterator(next);
        }else
            return subset[i+1]->upper_bound(key);
    }else{
        if(is_leaf()){

            if(i<data_count)
                return BPlusTree<T>::Iterator(this,i);
            else{
                //cout<<"returning next!"<<endl;
                if(next==NULL)
                    return end();
                else
                    return BPlusTree<T>::Iterator(next);
            }
        }else
            return subset[i]->upper_bound(key);
    }
}


template <typename T>
bool BPlusTree<T>::is_valid(){
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

#endif // BPTREE_CLASS_H
