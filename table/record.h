#ifndef RECORD_H
#define RECORD_H
#include <iostream>
#include <fstream>
#include <iomanip>
//#include "../../../CS3A/11P1_Vector_class/vector.h"
#include <vector>
using namespace std;

//typedef my_vector<string> vectorstr;
typedef vector<string> vectorstr;


class Record{
public:
    Record(){
//        for(int i=0; i<ROW_MAX; i++)
//            for(int j=0; j<COL_MAX; j++)
//            record[i][j]=NULL;    //rec[i]0=nullptr
//        recno = -1;
//        num_of_fields=0;
        for (int i = 0; i<ROW_MAX; i++)
                    record[i][0] = NULL;
                recno = -1;
    }

    Record(vectorstr v){   //arg vector<string>
        for (int i = 0; i<ROW_MAX; i++)
                    record[i][0] = NULL;
                for (int i=0; i<v.size(); i++)
                    strcpy(record[i], v[i].c_str());
    }
// return vector from 2d array
    vectorstr get_vector(){
        vectorstr v;
        int i=0;
        while (record[i][0]!=NULL) {
            string s=record[i];
            //cout<<"{ "<<s<<endl;
            v.push_back(s);
            i++;
        }
        return v;
    }
//    void insert_string(string a, int row){
//        for(int i=0; i<a.length(); i++){
//            record[row][i]=a[i];
//        }
//    }
    int fields(){
        return num_of_fields;
    }

    bool empty(){
        for(int i=0; i<ROW_MAX; i++){
            if(record[i][0]!=NULL)
               return false;
        }
        return true;
    }

    void update_num_fields(int n){
        num_of_fields=n;
    }

    long write(fstream& outs){
        //write to the end of the file.
        outs.seekg(0,outs.end);
        long pos = outs.tellp();

        outs.write(&record[0][0], sizeof(record));
        //cout<<"in record write, pos="<<pos/sizeof(record)<<endl;

        return pos/sizeof(record);
    }

    long read(fstream& ins, long recno){
        long pos= recno * sizeof(record);
        //long pos=recno;
        ins.seekg(pos, ios_base::beg);

        //ins.read(reinterpret_cast<char*>(record), sizeof(record));
        ins.read(&record[0][0], sizeof(record));
        return ins.gcount();
    }

    friend ostream& operator<<(ostream& outs, const Record& r){
        for(int i=0; i<ROW_MAX;i++){
            outs<<setw(15)<<r.record[i]<<" ";
        }
        outs<<endl;
        return outs;
    }

    int rec_no(){
        return recno;
    }
    int num(){
        return num_of_fields;
    }

private:
    static const int ROW_MAX = 10;
    static const int COL_MAX = 500;
    int num_of_fields;
    int recno;
    char record[ROW_MAX][COL_MAX];
};
#endif // RECORD_H
