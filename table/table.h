
#ifndef TABLE_H
#define TABLE_H
#include <iostream>
#include <fstream>

#include "../../../Cs8/!includes/table/record.h"

#include "../../../Cs8/!includes/BPT_map/bpt_map.h"
#include "../../../Cs8/!includes/BPT_Multi_Map/bpt_mmap.h"

#include "../../../Cs8/!includes/table/shunting_y.h"
using namespace std;
int serial_num=0;


class Table{
public:
    Table(){
    }
    Table(string file_name){
        assert(file_exists(file_name));
        filename=file_name;
        build_indices();
    }

    Table(string file_name, vector<string> f){
        //cout<<"in constructor"<<endl;

        for(int i=0; i<f.size(); i++){
            field_map.insert(f[i],i);
            MMap<string,long> temp;
            //cout<<"before push"<<endl;
            indices.push_back(temp);
        }
        field_names=f;
        filename=file_name;
        Record r(f);
        //save_record(r);
        fstream fs;
        open_fileW(fs, filename);
        r.write(fs);
        fs.close();

    }

    Table select_all();
    Table select(vector<string> f, vector<string> condition);
    vector<long> rpn(Queue<string> postfix);
    vector<long> intersection(vector<long> r1, vector<long> r2);
    vector<long> Or(vector<long> r1, vector<long> r2);

    void insert_into(vector<string> f);
    Record read(long recno);

    bool empty(); //no records have been added

    friend ostream& operator<<(ostream& outs, Table& print_me){
        outs<<"Table name: "<<print_me.filename<<", records: "<<print_me.records_num<<endl;
        fstream f;
        print_me.open_fileRW(f,print_me.filename); //W

        cout<<"================================================================"
              "============================================"<<endl;
        outs<<setw(15)<<"record";
        for(int i=0; i<print_me.field_names.size(); i++)
            outs<<setw(15)<<print_me.field_names[i];
        outs<<endl;
        cout<<"================================================================"
              "============================================"<<endl;
        long i=1; int p=0;;
        Record r;
        r=print_me.read(i);
        while (!r.empty()){
            cout<<"-------------------------------------------------------------"
                  "-------------------------------------------"<<endl;
            outs<<setw(15)<<p<<" "<<r;
            i++;
            r=print_me.read(i);
            p++;
        }
        cout<<"================================================================="
              "==========================================="<<endl<<endl;
        f.close();
        //cout<<"end of table output"<<endl;

        return outs;
    }
    string filename;  //table name
private:
    //int serial_num=0;
    int records_num=0;

    vector<string> field_names;

    vector< MMap<string,long> > indices;

    Map<string, int> field_map;

    void build_indices();

    void open_fileW(fstream& f, string filename);
    void open_fileRW(fstream& f, string filename) throw(char*);
    bool file_exists(string filename);
    long save_record(Record r);
};


void Table::insert_into(vector<string> f){

    Record r(f);
    //cout<<"in write record: "<<r.get_vector()<<endl;
    long pos=save_record(r);
    for(int i=0; i<indices.size(); i++){
        indices[i].insert(f[i],pos);
    }
    records_num++;
    //cout<<"in insert into: "<<indices<<endl;
    //cout<<"indices size: "<<indices.size()<<endl;
}

Record Table::read(long recno){
    Record r;
    fstream f;
    open_fileRW(f, filename);
    r.read(f, recno);
    //r.update_num_fields(field_map.size());
    //cout<<"end of read: "<<r<<endl;
    return r;
}

