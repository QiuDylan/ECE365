#ifndef _GRAPH_H
#define _GRAPH_H

#include <string>
#include <list>
#include "hash.h"
#include "heap.h"

class Graph {
private:
    // Nodes and Edges
    class Node {
    public:
        class Edge {
        public:
            std::string destId;
            int cost;
            
            Edge(const std::string &dest, int c) : destId(dest), cost(c) {}
        };
        
        std::string id;
        std::list<Edge> adjList;
        int dist;
        Node *prev;
        bool known;
        
        Node(const std::string &vertexId) 
            : id(vertexId), dist(INT_MAX), prev(nullptr), known(false) {}
    };
    std::list<Node> nodes;
    hashTable *nodeMap;
    Node* getOrCreateNode(const std::string &id);
public:
    Graph();
    ~Graph();
    
    // Load graph from file
    bool loadFromFile(const std::string &filename);
    
    // Check if a vertex exists
    bool vertexExists(const std::string &id);
    
    // Write results to output file
    void writeOutput(const std::string &filename, const std::string &startId);

    // Get a node pointer by ID (for external use)
    Node* getNode(const std::string &id);
    
    // Get the nodes list (for external iteration)
    std::list<Node>& getNodes();
    
    // Get the node map (for lookups in dijkstra)
    hashTable* getNodeMap();
    friend void dijkstra(Graph &graph, const std::string &startId);
};

#endif // _GRAPH_H