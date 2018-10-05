#ifndef OS_1_MYLIST_H
#define OS_1_MYLIST_H

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
    void print() const;
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
    void print() const;
    bool insertInOrder(char *nodeName);
    bool deleteNode(char *nodeName);
};

#endif
