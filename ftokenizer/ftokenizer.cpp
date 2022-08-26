#include "../../!includes/ftokenizer/ftokenizer.h"

FTokenizer::FTokenizer(string fname)
{
   cout<<"in F constructor"<<endl;
    _f.open(fname);
    if(_f.is_open()){
        cout<<"File opened!!"<<endl;
    }else
        cout<<"File did not open!"<<endl;
    _more=true;
    get_new_block();
}


bool FTokenizer:: more(){
    return _more;
}

Token FTokenizer:: next_token(){
    Token t;
    if(_stk.done()){     //if stk is done get another block
        get_new_block();
    }
    if(_more){ //if theres more tokens read
        _stk>>t;
    }
    return t; //return read token
}

bool FTokenizer::get_new_block(){
    //cout<<"in get new block()"<<endl;
    char _buffer[MAX_BUFFER];

    if(!_f.eof()){
        //cout<<"not end of file!"<<endl;
        _f.read(_buffer,MAX_BUFFER+1);
        int length=_f.gcount();
        _buffer[length]=NULL;
        //cout<<"buffer: "<<_buffer<<endl;
        _stk.set_string(_buffer);
        return true;
    }else{
        _more=false;
        return false;
    }
}
