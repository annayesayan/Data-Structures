#ifndef PARSOR_FUNCS_H
#define PARSOR_FUNCS_H
#include <iostream>
using namespace std;

const int ROWS=20;
const int COLUMNS=20;


//Fill all cells of the array with -1
void init_table(int _table[][COLUMNS]);

//Mark this state (row) with a 1 (success)
void mark_success(int _table[][COLUMNS], int state);

//Mark this state (row) with a 0 (fail)
void mark_fail(int _table[][COLUMNS], int state);

//true if state is a success state
bool is_success(int _table[][COLUMNS], int state);

//Mark a range of cells in the array.
void mark_cells(int row, int _table[][COLUMNS], int from, int to, int state);

//Mark columns represented by the string columns[] for this row
//void mark_cells(int row, int _table[][COLUMNS], const char columns[], int state);

//Mark this row and column
void mark_cell(int row, int table[][COLUMNS], int column, int state);

//This can realistically be used on a small table
void print_table(int _table[][COLUMNS]);

//show string s and mark this position on the string:
//hello world   pos: 7
//       ^
void show_string(char s[], int _pos);

/*----------------------------------------------------------------------------*/
//DEFINITIONS
void init_table(int _table[][COLUMNS]){
    for(int i=0; i < ROWS ; i++){        //init
        for(int j=0; j<COLUMNS; j++){
            _table[i][j]=-1;
        }
    }
}

void mark_success(int _table[][COLUMNS], int state){
    _table[state][0]=1;
}

void mark_fail(int _table[][COLUMNS], int state){
    _table[state][0]=0;
}

bool is_success(int _table[][COLUMNS], int state){
    return _table[state][0];
}

void mark_cell(int row, int table[][COLUMNS], int column, int state){
    table[row][column]=state;
}
void mark_cells(int row, int _table[][COLUMNS], int from, int to, int state){
    for (int i=from; i<=to; i++){
        _table[row][i]=state;
    }
}

void mark_cells(int row, int _table[][COLUMNS], const char columns[], int state){
    for (int i=0; i<sizeof(columns); i++){
        _table[row][columns[i]]=state;
    }
}

#endif // PARSOR_FUNCS_H
