#ifndef OS_1_MYLIST_H
#define OS_1_MYLIST_H

#include <fstream>

#define INITIAL_BUCKETS_NUM 16
#define HASHING_LOAD_FACTOR 0.7

// forward declarations
class Graph;
class Node;

class Edge {
    Node *receivingNode;
    int weight;
    bool visited;
    Edge *nextEdge;
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
    Edge *firstEdge;
public:
    EdgeList();
    ~EdgeList();
    Edge *getFirstEdge() const;
    void print(std::ostream& outstream) const;
    void insertEdge(Node *toNode, int weight);
    int deleteAllEdges(char *toNodeName);
    int deleteEdgesWithWeight(char *toNodeName, int weight);
    int modifyEdge(char *toNodeName, int weight, int nweight);
    void printTransactionsTo(char *fromNodeName, char *toNodeName, bool *printed) const;
};

class EdgeStack {
    Node *startingNode;
    Edge *headEdge;
public:
    explicit EdgeStack(Node *startingNode);
    ~EdgeStack();
    Node *getStartingNode() const;
    Edge *getHeadEdge() const;
    void push(Node *toNode, int weight);
    void deleteLast();
    EdgeStack *getReverseStack() const;
    void printCycle() const;
};

class Node {
    char *nodeName;
    EdgeList *edges;
    bool visited;
    Node *nextNode;
public:
    Node(char *nodeName, Node *nextNode);
    ~Node();
    char *getNodeName() const;
    EdgeList *getEdges() const;
    Node *getNextNode() const;
    void setNextNode(Node *nextNode);
    bool getVisited() const;
    void setVisited(bool visited);
    void simpleCycleCheck(EdgeStack *visited, bool *foundCycle);
    void cyclicTransactionCheck(EdgeStack *visited, int weight, bool *foundCycle);
    void traceflowCheck(EdgeStack *visited, Node *toNode, int len, bool *foundTrace);
};

class Graph {
    int nodesnum;
    int bucketsnum;
    Node **nodeTable;
    void insertNodeReference(Node *node);
    void rehashNodeTable();
public:
    explicit Graph(int bucketsnum);
    ~Graph();
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
