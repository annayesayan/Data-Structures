#include "../../!includes/stokenizer/token.h"

Token::Token(){}

Token::Token(string str, int type){
    _token=str;
    _type=type;
}


int Token::type(){
    return _type;
}

string Token::type_string(){
    if(_type==1)
        return "ALPHA";
    if(_type==2)
        return "NUM";
    if(_type==3)
        return "PUNC";
    if(_type==4)
        return "SPACE";
    else{
        //cout<<"unknown type num: "<<_type<<endl;
        return "UNKnOWN";
    }

}

string Token:: token_str(){
    return _token;
}
