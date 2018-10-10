#ifndef OS_1_MYLIST_H
#define OS_1_MYLIST_H

#include <fstream>

// forward declarations
class Graph;
class GraphNode;

class EdgeListnode {
    GraphNode *receivingNode;
    int weight;
    EdgeListnode *nextEdge;
public:
    EdgeListnode(GraphNode *receivingNode, int weight, EdgeListnode *nextEdge);
    ~EdgeListnode();
    GraphNode *getReceivingNode() const;
    int getWeight() const;
    void setWeight(int weight);
    EdgeListnode *getNextEdge() const;
    void setNextEdge(EdgeListnode *nextEdge);
};

class EdgeList {
protected:
    EdgeListnode *firstEdge;
public:
    EdgeList();
    ~EdgeList();
    EdgeListnode *getFirstEdge() const;
    void setFirstEdge(EdgeListnode *firstEdge);
    void print(std::ostream& outstream) const;
    void insertEdge(GraphNode *toNode, int weight);
    int deleteAllEdges(char *toNodeName);
    int deleteEdgesWithWeight(char *toNodeName, int weight);
    int modifyEdge(char *toNodeName, int weight, int nweight);
    bool printTransactionsTo(char *fromNodeName, char *toNodeName) const;
};

class Cycle : public EdgeList {
    GraphNode *startingNode;
    EdgeListnode *lastEdge;
public:
    explicit Cycle(GraphNode *startingNode);
    GraphNode *getStartingNode() const;
    void insertUnordered(GraphNode *toNode, int weight);
    void deleteLast();
    int nodeExists(GraphNode *node) const;
    bool edgeExists(EdgeListnode *edge) const;
    void printCycle() const;
};

class GraphNode {
    char *nodeName;
    EdgeList *edges;
    GraphNode *nextNode;
public:
    GraphNode(char *nodeName, GraphNode *nextNode);
    ~GraphNode();
    char *getNodeName() const;
    EdgeList *getEdges() const;
    GraphNode *getNextNode() const;
    void setNextNode(GraphNode *nextNode);
    bool simpleCycleCheck(Cycle *visited);
    bool cyclicTransactionCheck(Cycle *visited, int weight);
};

class Graph {
    GraphNode *firstNode;
public:
    Graph();
    ~Graph();
    GraphNode *getFirstNode() const;
    void setFirstNode(GraphNode *firstNode);
    void print(std::ostream& outstream) const;
    GraphNode *getNodeByName(char *nodeName) const;
    GraphNode *insertNode(char *nodeName);
    void insertEdge(char *fromNodeName, char *toNodeName, int weight);
    bool deleteNode(char *nodeName);
    int deleteAllEdges(char *fromName, char *toNodeName);
    int deleteEdgesWithWeight(char *fromName, char *toNodeName, int weight);
    int modifyEdge(char *fromName, char *toNodeName, int weight, int nweight);
    void printReceiving(char *nodeName) const;
    void circlefind(char *nodeName) const;
    void findcircles(char *nodeName, int k) const;
};

#endif
