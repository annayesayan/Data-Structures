#ifndef RECORD_H
#define RECORD_H
 const int MINIMUM = 1;
 const int MAXIMUM = 2 * MINIMUM;

template <typename T>
struct Record{
    Record(){
        for(int i=0; i<MAXIMUM+2; i++){
            children[i]=-1;
        }
        data_count=0;
        child_count=0;
        deleted=false;
    }

    Record(T d[], long c[], int d_count, int c_count){
        for(int i=0; i<d_count; i++)
            data[i]=d[i];
        for(int i=0; i<c_count; i++)
            children[i]=c[i];
        data_count=d_count;
        child_count=c_count;
        deleted=false;
    }

// member variables
    T data[MAXIMUM];
    long children[MAXIMUM+2];
    int data_count;
    int child_count;
    bool deleted;
};

#endif // RECORD_H
