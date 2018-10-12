#ifndef OS_1_MYLIST_H
#define OS_1_MYLIST_H

#include <fstream>

// forward declarations
class Graph;
class Node;

class Edge {
    Node *receivingNode;
    int weight;
    Edge *nextEdge;
    bool visited;
public:
    Edge(Node *receivingNode, int weight, Edge *nextEdge);
    ~Edge();
    Node *getReceivingNode() const;
    int getWeight() const;
    void setWeight(int weight);
    Edge *getNextEdge() const;
    void setNextEdge(Edge *nextEdge);
    bool getVisited() const;
    void setVisited(bool visited);
};

class EdgeList {
protected:
    Edge *firstEdge;
public:
    EdgeList();
    ~EdgeList();
    Edge *getFirstEdge() const;
    void setFirstEdge(Edge *firstEdge);
    void print(std::ostream& outstream) const;
    void insertEdge(Node *toNode, int weight);
    int deleteAllEdges(char *toNodeName);
    int deleteEdgesWithWeight(char *toNodeName, int weight);
    int modifyEdge(char *toNodeName, int weight, int nweight);
    void resetAllVisited();
    void printTransactionsTo(char *fromNodeName, char *toNodeName, bool *printed) const;
};

class Cycle : public EdgeList {
    Node *startingNode;
    Edge *lastEdge;
public:
    explicit Cycle(Node *startingNode);
    Node *getStartingNode() const;
    Edge *getLastEdge() const;
    void insertUnordered(Node *toNode, int weight);
    void deleteLast();
    int nodeExists(Node *node) const;
    void printCycle() const;
};

class Node {
    char *nodeName;
    EdgeList *edges;
    Node *nextNode;
public:
    Node(char *nodeName, Node *nextNode);
    ~Node();
    char *getNodeName() const;
    EdgeList *getEdges() const;
    Node *getNextNode() const;
    void setNextNode(Node *nextNode);
    bool simpleCycleCheck(Cycle *visited);
    bool cyclicTransactionCheck(Cycle *visited, int weight);
    bool traceflowCheck(Cycle *visited, Node *toNode, int len);
};

class Graph {
    Node *firstNode;
public:
    Graph();
    ~Graph();
    Node *getFirstNode() const;
    void setFirstNode(Node *firstNode);
    void print(std::ostream& outstream) const;
    Node *getNodeByName(char *nodeName) const;
    Node *insertNode(char *nodeName);
    void insertEdge(char *fromNodeName, char *toNodeName, int weight);
    bool deleteNode(char *nodeName);
    int deleteAllEdges(char *fromName, char *toNodeName);
    int deleteEdgesWithWeight(char *fromName, char *toNodeName, int weight);
    int modifyEdge(char *fromName, char *toNodeName, int weight, int nweight);
    void printReceiving(char *nodeName) const;
    void circlefind(char *nodeName) const;
    void findcircles(char *nodeName, int k) const;
    void traceflow(char *fromNodeName, char *toNodeName, int len) const;
};

#endif
