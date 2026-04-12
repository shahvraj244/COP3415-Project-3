#include "Graph.h"
#include "MinHeap.h"
#include <vector>
#include <iostream>
#include <string>

using namespace std;

#define INT_MAX 1000000

template <typename T>
void Graph<T>::insert_vertex(const Vertex<T>& ver) {
    if(get_vertex_index(ver) == -1) {
        vertices.push_back(ver); 
        vector<Edge> tmp; 
        edges.push_back(tmp); 
    }
}

template <typename T> 
int Graph<T>::get_vertex_index(const Vertex<T>& ver) {
    for(int i = 0; i < vertices.size(); i++) {
        if(vertices[i].getData() == ver.getData()) {
            return i; 
        }
    }
    return -1; 
}

template <typename T> 
void Graph<T>::add_edge(const Vertex<T>& ver1, const Vertex<T>& ver2, int weight) {
    int i1 = get_vertex_index(ver1);
    int i2 = get_vertex_index(ver2);
    if(i1 == -1 || i2 == -1) {
        throw string("Add edge: incorrect vertices\n"); 
    }
    Edge v(i1, i2, weight);
    Edge v2(i2, i1, weight);
    edges[i1].push_back(v);
    if(i1 != i2) {
        edges[i2].push_back(v2); 
    }
}

template <typename T>
void Graph<T>::print() const {
    for (int i = 0; i < vertices.size(); i++) {
        cout << "{ " << vertices[i].getData() << ": ";
        for(int j = 0; j < edges[i].size(); j++) {
            cout << '{' << vertices[edges[i][j].dest].getData() << ", ";
            cout << edges[i][j].weight << "} ";
        }
        cout << " }\n";
    }
}

template <typename T> 
void Graph<T>::DFS(Vertex<T>& ver) {
    clean_visited();
    DFS_helper(ver);
    clean_visited();
}

template <typename T>
void print_queue(std::queue<Vertex<T>> q) {
  while (!q.empty())
  {
    cout << q.front().getData() << " ";
    q.pop();
  }
  cout << endl;
}

template <typename T> 
void Graph<T>::BFS(Vertex<T>& ver) {
    clean_visited();
    int start_index = get_vertex_index(ver); 
    if(start_index == -1) {
        throw string("BFS: incorrect vertex\n"); 
    }
    vector<int> q; 
    int head = 0; 

    q.push_back(start_index);
    vertices[start_index].setVisited(true);

    while(head < q.size()) {
        int i = q[head++]; 
        cout<<vertices[i].getData() << " "; 

        for(int j = 0; j < edges[i].size(); j++) {
            int adjacent_ver = edges[i][j].dest;
            if(vertices[adjacent_ver].getVisited() == false) {
                vertices[adjacent_ver].setVisited(true); 
                q.push_back(adjacent_ver); 
            }
        }
    }
    clean_visited();
}


template <typename T> 
void Graph<T>::DFS_helper(Vertex<T>& ver) {
    int i = get_vertex_index(ver); 
    if(i == -1) 
        return;
    vertices[i].setVisited(true);
    cout << vertices[i].getData() << " ";
    for(int j = 0; j < edges[i].size(); j++) {
        int adjacent_ver = edges[i][j].dest;
        if(vertices[adjacent_ver].getVisited() == false) {
            DFS_helper(vertices[adjacent_ver]); 
        }
    }
}

template <typename T>
void Graph<T>::clean_visited() {
    for(int i = 0; i < vertices.size(); i++) {
        vertices[i].setVisited(false); 
    }
}


template<typename T>
int dijkstra_shortest_path(const Vertex<T>& src, const Vertex<T>& dest) {
    int i_src = get_vertex_index(src);
    int i_dest = get_vertex_index(dest);
    if (i_src == -1 || i_dest == -1) {
        throw std::string("Shortest path: incorrect vertices"); 
    }

    clean_visited();
    std::vector<int> distances(vertices.size(), INT_MAX);
    distances[i_src] = 0;

    MinHeap<Edge> heap;
    // For Dijkstra, the weight in the heap represents the total distance from source
    heap.insert(Edge(i_src, i_src, 0));

    // Use isEmpty() check instead of try-catch
    while (!heap.isEmpty()) {
        Edge e = heap.delete_min();

        int u = e.dest;
        if (vertices[u].getVisited()) {
            continue;
        }
        vertices[u].setVisited(true);

        // Optimization: stop if we reached the destination
        if (u == i_dest) {
            break;
        }

        // Use standard for-loop instead of range-based auto
        for (int j = 0; j < (int)edges[u].size(); j++) {
            int v = edges[u][j].dest;
            int weight = edges[u][j].weight;
            
            if (!vertices[v].getVisited() && (distances[u] + weight < distances[v])) {
                distances[v] = distances[u] + weight;
                heap.insert(Edge(u, v, distances[v]));
            }
        }
    }
    clean_visited();
    return distances[i_dest];
}

//Function to implement 
/*
        //Undirected Graph G_u 
        Graph<T> cost_graph; 
        //helper functions for kruskal 
        int find_set(vector<int>& parent, int i); 
        void union_set(vector<int>& parent, int i, int j);
        void short_paths_state(const Vertex<T>& src, const string& state);
        //short path with stops 
        void short_path_stops(const Vertex<T>& src, const Vertex<T>& dest, int stops);
        //Disp connections 
        void disp_connections_sort(); 
        //prim's minimum span tree
        void prim_mst(); 
        //kruskal's minimum span tree
        void kruskal_mst(); 
        //print function
        void print() const; 
        //Breadth first search
        void BFS(Vertex<T>& ver); 
        //Depth first search 
        void DFS(Vertex<T>& ver); 
*/