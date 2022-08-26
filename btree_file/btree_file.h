#ifndef BTREE_FILE_H
#define BTREE_FILE_H
#include <iostream>
#include <iomanip>
#include <fstream>
#include "../../../Cs8/!includes/BTREE_array_functions/btree_array_funcions.h"
#include "../../../Cs8/!includes/btree_file/record.h"

using namespace std;


template <class T>
class BTree
{
public:
    BTree(){}
    BTree(string file, bool dups = false){
        dups_ok=dups;
        filename=file;
        Record<T> me;
        write(me);
    }

    //big three:
    BTree(const BTree<T>& other){
        copy_tree(other);
    }

    ~BTree(){
//       if(!is_leaf())
//           clear_tree();
//       else{
//           data_count=0;
//           child_count=0;
//       }

    }

    BTree<T>& operator =(const BTree<T>& RHS){
        copy_tree(RHS);
        return *this;
    }

    void insert(const T& entry);                //insert entry into the tree
    void remove(const T& entry);                //remove entry from the tree

    void clear_tree(long pos=0);                          //clear this object (delete all nodes etc.)
    void copy_tree(const BTree<T>& other);      //copy other into this object

    bool contains(const T& entry, long pos=0);              //true if entry can be found in the array
    T& get(const T& entry);                     //return a reference to entry in the tree
    T* find(const T& entry,long pos=0);                    //return a pointer to this key. NULL if not there.

    int size(long pos=0);                           //count the number of elements in the tree
    bool empty() const;                         //true if the tree is empty
    bool is_valid();

    void print_tree(int level = 0, long pos=0, ostream &outs=cout); //print a readable version of the tree

    friend ostream& operator<<(ostream& outs,  BTree<T>& print_me){
        print_me.print_tree(0, 0,outs);
        return outs;
    }

private:
    void open_fileRW(fstream& f, string filename) throw(char*); //for file
    void open_fileW(fstream& f, string filename);
    bool file_exists(string filename);

    string filename;

    long write(Record<T> r);
    void write_at(Record<T> r, long pos);
    Record<T> read(long recno);

    bool dups_ok;                                   //true if duplicate keys may be inserted

    bool is_leaf(Record<T> r) const {return r.child_count==0;}   //true if this is a leaf node

    //insert element functions
    void loose_insert(const T& entry,long pos);              //allows MAXIMUM+1 data elements in the root
    void fix_excess(Record<T> r, int i, long my_pos);                         //fix excess of data elements in child i

    //remove element functions:
    void loose_remove(const T& entry,long pos);              //allows MINIMUM-1 data elements in the root
    void fix_shortage(int i,Record<T> r, long pos);                       //fix shortage of data elements in child i

    void remove_biggest(T& entry,Record<T> r, long pos);                  //remove the biggest child of this tree->entry
    void rotate_left(int i, Record<T> r, long pos);                        //transfer one element LEFT from child i
    void rotate_right(int i, Record<T> r, long pos);                       //transfer one element RIGHT from child i
    void merge_with_next_subset(int i, Record<T> r, long pos);             //merge subset i with subset i+1

    //for get
    T& get_existing(const T& entry);
};

template <class T>
void BTree<T>::print_tree(int level, long pos,  ostream &outs) {

    Record<T> me=read(pos);
    if(me.data_count!=0){
        for(int i=me.child_count-1; i>(me.child_count/2)-1; i--){
            print_tree(level+1,me.children[i]);
        }
        outs<<setw(4*level);
        print_array(me.data,me.data_count);
        outs<<endl;
        for(int i=me.child_count/2-1; i>=0; i--){
            print_tree(level+1,me.children[i]);
        }

    }
}

template <class T>
T* BTree<T>::find(const T& entry,long pos){
//    Record<T> r=read(pos);
//    int index=first_ge(r.data,r.data_count,entry);
//    if(r.data[index]==entry){
//        T* it=&data[index];
//        return it;
//    }else if(is_leaf()){
//        return NULL;
//    }else
//        return subset[index]->find(entry);
}

template <class T>
void BTree<T>:: insert(const T& entry){
    //cout<<"in insert"<<endl;
    loose_insert(entry,0);
    Record<T> root=read(0);

    if(root.data_count>MAXIMUM){
        Record<T> shadow;
        long shadow_pos=write(shadow);

        copy_array(shadow.data,root.data,shadow.data_count,root.data_count);
        copy_array(shadow.children,root.children,shadow.child_count,root.child_count);
        cout<<"shadow data: "; print_array(shadow.data,shadow.data_count);
        root.data_count=0;
        root.children[0]=shadow_pos;
        root.child_count=1;
        write_at(shadow,shadow_pos);
        write_at(root,0);

        fix_excess(root,0,0);
    }
}

