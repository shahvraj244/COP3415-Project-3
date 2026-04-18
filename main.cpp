#include <iostream>
#include <string>
#include "Graph.h"
#include "Vertex.h"
#include "Graph.cpp"
#include "MinHeap.cpp"

using namespace std;

int main() {
    cout << "--- Graph-Based Airport Connectivity and Flight Route Optimization System ---" << endl;
    
    //Call airports.csv file
    Graph<string> airportGraph("airports.csv");
    cout << endl;

    //Vertex airports test
    Vertex<string> vATL("ATL");
    Vertex<string> vIAD("IAD");
    Vertex<string> vMIA("MIA");
    Vertex<string> vORD("ORD");
    Vertex<string> vPIT("PIT");
    Vertex<string> vACT("ACT");
    Vertex<string> vTPA("TPA");
    Vertex<string> vMCO("MCO");
    Vertex<string> vJFK("JFK");

    //Task 2: Dijkstra Shortest Path
    cout << "--- Task 2: Dijkstra Shortest Path ---" << endl;
    cout << "Shortest route from IAD to MIA: ";
    airportGraph.dijkstra_shortest_path(vIAD, vMIA);

    cout << "Shortest route from PIT to ACT: ";
    airportGraph.dijkstra_shortest_path(vPIT, vACT);
    cout << endl;

    //Task 3: Shortest Paths to State
    cout << "--- Task 3: Shortest Paths to State ---" << endl;
    cout << "Shortest paths from ATL to FL state airports are:" << endl;
    airportGraph.short_paths_state(vATL, "FL");
    cout << endl;

    //Task 4: Shortest Path with Stops
    cout << "--- Task 4: Shortest Path with Stops ---" << endl;
    cout << "Shortest route from IAD to MIA with 3 stops: ";
    airportGraph.short_path_stops(vIAD, vMIA, 3);

    cout << "Shortest route from PIT to ACT with 2 stops: ";
    airportGraph.short_path_stops(vPIT, vACT, 2);
    cout << endl;

    //Task 5: Direct Flight Connection
    cout << "--- Task 5: Direct Flight Connection ---" << endl;
    airportGraph.disp_connections_sort();
    cout << endl;

    //Task 6: Prim's Minimum Spanning Tree
    airportGraph.cost_graph(); 
    cout << "--- Task 6: Prim's Minimum Spanning Tree ---" << endl;
    airportGraph.prim_mst();
    cout << endl;

    //Task 7: Kruskal's Minimum Spanning Tree
    cout << "--- Task 7: Kruskal's Minimum Spanning Tree ---" << endl;
    airportGraph.kruskal_mst();
    cout << endl;

    return 0;
}