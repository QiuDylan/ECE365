#include "graph.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <climits>

Graph::Graph() {
    nodeMap = new hashTable(1000000);
}

Graph::~Graph() {
    delete nodeMap;
}

// Get or create a node 
Graph::Node* Graph::getOrCreateNode(const std::string &id) {
    Node *node = static_cast<Node*>(nodeMap->getPointer(id));
    if (node == nullptr) {
        nodes.push_back(Node(id));
        node = &nodes.back();
        nodeMap->insert(id, node);
    }
    
    return node;
}

// Load graph from file
bool Graph::loadFromFile(const std::string &filename) {
    std::ifstream infile(filename);
    if (!infile) {
        return false;
    }
    
    std::string line;
    while (std::getline(infile, line)) {
        std::istringstream iss(line);
        std::string src, dest;
        int cost;
        
        if (iss >> src >> dest >> cost) {
            Node *srcNode = getOrCreateNode(src);
            getOrCreateNode(dest); 
            srcNode->adjList.push_back(Node::Edge(dest, cost));
        }
    }
    
    infile.close();
    return true;
}

// Check if a vertex exists
bool Graph::vertexExists(const std::string &id) {
    return nodeMap->contains(id);
}

// Get a node pointer by ID
Graph::Node* Graph::getNode(const std::string &id) {
    return static_cast<Node*>(nodeMap->getPointer(id));
}

// Get the nodes list
std::list<Graph::Node>& Graph::getNodes() {
    return nodes;
}

// Get hash table (for node lookups in dijkstra)
hashTable* Graph::getNodeMap() {
    return nodeMap;
}

// Write output to file
void Graph::writeOutput(const std::string &filename, const std::string &startId) {
    std::ofstream outfile(filename);
    if (!outfile) {
        std::cerr << "Error: Could not open output file." << std::endl;
        return;
    }
    
    // Write results for each node in order
    for (const auto &node : nodes) {
        outfile << node.id << ": ";
        
        if (node.dist == INT_MAX) {
            outfile << "NO PATH" << std::endl;
        } else {
            outfile << node.dist << " [";
            
            std::list<std::string> path;
            const Node *current = &node;
            while (current != nullptr) {
                path.push_front(current->id);
                current = current->prev;
            }
            bool first = true;
            for (const auto &id : path) {
                if (!first) {
                    outfile << ", ";
                }
                outfile << id;
                first = false;
            }
            
            outfile << "]" << std::endl;
        }
    }
    
    outfile.close();
}