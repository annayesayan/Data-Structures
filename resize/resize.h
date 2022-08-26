#ifndef RESIZE_H
#define RESIZE_H
template <typename T>
T* reallocate_l(int& capacity, T* src,int size);

template <typename T>
void copy_l(T* dest, T* src,int size);

template <typename T>
T** reallocate_e(int& capacity,T** src,int size);

template <typename T>
void copy_e(T** dest, T** src,int size);




template <typename T>
T* reallocate_l(int& capacity, T* src, int size){
    capacity*=1.5;
    T* newlist= new T [capacity];
    copy_l(newlist,src,size);
    delete [] src;
    return newlist;
}

template <typename T>
void copy_l(T* dest, T* src, int size){
    T* destwalker = dest;
    T* Swalker=src;
    for(int i=0; i<size; i++){
        *destwalker=*Swalker;
        destwalker++;
        Swalker++;
    }
}

template <typename T>
T** reallocate_e(int& capacity, T** src, int size){
    capacity*=1.5;
    T** newlist= new T*[capacity];
    for (int i = 0; i < capacity; ++i)
        newlist[i] = new T[capacity];
    copy_e(newlist,src,size);
    delete [] src;
    return newlist;
}


template <typename T>
void copy_e(T** dest, T** src, int size){
    for(int i=0; i<size; i++){
        for(int j=0; j<size ; j++){
            dest[i][j]=src[i][j];
        }
    }

}
#endif // RESIZE_H
