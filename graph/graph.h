#ifndef GRAPH_H
#define GRAPH_H
#include <iostream>
#include <set>
#include <queue>
#include "../../../Cs8/!includes/resize/resize.h"
using namespace std;

template <typename T>
class graph{
public:

    graph(int c=3){
        many_vertices = 0;
        capacity=c;
        labels= new T[capacity];
        edges= new bool*[capacity];
        for (int i = 0; i < capacity; ++i)
            edges[i] = new bool[capacity];
    }

    void add_vertex(const T& label);
    void add_edge(int source, int target);
    void remove_edge(int source,int target);

    T get_labels(int vertex){
        return labels[vertex];
    }

    bool get_edges(int row, int col){
        return edges[row][col];
    }

    void update_edges(int source, int target, bool b){
        edges[source][target]=b;
    }

    int size() const {
        return many_vertices;
    }

    bool path_exists(int start, int end);  // question 15.4

    bool is_edge(int source, int target) const;

    set<int> neighbors(int vertex) const;

    T& operator [ ] (int vertex) {
        assert(vertex < size());
        return labels[vertex];
    }
    void print(){
        for(int i=0; i<many_vertices; i++){
            set<int> n=neighbors(i);
            cout<< i<<"-> ";
            set<int>::iterator it;
            for (it = n.begin( ); it != n.end( ); ++it) {
                cout<<*it<<", ";
            }
            cout<<endl;
        }
        cout<<endl;
    }


private:
    bool** edges;
    T* labels;
    int many_vertices;
    int capacity;
};

template <typename T>
void graph<T> :: add_vertex(const T& label){
    int new_vertex_number;
    int other_number;
    new_vertex_number = many_vertices;
    ++many_vertices;
    for (other_number = 0; other_number < many_vertices; ++other_number) {
        edges[other_number][new_vertex_number] = false;
        edges[new_vertex_number][other_number] = false;
    }
    labels[new_vertex_number] = label;
    if(capacity==size()){
        labels=reallocate_l(capacity,labels,many_vertices);
        edges=reallocate_e(capacity,edges,many_vertices);
    }
}

template <typename T>
void graph<T> :: add_edge(int source, int target){
    assert(source < size( ));
    assert(target < size( ));
    edges[source][target] = true;
}

template <typename T>
bool graph<T> :: is_edge(int source, int target) const{
    assert(source < size( ));
    assert(target < size( ));
    return edges[source][target];
}

template <typename T>
void graph<T> :: remove_edge(int source,int target){
    assert(source < size( ));
    assert(target < size( ));
    edges[source][target] = false;
}

template <typename T>
set<int> graph<T> ::neighbors(int vertex) const{
    set<int> answer;
    assert(vertex < size( ));

    for (int i = 0; i < size( ); ++i){
        if (edges[vertex][i])
            answer.insert(i);
    }
    return answer;
}


template <typename T>
bool graph<T> :: path_exists(int start, int end){
    bool exists=false;

    if(edges[start][end])
        exists=true;
    else{
        set<int> connections=neighbors(start);
        set<int>::iterator it;
        it = connections.begin( );
        while(it != connections.end() && !exists){
            exists=path_exists(*it,end);
            it++;
        }
    }
    return exists;

}

//-----------------------------TRAVERSAL FUNCTIONS------------------------------
template <class Process, class Item, class SizeType>
void rec_dfs(Process f, graph<Item>& g, SizeType v, bool marked[ ]){
    set<int> connections = g.neighbors(v);
    set<int>::iterator it;
    marked[v] = true; // Mark vertex v.
    f(g[v]); // Process the label of vertex v with the function f.

    // Traverse all the neighbors, looking for unmarked vertices:
    for (it = connections.begin( ); it != connections.end( ); ++it) {
        if (!marked[*it])
        rec_dfs(f, g, *it, marked);
    }
}

template <class Process, class Item, class SizeType>
void depth_first(Process f, graph<Item>& g, SizeType start){
    bool marked[g.MAXIMUM];
    assert(start < g.size( ));
    fill_n(marked, g.size( ), false);
    rec_dfs(f, g, start, marked);
}

template <class Process, class Item, class SizeType>
void breadth_first(Process f, graph<Item>& g, SizeType start){
    bool marked[g.MAXIMUM];
    set<int> connections;
    set<int>::iterator it;
    queue<int> vertex_queue;
    assert(start < g.size( ));
    fill_n(marked, g.size( ), false);
    marked[start] = true;
    f(g[start]);
    vertex_queue.push(start);

    do{
        connections = g.neighbors(vertex_queue.front( ));
        vertex_queue.pop( );

        // Mark and process the unmarked neighbors, and place them in the queue.
        for (it = connections.begin( ); it != connections.end( ); ++it){
            if (!marked[*it]){
                marked[*it] = true;
                f(g[*it]);
                vertex_queue.push(*it);
            }
        }
    }while (!vertex_queue.empty( ));
}

#endif // GRAPH_H
