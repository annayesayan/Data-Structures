#ifndef PARSOR_H
#define PARSOR_H
#include <iostream>
#include "../../../Cs8/!includes/BPT_map/bpt_map.h"
#include "../../../Cs8/!includes/BPT_Multi_Map/bpt_mmap.h"

#include "../../../Cs8/!includes/stokenizer/stokenizer.h"

using namespace std;

enum {ZERO,SELECT,MAKE,INSERT,INTO,FROM,SYMBOL,WHERE,OP};
const int ROWS=20;
const int COLUMNS=20;

class Parsor{
public:
    Parsor(char s[]){
        strcpy(_buffer,s);
        make_matrix();
        build_keyword();
        set_string();
    }

    void make_matrix();
    void build_keyword();
    void set_string();
    bool get_parse_tree();
    bool fail();

    MMap<string,string> p_tree(){
        return ptree;
    }

private:
    char _buffer[200];                       //input string
    int _matrix[ROWS][COLUMNS];    //adjancecy matrix
    Map<string,int> keyword_list;
    MMap<string,string> ptree;
    //my_vector<string> input_q;
    vector<string> input_q;

    bool success_flag;

    //for matrix
    void init(int _table[][COLUMNS]);
    void mark_box(int row, int table[][COLUMNS], int column, int state);
    void make_success(int _table[][COLUMNS], int state);
    bool success(int _table[][COLUMNS], int state);
};

void Parsor::make_matrix(){
    init(_matrix);
    make_success(_matrix,4); //success state //for col
    make_success(_matrix,8);
    make_success(_matrix,14);//make

    mark_box(0,_matrix,SELECT,1);
    mark_box(1,_matrix,SYMBOL,2);
    mark_box(2,_matrix,SYMBOL,2);
    mark_box(2,_matrix,FROM,3);
    mark_box(3,_matrix,SYMBOL,4);
    mark_box(4,_matrix,WHERE,5);
    mark_box(5,_matrix,SYMBOL,6);
    mark_box(6,_matrix,OP,7);
    mark_box(7,_matrix,SYMBOL,8);
    mark_box(8,_matrix,OP,9);
    mark_box(9,_matrix,SYMBOL,6);

    mark_box(0,_matrix,MAKE,10); //make
    mark_box(10,_matrix,SYMBOL,11);
    mark_box(11,_matrix,SYMBOL,12);
    mark_box(12,_matrix,SYMBOL,13);
    mark_box(13,_matrix,SYMBOL,14);
    mark_box(14,_matrix,SYMBOL,14);

    mark_box(0,_matrix,INSERT,15);
    mark_box(15,_matrix,INTO,16);
    mark_box(16,_matrix,SYMBOL,17);
    mark_box(17,_matrix,SYMBOL,18);
    mark_box(18,_matrix,SYMBOL,19);
    mark_box(19,_matrix,SYMBOL,19);
}

void Parsor::build_keyword(){
    keyword_list.insert("select",SELECT);
    keyword_list.insert("from",FROM);
    keyword_list.insert("where",WHERE);
    keyword_list.insert("=",OP);
    keyword_list.insert(">",OP);
    keyword_list.insert("<",OP);
    keyword_list.insert(">=",OP);
    keyword_list.insert("<=",OP);
    keyword_list.insert("and",OP); //AND
    keyword_list.insert("or",OP);//or
    keyword_list.insert("make",MAKE);
    keyword_list.insert("insert",INSERT);
    keyword_list.insert("into",INTO);
}

void Parsor::set_string(){
    STokenizer stk(_buffer);
    Token t;

    //cout<<"stk input: "<<_buffer<<endl;
    while(!stk.done()){
        stk>>t;
        //cout<<t.type_string()<< "   "<<t<<endl;
        if(t.type()!=4 && t.token_str()!=","){
            if(t.token_str()=="\""){
                Token t1;
                stk>>t1;
                //cout<<t1.type_string()<< "   "<<t1<<endl;
                string word;
                //while(t1.token_str()[0]!='\"'){ //punc
                while(t1.type()!=3){ //punc
                    word+=t1.token_str();
                    stk>>t1;
                    //cout<<t1.type_string()<< "   "<<t1<<endl;
                }
                input_q.push_back(word);
            }else
                input_q.push_back(t.token_str());
        }
    }
    success_flag=get_parse_tree();
}

bool Parsor::fail(){
    return !success_flag;
}

bool Parsor::get_parse_tree(){
    int row=0;
    int col=0;
    int size=input_q.size();
    int i=0;

    do{
        string temp=input_q[i];
        //cout<<"temp: "<<temp<<endl;
        if(keyword_list.find(temp)==typename Map<string,int> :: Iterator()){
            //cout<<"token is a symbol"<<endl;
            col=SYMBOL;
        }else
            col=keyword_list[temp];

        row=_matrix[row][col];
        //cout<<"row: "<<row<<"  col: "<<col<<endl;
        //--------make ptree-----
        switch (col) {
            case SELECT:
                ptree.insert("Command",temp);
            break;
            case MAKE:
                ptree.insert("Command",temp);
            break;
            case INSERT:
                ptree.insert("Command",temp);
            break;
            case OP:
                ptree.insert("condition",temp);
            break;
            case SYMBOL:
            //cout<<"case SYMBOL"<<endl;
                if(row==2 || row==11){
                    //cout<<"row=2"<<endl;
                    ptree.insert("field",temp);
                }
                if(row==4 || row==12 || row==17)
                    ptree.insert("table",temp);
                if(row==6 || row==8 || row==14 ||row==19){
                    //cout<<"row==8"<<endl;
                    ptree.insert("condition",temp);
                    //cout<<"after"<<endl;
                }

            break;
            case WHERE:
                ptree.insert("where","yes");
            break;

        }

        i++;
    }while(i!=size);

    if(success(_matrix,row))
        return true;
    else{
        ptree.clear();
        return false;
    }
}


//------------------------------ matrix funcs-----------------------------------

void Parsor::init(int _table[][COLUMNS]){
    for(int i=0; i < ROWS ; i++){        //init
        for(int j=0; j<COLUMNS; j++){
            _table[i][j]=-1;
        }
    }
}

void Parsor::mark_box(int row, int table[][COLUMNS], int column, int state){
    table[row][column]=state;
}

void Parsor::make_success(int _table[][COLUMNS], int state){
    _table[state][0]=1;
}

bool Parsor::success(int _table[][COLUMNS], int state){
    return _table[state][0];
}
#endif // PARSOR_H
