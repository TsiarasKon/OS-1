#ifndef OS_1_MYLIST_H
#define OS_1_MYLIST_H

class EdgeList {

    class EdgeListnode {
    public:
        char *receivingNodeName;
        int weight;
        EdgeListnode *nextEdge;
        EdgeListnode(char *receivingNodeName, int weight, EdgeListnode *nextEdge);
        ~EdgeListnode();
    };

    int size;
    EdgeListnode *firstEdge;
public:
    EdgeList();
    ~EdgeList();

    void print() const;
};

class NodeList {

    class NodeListnode {
    public:
        char *nodeName;
        EdgeList *edges;
        NodeListnode *nextNode;
        NodeListnode(char *nodeName, NodeListnode *nextNode);
        ~NodeListnode();
    };

    int size;
    NodeListnode *firstNode;
public:
    NodeList();
    ~NodeList();
    bool insertInOrder(char *nodeName);
    bool deleteNode(char *nodeName);

    void print() const;
};

#endif
