#ifndef HEAP_FUNCTIONS_H
#define HEAP_FUNCTIONS_H
#include <iostream>
using namespace std;


//DECLARATIONS
int parent_index(int my_index);
int left_child_index(int my_index);
int right_child_index(int my_index);


//DEFINITIONS
int parent_index(int my_index){
    return (my_index-1)/2;
}
int left_child_index(int my_index){
    return 2*my_index+1;
}
int right_child_index(int my_index){
     return 2*my_index+2;
}





#endif // HEAP_FUNCTIONS_H