template <typename T>
long BTree<T>::write(Record<T> r){
    fstream f;
    open_fileRW(f, filename);

    f.seekg(0,f.end);
    long pos = f.tellp();

    f.write((char*)&r, sizeof(r));
    //cout<<"in btree write, pos="<<pos/sizeof(r)<<endl;
    pos= pos/sizeof(r);
    f.close();
    return pos;
}

template <typename T>
void BTree<T>::write_at(Record<T> r,long pos){
    //cout<<"pass position to write at: "<<pos<<endl;
    fstream f;
    open_fileRW(f, filename);

    f.seekg(pos * sizeof(r));
    long p = f.tellp();

    f.write((char*)&r, sizeof(r));
    //cout<<"in btree write_at, pos="<<p/sizeof(r)<<endl;
    f.close();
}

template <typename T>
Record<T> BTree<T>::read(long recno){
    fstream f;
    open_fileRW(f, filename);
    Record<T> r;
    long pos= recno * sizeof(r);
    f.seekg(pos, ios_base::beg);

    f.read((char*)&r, sizeof(r));
    //return f.gcount();
    return r;
}


template <typename T>
void BTree<T>::loose_insert(const T& entry, long pos){
    //cout<<"BTree loose insert"<<endl;
    Record<T> r=read(pos); //reads from file

    int i = first_ge(r.data, r.data_count, entry);
    bool found = (i<r.data_count && r.data[i] == entry);

    if(found && dups_ok){
        //if dups are ok data[i]=data[i]+entry for vectors later on
        r.data[i]=r.data[i]+entry;
    }else{
        if(is_leaf(r)){
            ordered_insert(r.data,r.data_count,entry);
            write_at(r,pos);
        }
        else{
            loose_insert(entry,r.children[i]);
            fix_excess(r,i,pos);
//            cout<<"after fix excess tree looks like this"<<endl;
//            print_tree();
        }

    }
}

template <typename T>
void BTree<T>::fix_excess(Record<T> r, int i, long my_pos){  //i is index of subset
    Record<T> child_i=read(r.children[i]);

    if(child_i.data_count>MAXIMUM){
        //cout<<"in fix excess"<<endl;
        //1. add new subset at loc i+1
        Record<T> new_child;
        long new_child_pos=write(new_child);
        insert_item(r.children,i+1,r.child_count,new_child_pos);
        cout<<"root has: "<<r.child_count<<" children"<<endl;

        //2. split subset[i] (both the subset array and the data array) and move half into
         // subset[i+1] (this is the subset we created in step 1.)
        split(child_i.data,child_i.data_count,new_child.data,new_child.data_count);
        split(child_i.children,child_i.child_count,new_child.children,new_child.child_count);

        //3. detach the last data item of subset[i] and bring it and insert it into this node's data[]
        T entry;
        detach_item(child_i.data,child_i.data_count,entry);

        cout<<"r sub: "; print_array(r.children,r.child_count);

        ordered_insert(r.data,r.data_count,entry);
        cout<<"r data: "; print_array(r.data,r.data_count);
        cout<<"r sub: "; print_array(r.children,r.child_count);

//        Record<T> temp2=read(r.children[0]);
//        cout<<"first kid: "; print_array(temp2.data,temp2.data_count);

        //update the affected records in file
        write_at(r,my_pos);
        cout<<"r sub: "; print_array(r.children,r.child_count);
        write_at(child_i,r.children[i]);
        cout<<"r sub: "; print_array(r.children,r.child_count);
        write_at(new_child,new_child_pos);
        cout<<"r sub: "; print_array(r.children,r.child_count);
//        write_at(r,my_pos);
//        cout<<"r sub: "; print_array(r.children,r.child_count);
    }
    cout<<"after fix excess tree looks like this"<<endl;
    print_tree();
    cout<<"root has: "<<r.child_count<<" children"<<endl;
    print_array(r.children,r.child_count);
}

template <typename T>
void BTree<T>::clear_tree(long pos){
    Record<T> r=read(pos);
    for(int i=0; i<r.child_count; i++){
        clear_tree(r.children[i]);
    }
    r.child_count=0; r.data_count=0;
    write_at(r,pos);
}