Table Table:: select(vector<string> f, vector<string> condition){
    //cout<<"in select"<<endl;

    serial_num++;
    string file =filename +"_"+to_string(serial_num);

    if(!condition.empty()){
        Queue<string> infix;
        for(int i=0; i<condition.size(); i++){
            infix.push(condition[i]);
        }

        //cout<<"infix: "<<infix<<endl;
        shunting_yard shunt(infix);
        Queue<string> postfix=shunt.postfix();
        //cout<<"postfix: "<<postfix<<endl;
        vector<long> recnos =rpn(postfix);
        //cout<<"recnos: "<<recnos<<endl;


        if(f[0]=="*"){
            Table t(file,field_names);
            for(int i=0; i<recnos.size(); i++){
                Record r=read(recnos[i]);
                vector<string> temp=r.get_vector();
                t.insert_into(temp);
            }
            return t;
        }else{
            //cout<<"in else statement"<<endl;
            Table t(file,f);                                   //new table
            for(int i=0; i<recnos.size(); i++){
                Record r=read(recnos[i]);
                vector<string> temp=r.get_vector();
                vector<string> final;
                for(int j=0; j<f.size(); j++){
                    int i=field_map.at(f[j]);
                    final.push_back(temp[i]);
                }
                t.insert_into(final);
                //cout<<"final table has: "<<t<<endl;
            }
            return t;
        }
    }else{
        Table t(file,f);
        for(int i=1; i<=records_num; i++){
            Record r=read(i);
            vector<string> temp=r.get_vector();
            vector<string> final;
            for(int j=0; j<f.size(); j++){
                int i=field_map.at(f[j]);
                final.push_back(temp[i]);
            }
            t.insert_into(final);
        }
        return t;
    }




}

vector<long> Table:: rpn(Queue<string> postfix){
    //cout<<"in rpn"<<endl;

    Queue<string> copy=postfix;
    Stack<vector<long>> r;

    while (!copy.empty()) {
        string temp=copy.pop();
        if(temp=="and"){
            //cout<<"in and"<<endl;
            vector<long> rec1=r.pop();
            vector<long> rec2=r.pop();
            r.push(intersection(rec1,rec2));
            //cout<<"stack in rpn: "<<r<<endl;
        }else if (temp=="or") {
            //cout<<"in or"<<endl;
            vector<long> rec1=r.pop();
            vector<long> rec2=r.pop();
            r.push(Or(rec1,rec2));
            //cout<<"stack in rpn: "<<r<<endl;
        }
        else{
            //cout<<"in rpn else"<<endl;
            //cout<<"indices:"<<endl;
            //cout<<indices<<endl;
            string op=copy.pop();
            string cond=copy.pop();
            int i=field_map.at(temp);
            //cout<<"temp: "<<temp<<endl;
            //cout<<"op: "<<op<<endl;
            //cout<<"cond: "<<cond<<endl;
            //cout<<"field_map.at(temp): "<<i<<endl;

                if(op==">"){
                    //cout<<"op = >"<<endl;
                    typename MMap<string,long> :: Iterator it=indices[i].upper_bound(cond);
                    vector<long> v;

                    while (it!=indices[i].end()) {
                        //cout<<"*it: "<<*it<<endl;
                        //cout<<"*it.value_list: "<<(*it).value_list<<endl;
                        v=v+(*it).value_list;
                        it++;
                    }
                    r.push(v);
                }else if (op=="<") {
                    //cout<<"op = <"<<endl;
                    typename MMap<string,long> :: Iterator it=indices[i].begin();
                    vector<long> v;
                    while (it!=indices[i].lower_bound(cond)) {
                        v=v+(*it).value_list;
                        it++;
                    }
                    r.push(v);
                }
                else if(op==">="){
                    //cout<<"op = >="<<endl;
                    typename MMap<string,long> :: Iterator it=indices[i].lower_bound(cond);
                    vector<long> v;
                    while (it!=indices[i].end()) {
                        v=v+(*it).value_list;
                        it++;
                    }
                    r.push(v);
                }else if (op=="<=") {
                    //cout<<"op = <="<<endl;
                    typename MMap<string,long> :: Iterator it=indices[i].begin();
                    vector<long> v;
                    //cout<<"begin: "<<*it<<endl;
                    typename MMap<string,long> :: Iterator t=indices[i].upper_bound(cond);

                    while (it!=indices[i].upper_bound(cond)) {
                        //cout<<"*it: "<<*it<<endl;
                        //cout<<"*it.value_list: "<<(*it).value_list<<endl;
                        v=v+(*it).value_list;
                        it++;
                    }
                    r.push(v);
                }

                else{
                    //cout<<indices[i]<<endl;
                    if(indices[i].contains(cond)){
                        vector<long> recnos= indices[i][cond];
                        //cout<<"in rpn indices[i][cond] "<<endl;
                        //cout<<recnos<<endl;
                        r.push(recnos);
                        //cout<<"stack in rpn: "<<r<<endl;
                    }else
                        r.push(vector<long>());
                }

        }
    }
    if(!r.empty())
        return r.pop();
    else
        return vector<long>();
}

