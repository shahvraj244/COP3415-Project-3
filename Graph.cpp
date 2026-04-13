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
    int i_src = get_vertex_index(src); //cehck for a vertex in graph 
    if (i_src == -1) {
        cout << "Origin airport not found." << endl;
        return;
    }
    clean_visited();
    //initialize distances, parent, and total_costs vectors
    vector<int> distances(vertices.size(), INT_MAX);
    vector<int> parent(vertices.size(), -1);
    vector<int> total_costs(vertices.size(), 0);
    distances[i_src] = 0;
//STORE EDGES BASED ON DISTANCE IN MIN HEAP 
    MinHeap<Edge> heap;
    heap.insert(Edge(i_src, i_src, 0, 0));

    while (!heap.isEmpty()) { //get edge with smallest distance 
        Edge e = heap.delete_min();
        int u = e.dest;
        if (vertices[u].getVisited()) 
            continue;
        vertices[u].setVisited(true);

        for (int j = 0; j < edges[u].size(); j++) { //check adjacent edges of u
            int v = edges[u][j].dest;
            int weight = edges[u][j].weight;
            int cost = edges[u][j].cost;

            if (!vertices[v].getVisited() && distances[u] + weight < distances[v]) {
                distances[v] = distances[u] + weight;
                parent[v] = u;
                total_costs[v] = total_costs[u] + cost;
                heap.insert(Edge(u, v, distances[v], 0));
            }
        }
    }
    cout << "Shortest paths from " << src.getData() << " to " << state << " state airports are:" << endl;
    bool found = false;

    for (int i = 0; i < vertices.size(); i++) {
        if (vertices[i].getData().find(state) != string::npos) {
            if (distances[i] != INT_MAX) {
                found = true;
                vector<T> path;
                for (int cur = i; cur != -1; cur = parent[cur]) {
                    path.push_back(vertices[cur].getData());
                }
                cout << "Shortest route to " << vertices[i].getData() << ": ";
                for (int p = path.size() - 1; p >= 0; p--) {
                    cout << path[p] << (p == 0 ? "" : " -> ");
                }
                cout << ". The length is " << distances[i] << ". The cost is " << total_costs[i] << "." << endl;
            }
        }
    }
    if (!found) {
        cout << "No paths found to " << state << " state airports." << endl;
    }
    clean_visited();
}

/*
4. Find the shortest path between the given origin airport and destination airport with a given
number of stops. The algorithm should provide the appropriate message if such path doesn’t exist.
*/
template<typename T>
void Graph<T>::short_path_stops(const Vertex<T>& src, const Vertex<T>& dest, int stops) {
    int i_src = get_vertex_index(src);
    int i_dest = get_vertex_index(dest);
    if(i_src == -1 || i_dest == -1) {//cehck for a vetex if it exists in graph 
        cout<<"Shortest route from "<<src.getData()<<" to "<<dest.getData()<<" with "<<stops<<" stops: None"<<endl;
        return;
    }
    int edges_needed = stops + 1; // n stops = n + 1 edges
    vector<vector<int>> dist(vertices.size(), vector<int>(edges_needed + 1, INT_MAX));
    vector<vector<int>> parent(vertices.size(), vector<int>(edges_needed + 1, -1));
    dist[i_src][0] = 0;
    //find shortest path for each edge count edges
    for(int e = 1; e <= edges_needed; e++) {
        for(int u = 0; u < vertices.size(); u++) {
            if(dist[u][e-1] == INT_MAX) continue;
            for(int j = 0; j < edges[u].size(); j++) {
                int v = edges[u][j].dest;
                int weight = edges[u][j].weight;
                if(dist[u][e-1] != INT_MAX && dist[u][e-1] + weight < dist[v][e]) {
                    dist[v][e] = dist[u][e-1] + weight;
                    parent[v][e] = u;
                }
            }
        }
    }
    cout<<"Shortest route from "<<src.getData()<<" to "<<dest.getData()<<" with "<<stops<<" stops: ";
    if(dist[i_dest][edges_needed] == INT_MAX) {
        cout<<"None"<<endl;
    } else {
        vector<T> path;
        int cur = i_dest, e = edges_needed;
        while(cur != -1 && e >= 0) {
            path.push_back(vertices[cur].getData());
            cur = parent[cur][e];
            e--;
        }
        for(int i = path.size() - 1; i >= 0; i--) {
            cout << path[i] << (i == 0 ? "" : " -> ");
        }
        int total_cost = 0;
        int temp_v = i_dest;
        for(int e = edges_needed; e > 0; e--) {
            int par = parent[temp_v][e];
            for(int j = 0; j < edges[par].size(); j++) {
                if(edges[par][j].dest == temp_v) {
                    total_cost += edges[par][j].cost;
                    break;
                }
            }
            temp_v = par;
        }
        cout<<". The lenght is "<<dist[i_dest][edges_needed]<<". The cost is "<<total_cost<<"."<<endl;
    }
}

/*
5. Count and display total direct flight connections to each airport. You should consider both
outbound and inbound flights. For instance, if you can directly fly to Tampa airport only from
Miami, Orlando, and Atlanta, the inbound count for Tampa airport would be 3. If you can directly
fly from Tampa airport only to New York, the outbound count for Tampa airport is 1. The total
number of direct flight connections for Tampa airport is 4. The list of airports should be sorted
based on the total direct flight connections count, starting with the airports having the highest
number of direct flight connections.
*/
template <typename T>
void Graph<T>::disp_connections_sort() {
    //TODO
}

/*
6. Create an undirected graph G_u from the original directed graph G using the following rules:
a. For each pair of vertices u and v, if there is only one directed edge(either (u,v) or (v,u))
between them, you keep that single edge with its corresponding cost as an undirected
weighted edge. You can ignore the distance on that edge.
b. For each pair of vertices u and v, if there are two directed edges (u,v) and (v, u) between
them, you keep the one with the minimum cost value as an undirected weighted edge. You
can ignore the distance on that edge.
*/
template <typename T>
void Graph<T>::cost_graph(){
    //TODO
}

/*
7.Generate a Minimal Spanning Tree utilizing Prim’s algorithm on G_u that you created in the
previous step. The algorithm will output both the content of the constructed MST and its total cost.
In this step, for each edge you need to consider the cost as weight to minimize the total cost. In the
event of a disconnected graph, the algorithm will appropriately notify that an MST cannot be
formed. Note: A connected graph is defined as one where there exists a path between every pair of
vertices.
*/
template <typename T>
void Graph<T>::prim_mst() {
    //TODO
}


/*
8.Generate a Minimal Spanning Tree using Kruskal’s algorithm on G_u that you created in the
previous step. The algorithm will output both the content of the constructed MST and its total cost.
In this step, for each edge you need to consider the cost as weight to minimize the total cost. If the
graph is disconnected the algorithm should provide minimum spanning forest consisting of a
minimum spanning tree for each connected component
*/
template<typenname T>
int Graph<T>::find_set(vector<int>& parent, int i) {
    //TODO
}

template<typename T>
void Graph<T>::union_set(vector<int>& parent, int i, int j) {
    //TODO
}

template <typename T>
void Graph<T>::kruskal_mst() {
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