template <typename T>
void BTree<T>:: copy_tree(const BTree<T>& other){

//    copy_array(data,other.data,data_count,other.data_count);
//    for(int i=0; i<other.child_count; i++){
//        subset[i]= new BTree;
//        subset[i]->copy_tree(*other.subset[i]);
//        child_count++;
//    }
}

template <typename T>
bool BTree<T>::empty() const{
    Record<T> r=read(0);
    return (r.data_count==0 && r.child_count==0);
}

template <typename T>
int BTree<T>::size(long pos) {
    Record<T> r=read(pos);
    int temp= r.data_count;
    for(int i=0; i<r.child_count; i++){
        temp+=size(r.children[i]);
    }
    return temp;
}

template <typename T>
bool BTree<T>::contains(const T& entry, long pos){
    Record<T> r=read(pos);
    int index=first_ge(r.data,r.data_count,entry);
    if(r.data[index]==entry){
        return true;
    }else if(is_leaf(r)){
        return false;
    }else
        return contains(entry, r.children[index]);
}

//template <typename T>
//T& BTree<T>::get(const T& entry){
//    if (!contains(entry))
//             insert(entry);
//    return get_existing(entry);
//}

//template <typename T>
//T& BTree<T>::get_existing(const T& entry){
//    T* it=find(entry);
//    assert(it!=NULL);
//    return *it;
//}

template <typename T>
void BTree<T>::remove(const T& entry){
    loose_remove(entry,0);
    Record<T> root=read(0);

    if(root.data_count<MINIMUM && !is_leaf(root)){
        Record<T> shrink_ptr=read(root.children[0]);
        copy_array(root.data,shrink_ptr.data,root.data_count,shrink_ptr.data_count);
        copy_array(root.children,shrink_ptr.children,root.child_count,shrink_ptr.child_count);
        shrink_ptr.data_count=0; shrink_ptr.child_count=0;

        write_at(shrink_ptr,root.children[0]);
        write_at(root,0);
    }

}

template <typename T>
void BTree<T>::loose_remove(const T& entry, long pos){
     /* four cases:
          a. leaf && not found target: there is nothing to do
          b. leaf && found target: just remove the target
          c. not leaf and not found target: recursive call to loose_remove
          d. not leaf and found: replace target with largest child of subset[i]
     */
    //cout<<"BTree loose remove"<<endl;

    Record<T> r=read(pos); //reads from file
    int i = first_ge(r.data, r.data_count, entry);
    bool found = (i<r.data_count && r.data[i] == entry);

    if(is_leaf(r)&&!found){
        //nothing
    }
    if(is_leaf(r)&&found){
        T temp;
        delete_item(r.data,i,r.data_count,temp);
        write_at(r,pos);
        //cout<<"after delete"<<endl;
    }else if(!is_leaf(r) && !found){
        loose_remove(entry,r.children[i]);
        fix_shortage(i,r,pos);
    }else if(!is_leaf(r) && found){
        Record<T> child_i=read(r.children[i]);
        remove_biggest(r.data[i],child_i,r.children[i]);
        fix_shortage(i,r,pos);
    }
}

template <typename T>
void BTree<T>::fix_shortage(int i,Record<T> r, long pos){   //i is index of subset
        /* fix shortage in subtree i:
         * if child i+1 has more than MINIMUM, rotate left
         * elif child i-1 has more than MINIMUM, rotate right
         * elif there is a right child, merge child i with next child
         * else merge child i with left child
         */
    Record<T> child_i=read(r.children[i]);
    if(child_i.data_count<MINIMUM){
        cout<<"in fix shortage"<<endl;
        if (i+1<r.child_count && read(r.children[i+1]).data_count>MINIMUM)
            rotate_left(i+1,r,pos);
        else if (i-1>-1 && read(r.children[i-1]).data_count>MINIMUM)
            rotate_right(i-1,r,pos);
        else if(i+1<r.child_count)
            merge_with_next_subset(i,r,pos);
        else
            merge_with_next_subset(i-1,r,pos);
    }
}

template <typename T>
void BTree<T>::remove_biggest(T& entry, Record<T> r, long pos){
    if(is_leaf(r)){
        detach_item(r.data,r.data_count,entry);
        write_at(r,pos);
    }
    else{
        Record<T> child=read(r.children[r.child_count-1]);
        remove_biggest(entry,child,r.children[r.child_count-1]);
        fix_shortage(r.child_count-1,r,pos);
    }
}

