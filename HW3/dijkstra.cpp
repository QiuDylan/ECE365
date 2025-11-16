// Dylan Qiu ME '27
// ECE365 Fall 2025 HW3 Dijkstra's Implementation

#include <iostream>
#include <string>
#include <ctime>
#include <climits>
#include "graph.h"

using namespace std;

void dijkstra(Graph &graph, const string &startId) {
    auto &nodes = graph.getNodes();
    hashTable *nodeMap = graph.getNodeMap();
    for (auto &node : nodes) {
        node.dist = INT_MAX;
        node.prev = nullptr;
        node.known = false;
    }
    Graph::Node *startNode = graph.getNode(startId);
    if (startNode == nullptr) {
        return;
    }
    
    startNode->dist = 0;
    // Create heap based on the number of input nodes
    heap pq(nodes.size());
    for (auto &node : nodes) {
        pq.insert(node.id, node.dist, &node);
    }
    
    // Main Dijkstra's algorithm loop
    while (true) {
        string minId;
        int minDist;
        void *pv;
        
        if (pq.deleteMin(&minId, &minDist, &pv) != 0) {
            break;
        }
        
        Graph::Node *v = static_cast<Graph::Node*>(pv);
        
        if (v->known || v->dist == INT_MAX) {
            continue;
        }
        
        v->known = true;
        
        // Update distances to adjacent nodes
        for (const auto &edge : v->adjList) {
            Graph::Node *w = static_cast<Graph::Node*>(nodeMap->getPointer(edge.destId));
            
            if (!w->known) {
                int newDist = v->dist + edge.cost;
                
                if (newDist < w->dist) {
                    w->dist = newDist;
                    w->prev = v;
                    pq.setKey(edge.destId, newDist);
                }
            }
        }
    }
}

int main() {
    Graph graph;
    string graphFile;
    cout << "Enter name of graph file: ";
    cin >> graphFile;
    if (!graph.loadFromFile(graphFile)) {
        cerr << "Error: Could not open file " << graphFile << endl;
        return 1;
    }
    
    string startVertex;
    while (true) {
        cout << "Enter name of starting vertex: ";
        cin >> startVertex;
        if (graph.vertexExists(startVertex)) {
            break;
        }
        cout << "Error: Vertex does not exist in graph. Please try again." << endl;
    }

    clock_t startTime = clock();
    dijkstra(graph, startVertex);
    clock_t endTime = clock();
    double timeTaken = static_cast<double>(endTime - startTime) / CLOCKS_PER_SEC;
    cout << "Total time (in seconds) to apply Dijkstra's algorithm: " << timeTaken << endl;
    string outputFile;
    cout << "Enter name of output file: ";
    cin >> outputFile;
    graph.writeOutput(outputFile, startVertex);
    
    return 0;
}