vector<long> Table:: intersection(vector<long> r1, vector<long> r2){
    vector<long> final;
    for(int i=0; i<r1.size(); i++){
        for(int j=0; j<r2.size(); j++){
            if(r2[j]==r1[i])
                final.push_back(r1[i]);
        }
    }
    return final;
}

vector<long> Table:: Or(vector<long> r1, vector<long> r2){
    vector<long> final=r1;
    bool dup=false;
    for(int i=0; i<r2.size(); i++){
        for(int j=0; j<final.size(); j++){
            if(final[j]==r2[i])
                dup=true;
        }
        if(dup==false)
            final.push_back(r2[i]);
        dup=false;
    }
    return final;
}

Table Table:: select_all(){
//    Table all=*this;  //?
//    return all;
    serial_num++;
    Table all(filename+to_string(serial_num), field_names);
    long i=1; int p=0;;

    for(int i=1; i<=records_num; i++){
        Record r;
        r=read(i);
        all.insert_into(r.get_vector());
    }
    return all;

}

bool Table::empty(){
    Record r=read(1); //read second element bc first is field names
    return r.empty();
}

void Table::build_indices(){
    fstream file;
    open_fileRW(file, filename);

    long pos=0;
    Record r=read(pos);
    vector<string> f=r.get_vector();
    field_names=f;
    //cout<<"build_indices: "<<f<<endl;

    for(int i=0; i<f.size(); i++){
        field_map.insert(f[i],i);
        MMap<string,long> temp;
        indices.push_back(temp);
    }
    pos=1;
    r=read(pos);
    f=r.get_vector();
    while (!r.empty()){
        records_num++;
        for(int i=0; i<f.size(); i++){
            indices[i].insert(f[i],pos);
        }
        pos++;
        r=read(pos);
        f=r.get_vector();
    }
}


long Table::save_record(Record r){
    fstream f;
    //open_fileW(f, filename);
    open_fileRW(f, filename);
    long pos=r.write(f);
    f.close();
    //cout<<"in save record pos is: "<<pos<<endl;
    return pos;
}

void Table::open_fileW(fstream& f, string filename){
    f.open (filename,
                std::fstream::out| std::fstream::binary );
        if (f.fail()){
            cout<<"file open failed: "<<filename<<endl;
            throw("file failed to open.");
        }
}

