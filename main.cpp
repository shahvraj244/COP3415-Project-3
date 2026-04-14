#include <iostream>
#include <string>
#include "Graph.h"
#include "Graph.cpp"
#include "MinHeap.h"
#include "MinHeap.cpp"

using namespace std;


int main() {
    cout<<"---------------------------------------------------------"<<endl;
    cout<<"Airport Connectivity and Flight Route Optimization System"<<endl;
    cout<<"---------------------------------------------------------"<<endl;
    Graph<string> G;
    try {
        G = Graph<string>("airports.csv");
        cout << "File Loaded successfully"<<endl;
    } catch(const string& e) {
        cout << "Error: " << e << endl;
        return 1; 
    }
    
    cout<<"---------------------------------------------------------------------------"<<endl;
    cout<<"Task 2: Shortest Path between origin airport and destination airport"<<endl;
    cout<<"---------------------------------------------------------------------------"<<endl;
    
    cout << "IAD -> MIA: " << G.dijkstra_shortest_path(Vertex<string>("IAD"), Vertex<string>("MIA")) << endl;
    cout << "PIT -> ACT: " << G.dijkstra_shortest_path(Vertex<string>("PIT"), Vertex<string>("ACT")) << endl;
    cout << endl;
    
    
    cout<<"---------------------------------------------------------"<<endl;
    cout<<"Task 3: Shortest Path to all airports in a State"<<endl;
    cout<<"---------------------------------------------------------"<<endl;
    
    G.short_paths_state(Vertex<string>("ATL"), "FL");
    cout << endl;
    G.short_paths_state(Vertex<string>("ORD"), "TX");
    cout << endl;
    G.short_paths_state(Vertex<string>("AUS"), "NJ");
    cout << endl;
    
    cout<<"---------------------------------------------------------"<<endl;
    cout<<"Task 4: Shortest Path with a fixed number of stops"<<endl;
    cout<<"---------------------------------------------------------"<<endl;
    
    G.short_path_stops(Vertex<string>("ATL"), Vertex<string>("TPA"), 2);
    G.short_path_stops(Vertex<string>("ORD"), Vertex<string>("MIA"), 1);
    G.short_path_stops(Vertex<string>("PIT"), Vertex<string>("ACT"), 2);
    cout << endl;
    
    cout<<"---------------------------------------------------------"<<endl;
    cout<<"Task 5: Airport Connections Counts"<<endl;
    cout<<"---------------------------------------------------------"<<endl;
    
    G.disp_connections_sort(); 
    cout<<endl; 
    
    cout<<"---------------------------------------------------------"<<endl;
    cout<<"Task 6: Undirected Cost Graph G_u"<<endl;
    cout<<"---------------------------------------------------------"<<endl;
    
    Graph<string> G_u = G.cost_graph(); 
    cout<<"G_u built successfully"<<endl;

    cout<<"---------------------------------------------------------"<<endl;
    cout<<"Task 7: Prim's Minimal Spanning Tree"<<endl;
    cout<<"---------------------------------------------------------"<<endl;
    
    G_u.prim_mst(); 
    cout<<endl; 
    
    /*
    cout<<"---------------------------------------------------------"<<endl;
    cout<<"Task 8: Kruskal's Minimal Spanning Tree"<<endl;
    cout<<"---------------------------------------------------------"<<endl;    
    
    G_u.kruskal.mst();
    cout<<endl;
    */
    
    return 0; 
}