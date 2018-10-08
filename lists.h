#ifndef OS_1_MYLIST_H
#define OS_1_MYLIST_H

#include <fstream>

class NodeListnode;         // forward declaration

class EdgeListnode {
    NodeListnode *receivingNode;
    int weight;
    EdgeListnode *nextEdge;
public:
    EdgeListnode(NodeListnode *receivingNode, int weight, EdgeListnode *nextEdge);
    ~EdgeListnode();
    NodeListnode *getReceivingNode() const;
    int getWeight() const;
    void setWeight(int weight);
    EdgeListnode *getNextEdge() const;
    void setNextEdge(EdgeListnode *nextEdge);
};

class EdgeList {
    EdgeListnode *firstEdge;
public:
    EdgeList();
    ~EdgeList();
    EdgeListnode *getFirstEdge() const;
    void setFirstEdge(EdgeListnode *firstEdge);
    void print(std::ostream& outstream) const;
    void insertEdge(NodeListnode *toNode, int weight);
    int deleteAllEdges(char *toNodeName);
    int deleteEdgesWithWeight(char *toNodeName, int weight);
    int modifyEdge(char *toNodeName, int weight, int nweight);
    bool printTransactionsTo(char *fromNodeName, char *toNodeName);
};

class NodeListnode {
    char *nodeName;
    EdgeList *edges;
    NodeListnode *nextNode;
public:
    NodeListnode(char *nodeName, NodeListnode *nextNode);
    ~NodeListnode();
    char *getNodeName() const;
    EdgeList *getEdges() const;
    NodeListnode *getNextNode() const;
    void setNextNode(NodeListnode *nextNode);
};

class NodeList {
    NodeListnode *firstNode;
public:
    NodeList();
    ~NodeList();
    NodeListnode *getFirstNode() const;
    void setFirstNode(NodeListnode *firstNode);
    void print(std::ostream& outstream) const;
    NodeListnode *getNodeByName(char *nodeName);
    NodeListnode *insertNode(char *nodeName);
    void insertEdge(char *fromNodeName, char *toNodeName, int weight);
    bool deleteNode(char *nodeName);
    int deleteAllEdges(char *fromName, char *toNodeName);
    int deleteEdgesWithWeight(char *fromName, char *toNodeName, int weight);
    int modifyEdge(char *fromName, char *toNodeName, int weight, int nweight);
    void printReceiving(char *nodeName);
};

#endif