void Table::open_fileRW(fstream& f, string filename) throw(char*){
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

bool Table::file_exists(string filename){
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


































//class Table{
//public:
//    Table(){
//    }
//    Table(string file_name){
//        assert(file_exists(file_name));
//        filename=file_name;
//        build_indices();
//    }

//    Table(string file_name, my_vector<string> f){
//        cout<<"in constructor"<<endl;

//        for(int i=0; i<f.size(); i++){
//            field_map.insert(f[i],i);
//            MMap<string,long> temp;
//            cout<<"before push"<<endl;
//            indices.push_back(temp);
//        }
//        field_names=f;
//        filename=file_name;
//        Record r(f);
//        cout<<"in write record: "<<r.get_vector()<<endl;
//        save_record(r);
//        //insert_into(f);
//    }

////    Table(Table& table){    //copy constructor
////        filename=table.filename+"_copy";  //use serial num
////        field_names=table.field_names;
////        for(int i=0; i<field_names.size(); i++){
////            field_map.insert(field_names[i],i);
////            MMap<string,long> temp;
////            indices.push_back(temp);
////        }
////        insert_into(field_names);
////        long i=1; Record r;
////        do{
////            r=table.read(i);
////            insert_into(r.get_my_vector());
////            i++;
////        }while (!r.empty());
////    }

////    Table& operator =( Table& RHS){
////        filename=RHS.filename+"_copy";  //use serial num
////        field_names=RHS.field_names;
////        for(int i=0; i<field_names.size(); i++){
////            field_map.insert(field_names[i],i);
////            MMap<string,long> temp;
////            indices.push_back(temp);
////        }
////        insert_into(field_names);
////        long i=1; Record r;
////        do{
////            r=RHS.read(i);
////            insert_into(r.get_my_vector());
////            i++;
////        }while (!r.empty());
////        return *this;
////    }

//    Table select_all();
//    Table select(my_vector<string> f, my_vector<string> condition);
//    my_vector<long> rpn(Queue<string> postfix);
//    my_vector<long> intersection(my_vector<long> r1, my_vector<long> r2);

//    void insert_into(my_vector<string> f);
//    Record read(long recno);

//    bool empty(); //no records have been added

//    friend ostream& operator<<(ostream& outs, Table& print_me){
//        outs<<"Table name: "<<print_me.filename<<", records: "<<print_me.records_num<<endl;
//        fstream f;
//        print_me.open_fileRW(f,print_me.filename); //W
//        outs<<setw(15)<<"record";
//        for(int i=0; i<print_me.field_names.size(); i++)
//            outs<<setw(15)<<print_me.field_names[i];
//        outs<<endl;

//        long i=1; int p=0;;
//        Record r;
//        r=print_me.read(i);
//        while (!r.empty()){
//            outs<<setw(15)<<p<<" "<<r;
//            i++;
//            r=print_me.read(i);
//            p++;
//        }

//        f.close();
//        cout<<"end of table output"<<endl;
//        return outs;
//    }

//private:
//    //int serial_num=0;
//    int records_num=0;
//    string filename;  //table name
//    my_vector<string> field_names;

//    my_vector< MMap<string,long> > indices;

//    Map<string, int> field_map;

//    void build_indices();

//    void open_fileW(fstream& f, string filename);
//    void open_fileRW(fstream& f, string filename) throw(char*);
//    bool file_exists(string filename);
//    long save_record(Record r);
//};


//void Table::insert_into(my_vector<string> f){

//    Record r(f);
//    cout<<"in write record: "<<r.get_vector()<<endl;
//    long pos=save_record(r);
//    for(int i=0; i<indices.size(); i++){
//        indices[i].insert(f[i],pos);
//    }
//    records_num++;
//    cout<<"in insert into: "<<indices<<endl;
//    cout<<"indices size: "<<indices.size()<<endl;
//}

//Record Table::read(long recno){
//    Record r;
//    fstream f;
//    open_fileRW(f, filename);
//    r.read(f, recno);
//    //r.update_num_fields(field_map.size());
//    //cout<<"end of read: "<<r<<endl;
//    return r;
//}

//Table Table:: select(my_vector<string> f, my_vector<string> condition){
//    cout<<"in select"<<endl;
////    cout<<"Index Maps: ";
////    for(int i=0; i<indices.size(); i++){
////        cout<<indices[i];
////    }

//    Queue<string> infix;
//    for(int i=0; i<condition.size(); i++){
//        infix.push(condition[i]);
//    }
//    cout<<"infix: "<<infix<<endl;
//    shunting_yard shunt(infix);
//    Queue<string> postfix=shunt.postfix();
//    cout<<"postfix: "<<postfix<<endl;
//    my_vector<long> recnos =rpn(postfix);
//    cout<<"recnos: "<<recnos<<endl;

//    //string file=condition[0]+"_"+condition[2]+".tbl";  //new file name // change to work with serial nums
//    serial_num++;
//    string file =filename +"_"+to_string(serial_num);

//    if(f[0]=="*"){
//        Table t(file,field_names);
//        for(int i=0; i<recnos.size(); i++){
//            Record r=read(recnos[i]);
//            my_vector<string> temp=r.get_vector();
//            t.insert_into(temp);
//        }
//        return t;
//       // return select_all();
//    }else{
//        //cout<<"in else statement"<<endl;
//        Table t(file,f);                                   //new table
//        for(int i=0; i<recnos.size(); i++){
//            Record r=read(recnos[i]);
//            my_vector<string> temp=r.get_vector();
//            my_vector<string> final;
//            for(int j=0; j<f.size(); j++){
//                int i=field_map.at(f[j]);
//                final.push_back(temp[i]);
//            }
//            t.insert_into(final);
//            //cout<<"final table has: "<<t<<endl;
//        }
//        return t;
//    }
//}

//my_vector<long> Table:: rpn(Queue<string> postfix){
//    cout<<"in rpn"<<endl;
//    Queue<string> copy=postfix;
//    Stack<my_vector<long>> r;
//    //my_vector<my_vector<long>> recs;
//    while (!copy.empty()) {
//        string temp=copy.pop();
//        if(temp=="and"){
//            my_vector<long> rec1=r.pop();
//            my_vector<long> rec2=r.pop();
//            r.push(intersection(rec1,rec2));
//        }else if (temp=="or") {
//            my_vector<long> rec1=r.pop();
//            my_vector<long> rec2=r.pop();
//            r.push(rec1+rec2);
//        }
//        else{
//            cout<<"in rpn else"<<endl;
//            cout<<"indices:"<<endl;
//            cout<<indices<<endl;
//            string op=copy.pop();
//            string cond=copy.pop();
//            int i=field_map.at(temp);
//            cout<<"temp: "<<temp<<endl;
//            cout<<"op: "<<op<<endl;
//            cout<<"cond: "<<cond<<endl;
//            cout<<"field_map.at(temp): "<<i<<endl;

//                if(op==">"){
//                    cout<<"op = >"<<endl;
//                    typename MMap<string,long> :: Iterator it=indices[i].upper_bound(cond);
//                    my_vector<long> v;
//                    //cout<<"it="<<*it<<endl;
//                    while (it!=indices[i].end()) {
//                        cout<<"*it: "<<*it<<endl;
//                        cout<<"*it.value_list: "<<(*it).value_list<<endl;
//                        v=v+(*it).value_list;
//                        it++;
//                    }
//                    r.push(v);
//                }else if (op=="<") {
//                    cout<<"op = <"<<endl;
//                    typename MMap<string,long> :: Iterator it=indices[i].begin();
//                    my_vector<long> v;
//                    while (it!=indices[i].lower_bound(cond)) {
//                        //cout<<"*it: "<<*it<<endl;
//                        //cout<<"*it.value_list: "<<(*it).value_list<<endl;
//                        v=v+(*it).value_list;
//                        it++;
//                    }
//                    r.push(v);
//                }
//                else if(op==">="){
//                    cout<<"op = >="<<endl;
//                    typename MMap<string,long> :: Iterator it=indices[i].lower_bound(cond);
//                    my_vector<long> v;
//                    while (it!=indices[i].end()) {
//                        //cout<<"*it: "<<*it<<endl;
//                        //cout<<"*it.value_list: "<<(*it).value_list<<endl;
//                        v=v+(*it).value_list;
//                        it++;
//                    }
//                    r.push(v);
//                }else if (op=="<=") {
//                    cout<<"op = <="<<endl;
//                    typename MMap<string,long> :: Iterator it=indices[i].begin();
//                    my_vector<long> v;
//                    cout<<"begin: "<<*it<<endl;
//                    typename MMap<string,long> :: Iterator t=indices[i].upper_bound(cond);
//                    //cout<<"upper_bound: "<<*t<<endl;
//                    while (it!=indices[i].upper_bound(cond)) {
//                        cout<<"*it: "<<*it<<endl;
//                        cout<<"*it.value_list: "<<(*it).value_list<<endl;
//                        v=v+(*it).value_list;
//                        it++;
//                    }
//                    r.push(v);
//                }

//                else{
//                    cout<<indices[i]<<endl;
//                    if(indices[i].contains(cond)){
//                        my_vector<long> recnos= indices[i][cond];
//                        cout<<"in rpn indices[i][cond] "<<endl;
//                        cout<<recnos<<endl;
//                        r.push(recnos);
//                        cout<<"stack in rpn: "<<r<<endl;
//                    }
//                }

//        }
//    }
//    if(!r.empty())
//        return r.pop();
//    else
//        return my_vector<long>();
//}

//my_vector<long> Table:: intersection(my_vector<long> r1, my_vector<long> r2){
//    my_vector<long> final;
//    for(int i=0; i<r1.size(); i++){
//        for(int j=0; j<r2.size(); j++){
//            if(r2[j]==r1[i])
//                final.push_back(r1[i]);
//        }
//    }
//    return final;
//}

//Table Table:: select_all(){
//    Table all=*this;  //?
//    return all;
//}

//bool Table::empty(){
//    Record r=read(1); //read second element bc first is field names
//    return r.empty();
//}

//void Table::build_indices(){
//    fstream file;
//    open_fileRW(file, filename);

//    long pos=0;
//    Record r=read(pos);
//    //cout<<"number of field in record: "<<r.num()<<endl;
//    my_vector<string> f=r.get_vector();
//    field_names=f;
//    cout<<"build_indices: "<<f<<endl;

//    for(int i=0; i<f.size(); i++){
//        field_map.insert(f[i],i);
//        MMap<string,long> temp;
//        indices.push_back(temp);
//    }
//    pos=1;
//    r=read(pos);
//    f=r.get_vector();
//    while (!r.empty()){
//        records_num++;
//        for(int i=0; i<f.size(); i++){
//            indices[i].insert(f[i],pos);
//        }
//        pos++;
//        r=read(pos);
//        f=r.get_vector();
//    }

////    do{
////        for(int i=0; i<f.size(); i++){
////            indices[i].insert(f[i],pos);
////        }
////        pos++;
////        r=read(pos);
////        f=r.get_vector();
////    }while (!r.empty());

//}


//long Table::save_record(Record r){
//    fstream f;
//    //open_fileW(f, filename);
//    open_fileRW(f, filename);
//    long pos=r.write(f);
//    f.close();
//    cout<<"in save record pos is: "<<pos<<endl;
//    return pos;
//}

//void Table::open_fileW(fstream& f, string filename){
////    f.open (filename,
////            std::fstream::out| std::fstream::binary );
//    f.open (filename,
//            std::fstream::out| std::fstream::binary | std::fstream::app);
//    if (f.fail()){
//        cout<<"file open failed: "<<filename<<endl;
//        throw("file failed to open.");
//    }
//}

//void Table::open_fileRW(fstream& f, string filename) throw(char*){
//    const bool debug = false;
//    //openning a nonexistent file for in|out|app causes a fail()
//    //  so, if the file does not exist, create it by openning it for
//    //  output:
//    if (!file_exists(filename)){
//        f.open(filename, std::fstream::out|std::fstream::binary);
//        if (f.fail()){
//            cout<<"file open (RW) failed: with out|"<<filename<<"]"<<endl;
//            throw("file RW failed  ");
//        }
//        else{
//            if (debug) cout<<"open_fileRW: file created successfully: "<<filename<<endl;
//        }
//    }
//    else{
//        f.open (filename,
//            std::fstream::in | std::fstream::out| std::fstream::binary );
//        if (f.fail()){
//            cout<<"file open (RW) failed. ["<<filename<<"]"<<endl;
//            throw("file failed to open.");
//        }
//    }
//}

//bool Table::file_exists(string filename){
//    const bool debug = false;
//    fstream ff;
//    ff.open (filename,
//        std::fstream::in | std::fstream::binary );
//    if (ff.fail()){
//        if (debug) cout<<"file_exists(): File does NOT exist: "<<filename<<endl;
//        return false;
//    }
//    if (debug) cout<<"file_exists(): File DOES exist: "<<filename<<endl;
//    ff.close();
//    return true;
//}

#endif // TABLE_H
