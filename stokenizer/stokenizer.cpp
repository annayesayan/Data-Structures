#include "../../!includes/stokenizer/stokenizer.h"
#include "../../!includes/stokenizer/functions.h"


int STokenizer::_table[MAX_ROWS][MAX_COLUMNS];

STokenizer::STokenizer()
{
    _pos=0;
    make_table(_table);
}

STokenizer::STokenizer(char str[])
{
    for (int i=0; i<MAX_BUFFER; i++){
        _buffer[i]=str[i];
    }
    _pos=0;
    make_table(_table);
    //cout<<"_buffer: "<<_buffer<<endl;
}
void STokenizer:: set_string(char str[]){
    strcpy (_buffer,str);
    _pos=0;
}

void STokenizer::make_table(int _table[][MAX_COLUMNS]){
    init_table(_table);
    mark_success(_table,1);
    mark_success(_table,4);
    mark_success(_table,2);
    mark_success(_table,3);
    mark_fail(_table,5);

    mark_cells(0,_table,97,122,1); //ALPHA state 1.
    mark_cells(1,_table,97,122,1);
    mark_cells(0,_table,65,90,1);
    mark_cells(1,_table,65,90,1);

    mark_cells(0,_table,48,57,2); //NUM state 2.
    mark_cells(2,_table,48,57,2);
    mark_cell(2,_table,46,5);
    mark_cells(5,_table,48,57,2);

    mark_cells(0,_table,33,47,3); //PUNC state 3
    mark_cells(3,_table,33,47,3);
    mark_cells(0,_table,58,64,3);
    mark_cells(3,_table,58,64,3);
    mark_cells(0,_table,91,96,3);
    mark_cells(3,_table,91,96,3);

    mark_cell(0,_table,32,4);//SPACE state 4
    mark_cell(4,_table,32,4);

    mark_cell(0,_table,13,4);//enter state 4 (carriage return)
    mark_cell(4,_table,13,4);

    mark_cell(0,_table,10,4);//LF state 4 (new line, line feed)
    mark_cell(4,_table,10,4);

    mark_cell(0,_table,9,4);//tab (horizontal tab)
    mark_cell(4,_table,9,4);

    mark_cell(0,_table,11,4);//tab (vertical tab)
    mark_cell(4,_table,11,4);

    mark_cell(0,_table,12,4);//form feed
    mark_cell(4,_table,12,4);
}

int STokenizer::get_token(int start_state, string& token){
    int row=0;
    int begin=_pos;
    int success=-1;
    //cout<<"_pos: "<<_pos<<endl;
    //cout<<"in stk, _buffer[_pos]: "<<_buffer[_pos]<<endl;
    int col=_buffer[_pos];
    if(col>=0 && col <= 128){
        int state=_table[row][col];

        while(state!=-1  && _pos<strlen(_buffer)){
            if(_table[state][0]==1){
                 success=_pos; //keep track of success
            }
            row=state;
            _pos++;
            col=_buffer[_pos];
            state=_table[row][col];
        }
        _pos=success+1;

        for(int i=begin; i<_pos; i++){
            token+=_buffer[i];
        }

        col=_buffer[_pos-1];
        state=_table[row][col];
        return state;
    }
    else{
        token+=_buffer[_pos];
        _pos++;
        return 0;
    }
}

bool STokenizer::done(){
    return(_pos==strlen(_buffer));
}
