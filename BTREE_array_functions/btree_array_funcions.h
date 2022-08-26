#ifndef BTREE_ARRAY_FUNCIONS_H
#define BTREE_ARRAY_FUNCIONS_H
#include <iostream>
#include <vector>
using namespace std;

template <class T>
T maximal(const T& a, const T& b);
template <class T>
void swap(T& a, T& b);
template <class T>
int index_of_maximal(T data[ ], int n);
template <class T>
void ordered_insert(T data[ ], int& n, T entry);
template <class T>
int first_ge(const T data[ ], int n, const T& entry);
template <class T>
void attach_item(T data[ ], int& n, const T& entry);
template <class T>
void insert_item(T data[ ], int index, int& n, T entry);
template <class T>
void detach_item(T data[ ], int& n, T& entry);
template <class T>
void delete_item(T data[ ], int i, int& n, T& entry);
template <class T>
void merge(T data1[ ], int& n1, T data2[ ], int& n2);
template <class T>
void split(T data1[ ], int& n1, T data2[ ], int& n2);
template <class T>
void copy_array(T dest[],const T src[],int& dest_size, const int src_size);
template <class T>
void print_array(const T data[], int n, int pos = -1);
template <class T>
bool is_gt(const T data[], int n, const T& item);
template <class T>
bool is_le(const T data[], int n, const T& item);

//------------------------------------------------------------------------------

template <class T>
T maximal(const T& a, const T& b){ //return the larger of the two items
    if(a>b)
        return a;
    else
        return b;
}

template <class T>
void swap(T& a, T& b){  //swap the two items
    T temp=a;
    a=b;
    b=temp;
}

template <class T>
int index_of_maximal(T data[ ], int n){ //return index of the largest item in data
    int max_index=0;
    for(int i=0; i<n; i++){
        if(data[i]>data[max_index])
            max_index=i;
    }
}

template <class T>
void ordered_insert(T data[ ], int& n, T entry){        //insert entry into the sorted array
                                                        //data with length n
      int index=first_ge(data,n,entry);
      //cout<<"index: "<<index<<endl;
      T temp=data[index];

      for(int i=index+1; i<n+1; i++){
          T t=data[i];
          data[i]=temp;
          temp=t;
      }
      data[index]=entry; n++;
}

template <class T>
int first_ge(const T data[ ], int n, const T& entry){   //return the first element in data that is
                                                        //not less than entry
    for(int i=0; i<n; i++){
        if(data[i]>=entry)
            return i;
    }
    return n;
}

template <class T>
void attach_item(T data[ ], int& n, const T& entry){   //append entry to the right of data
    data[n]=entry;
    n++;
}

template <class T>
void insert_item(T data[ ], int index, int& n, T entry){    //insert entry at index i in data
    T temp=data[index];

    for(int i=index+1; i<n+1; i++){
        T t=data[i];
        data[i]=temp;
        temp=t;
    }
    data[index]=entry; n++;
}

template <class T>
void detach_item(T data[ ], int& n, T& entry){          //remove the last element in data and place
                                                        //it in entry
    entry=data[n-1];
    //data[n-1]=NULL;
    n--;
}

template <class T>
void delete_item(T data[ ], int i, int& n, T& entry){   //delete item at index i and place it in entry
    entry=data[i];
    for(int j=i; j<n-1; j++){
        T temp=data[j+1];
        data[j]=temp;
    }
    //data[n-1]=NULL;
    n--;
}

template <class T>
void merge(T data1[ ], int& n1, T data2[ ], int& n2){   //append data2 to the right of data1
    for(int i=0; i<n2; i++){
        data1[n1+i]=data2[i];
    }
}

template <class T>
void split(T data1[ ], int& n1, T data2[ ], int& n2){   //move n/2 elements from the right of data1
                                                        //and move to data2
    int half=n1/2; int j=0;
    for(int i=n1-half; i<n1; i++){
        data2[j]=data1[i];
        //data1[i]=NULL;
        j++;
    }
    n1-=half;
    n2=half;
}

template <class T>
void copy_array(T dest[], const T src[],int& dest_size, const int src_size){
                                                //copy src[] into dest[]
        T* destwalker = dest;
        const T* Swalker=src;
        for(int i=0; i<src_size; i++){
            *destwalker=*Swalker;
            destwalker++;
            Swalker++;
        }
        dest_size=src_size;
}

template <class T>
void print_array(const T data[], int n, int pos){  //print array data
    cout<<"[";
    for(int i=0; i<n; i++){
        cout<<" "<<data[i];
    }
    cout<<" ]";
}

template <class T>
bool is_gt(const T data[], int n, const T& item){       //item > all data[i]
    for(int i=0; i<n; i++){
        if(item<data[i])
            return false;
    }
    return true;
}

template <class T>
bool is_le(const T data[], int n, const T& item){       //item <= all data[i]
    for(int i=0; i<n; i++){
        if(item>data[i])
            return false;
    }
    return true;
}
//-------------- Vector Extra operators: ---------------------
template <typename T>
ostream& operator<<(ostream& outs, const vector<T>& v){
    for (int i= 0; i<v.size(); i++)
        outs<<v[i]<<" ";
    return outs;
}

template <typename T>
vector<T> operator+(const vector<T>& list1, const vector<T>& list2){
    //cout<<"+  operator"<<endl;
    vector<T> result;
    for (int i= 0; i<list1.size(); i++)
        result.push_back(list1[i]);
    for (int i= 0; i<list2.size(); i++)
        result.push_back(list2[i]);
    //cout<<"result: "<<result<<endl;
    //cout<<"result size: "<<result.size()<<endl;
    return result;
}

template <typename T>
vector<T>& operator +=(vector<T>& list, const T& addme){ //list.push_back addme
    list.push_back(addme);
    return list;
}

#endif // BTREE_ARRAY_FUNCIONS_H