template <typename T>
void BTree<T>::rotate_left(int i, Record<T> r, long pos){  //transfer one element LEFT from child i
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

    T temp=r.data[i-1];
    Record<T> child_i_1=read(r.children[i-1]);
    ordered_insert(child_i_1.data,child_i_1.data_count,temp);

    Record<T> child_i=read(r.children[i]);
    delete_item(child_i.data,0,child_i.data_count,child_i.data[i-1]);

    if(child_i.child_count>0){
        insert_item(child_i_1.children,child_i_1.child_count,child_i_1.child_count,child_i.children[0]);
        delete_item(child_i.children,0,child_i.child_count,child_i.children[0]);
    }
    write_at(child_i_1,r.children[i-1]);
    write_at(child_i,r.children[i]);
    write_at(r,pos);
}

template <typename T>
void BTree<T>::rotate_right(int i, Record<T> r, long pos){ //transfer one element RIGHT from child i
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
    T temp=r.data[i];
    Record<T> child_i_p1=read(r.children[i+1]);
    ordered_insert(child_i_p1.data,child_i_p1.data_count,temp);

    Record<T> child_i=read(r.children[i]);
    detach_item(child_i.data,child_i.data_count,r.data[i]);

    if(child_i.child_count>0){
        long b=child_i.children[child_i.child_count-1];
        insert_item(child_i_p1.children,0,child_i_p1.child_count,b);
        delete_item(child_i.children,child_i.child_count-1,child_i.child_count,b);
    }
    write_at(child_i_p1,r.children[i+1]);
    write_at(child_i,r.children[i]);
    write_at(r,pos);
}

template <typename T>
void BTree<T>::merge_with_next_subset(int i, Record<T> r, long pos){  //merge subset i with subset i+1
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
    delete_item(r.data,i,r.data_count,temp);

    Record<T> child_i=read(r.children[i]);
    ordered_insert(child_i.data,child_i.data_count,temp);

    Record<T> child_i_p1=read(r.children[i+1]);
    for(int j=0; j<child_i_p1.data_count; j++){
        ordered_insert(child_i.data,child_i.data_count,child_i_p1.data[j]);
    }
    for(int j=0; j<child_i_p1.child_count; j++){
        insert_item(child_i.children,child_i.child_count,
                    child_i.child_count,child_i_p1.children[j]);
    }
    long t;
    delete_item(r.children,i+1,r.child_count,t);

    write_at(child_i_p1,r.children[i+1]);
    write_at(child_i,r.children[i]);
    write_at(r,pos);
}

//template <typename T>
//bool BTree<T>::is_valid(){
//    if(empty())
//        return true;
//    if(data_count>=MINIMUM && data_count<=MAXIMUM && is_leaf())
//        return true;
//    else if (data_count>=MINIMUM && data_count<=MAXIMUM && !is_leaf() && child_count==data_count+1) {
//        for(int i=0; i<child_count; i++)
//            return subset[i]->is_valid();
//    }
//    else
//        return false;
//}





//-------------------file opening-----------------------------------------------



template <typename T>
void BTree<T>::open_fileW(fstream& f, string filename){
    f.open (filename,
            std::fstream::out| std::fstream::binary | std::fstream::app);
    if (f.fail()){
        cout<<"file open failed: "<<filename<<endl;
        throw("file failed to open.");
    }
}

template <typename T>
void BTree<T>::open_fileRW(fstream& f, string filename) throw(char*){
    const bool debug = false;
    //openning a nonexistent file for in|out|app causes a fail()
    //  so, if the file does not exist, create it by openning it for
    //  output:
    if (!file_exists(filename)){
        f.open(filename, std::fstream::out|std::fstream::binary);
        if (f.fail()){
            cout<<"file open (RW) failed: with out|"<<filename<<"]"<<endl;
            throw("file RW failed  ");
        }
        else{
            if (debug) cout<<"open_fileRW: file created successfully: "<<filename<<endl;
        }
    }
    else{
        f.open (filename,
            std::fstream::in | std::fstream::out| std::fstream::binary );
        if (f.fail()){
            cout<<"file open (RW) failed. ["<<filename<<"]"<<endl;
            throw("file failed to open.");
        }
    }

}

template <typename T>
bool BTree<T>::file_exists(string filename){
    const bool debug = false;
    fstream ff;
    ff.open (filename,
        std::fstream::in | std::fstream::binary );
    if (ff.fail()){
        if (debug) cout<<"file_exists(): File does NOT exist: "<<filename<<endl;
        return false;
    }
    if (debug) cout<<"file_exists(): File DOES exist: "<<filename<<endl;
    ff.close();
    return true;
}

#endif // BTREE_FILE_H
