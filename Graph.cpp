#include "Graph.h"
#include "MinHeap.h"
#include <vector>
#include <iostream>
#include <fstream>
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

/*
1. Read the information from the dataset (a csv file) and create a weighted directed graph G. Note
that you need to consider two weights for each edge. One is the Distance and the other is Cost.
*/
template<typename T>
Graph<T>::Graph(const string& filename) {
    ifstream file(filename); 
    string line; 

    if(!file.is_open()) {
        throw string("Could not open file\n"); 
    }
    getline(file,line); 
    while(getline(file,line)) {
        if(line.empty()) {
            continue;
        }
        string fields[6];
        size_t start = 0; 

        for(int i = 0; i < 6; i++) {
            if(start < line.length() && line[start] == '"') {
                start++;
                size_t end = line.find('"', start); 
                fields[i] = line.substr(start, end - start); 
                start = line.find(',', end) + 1; 
            } else {
                size_t next = line.find(',', start);
                fields[i] = line.substr(start, next - start);
                start = next + 1;
            }
        }
        Vertex<T> v1(fields[0]);
        Vertex<T> v2(fields[1]); 
        int distance = stoi(fields[4]);
        int cost = stoi(fields[5]);
        
        insert_vertex(v1);
        insert_vertex(v2);

        int i1 = get_vertex_index(v1);
        int i2 = get_vertex_index(v2);
        if(i1 != -1 && i2 != -1) {
            edges[i1].push_back(Edge(i1, i2, distance, cost));
        }
    }
    file.close();
}

/*
2. Find the shortest path between the given origin airport and destination airport. The algorithm
should output both the path and the length of the path. The algorithm should provide the
appropriate message if such path doesn't exist.
*/
template<typename T>
int Graph<T>::dijkstra_shortest_path(const Vertex<T>& src, const Vertex<T>& dest) {
    int i_src = get_vertex_index(src);
    int i_dest = get_vertex_index(dest);
    if(i_src == -1 || i_dest == -1) {
        throw string("Shortest path: incorrect vertices\n");
    }
    clean_visited();
    vector<int> distances(vertices.size(), INT_MAX);
    vector<int> parent(vertices.size(), -1);
    distances[i_src] = 0;

    MinHeap<Edge> heap;
    heap.insert(Edge(i_src, i_src, 0, 0));

    while(!heap.isEmpty()) {
        Edge e = heap.delete_min();
        int u = e.dest;
        if(vertices[u].getVisited())
            continue;
        vertices[u].setVisited(true);
        if(u == i_dest)
            break;
        for(int j = 0; j < edges[u].size(); j++) {
            int v = edges[u][j].dest;
            int weight = edges[u][j].weight;
            if(!vertices[v].getVisited() && distances[u] != INT_MAX && distances[u] + weight < distances[v]) {
                distances[v] = distances[u] + weight;
                parent[v] = u;
                heap.insert(Edge(u, v, distances[v], 0));
            }
        }
    }

    cout << "Shortest route from " << src.getData() << " to " << dest.getData() << ": ";
    if(distances[i_dest] == INT_MAX) {
        cout << "None" << endl;
    } else {
        // Backtrack path
        vector<T> path;
        for(int cur = i_dest; cur != -1; cur = parent[cur]) {
            path.push_back(vertices[cur].getData());
        }
        for(int i = path.size() - 1; i >= 0; i--) {
            cout << path[i] << (i == 0 ? "" : " -> ");
        }
        int total_cost = 0;
        for(int cur = i_dest; parent[cur] != -1; cur = parent[cur]) {
            int par = parent[cur];
            for(int j = 0; j < edges[par].size(); j++) {
                if(edges[par][j].dest == cur) {
                    total_cost += edges[par][j].cost;
                    break;
                }
            }
        }

        cout << ". The length is " << distances[i_dest] << ". The cost is " << total_cost << "." << endl;
    }
    clean_visited();
    return distances[i_dest];
}

/*
3.Find all shortest paths between the given origin airport and all the airports in the destination state.
The algorithm should output all the paths and their lengths. The algorithm should provide the
appropriate message if no such paths exist
*/
template<typename T>
void Graph<T>::short_paths_state(const Vertex<T>& src, const string& state) {
    cout<<"Shortest paths from "<<src.getData()<<" to "<<state<<" state airports are:"<<endl;
    bool found = false; //flag to check if any paths were found
    for(int i = 0; i < vertices.size(); i++) { //iterate through all vertices to find those that match the state
        if(vertices[i].getData().find(state) != string::npos) {
            int path_length = dijkstra_shortest_path(src, vertices[i]);
            if(path_length != INT_MAX) {
                found = true; 
            }
        }
    }
    if(!found) {
        cout<<"No paths found to "<<state<<" state airports."<<endl;
    }
}

/*
4. Find the shortest path between the given origin airport and destination airport with a given
number of stops. The algorithm should provide the appropriate message if such path doesn’t exist.
*/
template<typename T>
void Graph<T>::short_path_stops(const Vertex<T>& src, const Vertex<T>& dest, int stops) {
    //TODO
}

//Function to implement 
/*
        //read from csv file 
        Graph(const string& filename);
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
*/