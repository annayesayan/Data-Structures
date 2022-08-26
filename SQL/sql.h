#ifndef SQL_H
#define SQL_H
#include <fstream>
#include "../../../Cs8/!includes/table/table.h"
#include "../../../Cs8/!includes/Parsor/parsor.h"

class SQL{
public:
    SQL(){
        fstream my_file;
        my_file.open("tables.txt");
        string temp;
        while (getline(my_file, temp)) {
            tables.push_back(temp);
        }
    }
    SQL(string f){
        file_name=f;
        fstream my_file;
        my_file.open("tables.txt");
        string temp;
        while (getline(my_file, temp)) {
            tables.push_back(temp);
        }
    }
    void set_fname(string f){
        file_name=f;
    }
    void run();
    void interactive();

private:
    void parsor_table(char s[],ofstream& my_tables);
    void delete_tables();
    void batch1();
    bool table_exists(string name);
    string file_name;
    vector<string> tables;
    vector<string> sub_tables;
};

void SQL::run(){
    cout<<"------ SQL Tables I Manage: --------"<<endl;
    cout<<tables<<endl;
    cout<<"------------------------------------"<<endl;

    string line;
    fstream myfile;
    ofstream my_tables ("tables.txt");
    myfile.open(file_name);
    int command_num=0;

    if(!myfile.is_open()) {
      perror("Error open");
      exit(EXIT_FAILURE);
    }
    while(getline(myfile, line)) {
        if (line == "") continue;
        if(line[0]!='/'){
            cout<<endl;
            cout << "["<<command_num<<"] " <<line << endl<<endl;
            command_num++;
            int n = line.length();
            char char_array[n];
            strcpy(char_array, line.c_str());
            parsor_table(char_array, my_tables);
            //cout<<"after parse"<<endl;
        }
        else
            cout<<line<<endl;
    }
    my_tables.close();
    myfile.close();
    delete_tables();  //deletes sub tables

}

void SQL::parsor_table(char s[],ofstream& my_tables){
    //cout<<"in sql parse func"<<endl;
    //cout<<"char s[]: "<<s<<endl;
    Parsor p(s);
    //ofstream myfile ("tables.txt");
    if(p.fail())
        cout<<"Invalid command!"<<endl;
    else{

        //cout<<"in else"<<endl;
        MMap<string,string> ptree=p.p_tree();

        //cout<<ptree<<endl;
        vector<string> table_name=ptree["table"];
        //cout<<"table_name: "<<table_name<<endl;

        vector<string> command=ptree["Command"];
        if(command[0]=="make"){
        if(!table_exists(table_name[0]))
            tables.push_back(table_name[0]);
            Table t(table_name[0], ptree["condition"]);
            //cout<<"table content-->"<<endl;
            cout<<t<<endl;
            if (my_tables.is_open())            //write table names to file
              my_tables << table_name[0]<<endl;
        }else if (command[0]=="insert") {
            if(table_exists(table_name[0])){
                Table t(table_name[0]);
                t.insert_into(ptree["condition"]);
                //cout<<"table content-->"<<endl;
                cout<<t<<endl;
            }else
                cout<<"Sorry table does not exist!"<<endl;
        }else if (command[0]=="select"){

            Table t(table_name[0]);
            //cout<<"tbl content-->"<<endl;
            //cout<<t<<endl;
            if(ptree["field"][0]=="*"&& !ptree.contains("condition")){
                Table temp=t.select_all();
                //cout<<"sub table content-->"<<endl;
                cout<<temp<<endl;
                sub_tables.push_back(temp.filename);
            }else if(!ptree.contains("condition")){
                vector<string> v;
                Table temp=t.select(ptree["field"], v);
                //cout<<"sub table content-->"<<endl;
                cout<<temp<<endl;
                sub_tables.push_back(temp.filename);
            }
            else{
                Table temp=t.select(ptree["field"],ptree["condition"]);
                //cout<<"sub table content-->"<<endl;
                cout<<temp<<endl;
                sub_tables.push_back(temp.filename);
            }
            cout<<"SQL: DONE."<<endl<<endl;
        }
    }

}

void SQL::delete_tables(){
    const bool debug=false;

    for(int i=0; i<sub_tables.size(); i++){
        if(debug){
            if(remove(sub_tables[i].c_str()) != 0)
                perror("Error deleting file");
            else
                puts("File successfully deleted");
        }else
            remove(sub_tables[i].c_str());
    }
}

bool SQL::table_exists(string name){

    for(int i=0; i<tables.size(); i++){
        if(tables[i]==name)
            return true;
    }
    return false;
}

void  SQL::interactive(){
    string filename;
    char choice;

    do{
        cout<<"..   ..   ..   ..   ..   ..   ..   ..   ..   ..   ..   ..   ..   "
              "..   ..   "<<endl;
        cout<<"Batch[1] Batch[2] [F]ile name e[X]it  :";
        cin>>choice; cout<<endl;

        switch (toupper(choice)) {
            case '1':
                set_fname("_!sample.txt");
                run();
                break;
            case '2':
                set_fname("_!select.txt");
                run();
                break;
            case 'F':
                cin>>filename;
                set_fname(filename);
                run();
                break;
        }
    }while (toupper(choice)!='X');
}

#endif // SQL_H
