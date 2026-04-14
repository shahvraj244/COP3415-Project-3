#include <iostream>
#include <string>
#include "Graph.h"
#include "Vertex.h"
#include "Graph.cpp"
#include "MinHeap.cpp"

using namespace std;

int main() {
    try {
        cout << "--- Loading Graph from CSV ---" << endl;
        Graph<string> airportGraph("airports.csv");
        cout << "Graph loaded successfully.\n" << endl;
        
        Vertex<string> vALB("ALB");
        Vertex<string> vBOS("BOS");
        Vertex<string> vDTW("DTW");
        Vertex<string> vABE("ABE");

        cout << "--- Testing Task 2: Dijkstra Shortest Path ---" << endl;
        airportGraph.dijkstra_shortest_path(vALB, vBOS);
        airportGraph.dijkstra_shortest_path(vABE, vDTW);
        cout << endl;

        cout << "--- Testing Task 3: Shortest Paths to State (MI) ---" << endl;
        airportGraph.short_paths_state(vALB, "DTW"); // Searching for DTW in Michigan
        cout << endl;

        cout << "--- Testing Task 4: Shortest Path with Stops ---" << endl;
        cout << "Testing ALB to BOS with 0 stops:" << endl;
        airportGraph.short_path_stops(vALB, vBOS, 0);
        
        cout << "\nTesting ALB to DTW with 1 stop:" << endl;
        airportGraph.short_path_stops(vALB, vDTW, 1);
        cout << endl;

    } catch (const string& e) {
        cerr << "Error: " << e << endl;
    } catch (const exception& e) {
        cerr << "Standard Exception: " << e.what() << endl;
    }

    return 0;
}