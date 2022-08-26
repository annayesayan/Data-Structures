#ifndef GRAPH_15_3_H
#define GRAPH_15_3_H
#include "../../../Cs8/!includes/graph/graph.h"

template <typename T>
class graph2: public graph<T>{
public:
    graph2(int c=3):graph<T>(c){
        cap=c;
        edge_label= new T*[cap];
        for (int i = 0; i < cap; ++i)
            edge_label[i] = new T[cap];
    }

    void add_edge(int source, int target, T label);
    void remove_edge(int source,int target);
    void add_vertex(const T& label);
    void print();
private:
    T** edge_label;
    int cap;
};

template <typename T>
void graph2<T> :: add_vertex(const T& label){
    graph<T>::add_vertex(label);
    if(graph<T>::size()==cap){
        edge_label=reallocate_e(cap,edge_label,cap);
    }
}

template <typename T>
void graph2<T>::add_edge(int source, int target, T label){
    assert(source < graph<T>::size( ));
    assert(target < graph<T>::size( ));
    graph<T>::add_edge(source,target);
    edge_label[source][target]=label;
}

template <typename T>
void graph2<T>::remove_edge(int source,int target){
    assert(source < graph<T>::size( ));
    assert(target < graph<T>::size( ));
    graph<T>::remove_edge(source,target);
    edge_label[source][target]=-1;
}

template <typename T>
void graph2<T>:: print(){
    for(int i=0; i<graph<T>::size(); i++){
        set<int> n=graph<T>::neighbors(i);
        cout<< i<<"-> ";
        set<int>::iterator it;
        for (it = n.begin( ); it != n.end( ); ++it) {
            cout<<*it<<"("<<edge_label[i][*it]<<"),";
        }
        cout<<endl;
    }
    cout<<endl;
}

#endif // GRAPH_15_3_H
