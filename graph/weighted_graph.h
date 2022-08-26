#ifndef WEIGHTED_GRAPH_H
#define WEIGHTED_GRAPH_H
#include "../../../Cs8/!includes/graph/graph.h"

template <typename T>
class weighted_graph: public graph<T>{
public:
    weighted_graph(int c):graph<T>(c){
        cap=c;
        weights= new int*[cap];
        for (int i = 0; i < cap; ++i)
            weights[i] = new int[cap];

        for (int i = 0; i < cap; i++) {
            for(int j=0; j<cap; j++)
                weights[i][j] = -1;
        }
    }

    void add_vertex(const T& label);
    void add_edge(int source, int target, int weight);
    void remove_edge(int source,int target);
    int get_weight(int source, int target);

    int* shortest_distance(int start, int end);
    int* shortest_path(int start, int end);
    int Min(int* distance, bool* visited, int size);

    void print();

private:
    int** weights;
    int cap;
};

template <typename T>
void weighted_graph<T>:: add_vertex(const T& label){
    int new_vertex_number=graph<T>::size();
    graph<T>::add_vertex(label);

    for (int other_number = 0; other_number < graph<T>::size(); other_number++) {
        weights[other_number][new_vertex_number] = -1;
        weights[new_vertex_number][other_number] = -1;
    }
    if(graph<T>::size()==cap){
        weights=reallocate_e(cap,weights,cap);
    }
}

template <typename T>
void weighted_graph<T>::add_edge(int source, int target, int weight){
    assert(source < graph<T>::size( ));
    assert(target < graph<T>::size( ));
    graph<T>::add_edge(source,target);
    weights[source][target]=weight;
}

template <typename T>
void weighted_graph<T>::remove_edge(int source,int target){
    assert(source < graph<T>::size( ));
    assert(target < graph<T>::size( ));
    graph<T>::remove_edge(source,target);
    weights[source][target]=-1;
}

template <typename T>
int weighted_graph<T>::get_weight(int source, int target){
    assert(graph<T>::is_edge(source,target));
    return weights[source][target];
}

template <typename T>
void weighted_graph<T>:: print(){
    for(int i=0; i<graph<T>::size(); i++){
        set<int> n=graph<T>::neighbors(i);
        cout<< i<<"-> ";
        set<int>::iterator it;
        for (it = n.begin( ); it != n.end( ); ++it) {
            cout<<*it<<"("<<weights[i][*it]<<"),";
        }
        cout<<endl;
    }
    cout<<endl;
}

template <typename T>
int* weighted_graph<T>::shortest_distance(int start, int end){
    //cout<<"in shortest dist"<<endl;
    int distances[end];
    bool visited[end];

    for(int i=0; i<=end; i++){
        distances[i]=-1;
        visited[i]=false;
    }
    distances[start]=0;
    set<int> allowed_vertices;
    set<int> neighbours;
    int next, sum=0;

    for (int allowed_size = 1; allowed_size <= end; ++allowed_size) {
        cout<<"distances: ";
        for(int t=0; t<=end; t++)
            cout<<distances[t]<<" ";
        cout<<endl;
        next=Min(distances,visited,end);
        visited[next]=true;
        allowed_vertices.insert(next);
        neighbours=graph<T>::neighbors(next);
        for(int v=0; v<=end; v++){
            if(!allowed_vertices.count(v) && graph<T>::is_edge(next,v)){
                sum=distances[next]+get_weight(next,v);
                if(distances[v]!=-1 && sum<distances[v])
                    distances[v]=sum;
                else if (distances[v]==-1)
                    distances[v]=sum;
            }
        }
    }
    cout<<"---end of shortest distance---"<<endl;
    int* d=distances;
    return d;
}

template <typename T>
int* weighted_graph<T>::shortest_path(int start, int end){
    int distances[end];
    int predecessor[end];
    bool visited[end];

    for(int i=0; i<=end; i++){
        distances[i]=-1;
        visited[i]=false;
    }
    distances[start]=0;
    set<int> allowed_vertices;
    set<int> neighbours;
    int next, sum=0;

    for (int allowed_size = 1; allowed_size <= end; ++allowed_size) {
        cout<<"distances: ";
        for(int t=0; t<=end; t++){
            cout<<distances[t]<<" ";

        }
        next=Min(distances,visited,end);
        visited[next]=true;
        allowed_vertices.insert(next);
        neighbours=graph<T>::neighbors(next);
        for(int v=0; v<=end; v++){
            if(!allowed_vertices.count(v) && graph<T>::is_edge(next,v)){
                sum=distances[next]+get_weight(next,v);
                if(distances[v]!=-1 && sum<distances[v]){
                    distances[v]=sum;
                    predecessor[v]=next;
                }
                else if (distances[v]==-1){
                    distances[v]=sum;
                    predecessor[v]=next;
                }
            }
        }
    }
    cout<<"path: ";
    for(int t=0; t<=end; t++){
        cout<<predecessor[t]<<" ";

    }
    cout<<"---end of shortest distance---"<<endl;
    int* d=distances;
    return d;

}

template <typename T>
int weighted_graph<T>:: Min(int* distance, bool* visited, int size){
    //cout<<"in min"<<endl;
    int minV=0;
    while(visited[minV]){
        minV++;
    }
    int min=distance[minV];
    for(int i=0; i<=size; i++){
        if(!visited[i] && distance[i]<=min && distance[i]!=-1){
            minV=i;
            min=distance[i];
        }
    }
    return minV;
}
#endif // WEIGHTED_GRAPH_H
