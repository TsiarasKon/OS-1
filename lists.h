#ifndef OS_1_MYLIST_H
#define OS_1_MYLIST_H

class EdgeList {

    class EdgeListnode {
    public:
        char *receivingNodeName;
        int weight;
        EdgeListnode *nextEdge;
        EdgeListnode(char *receivingNodeName, int weight);
        ~EdgeListnode();
    };

    int size;
    EdgeListnode *firstEdge;
public:
    EdgeList();
    ~EdgeList();
};

class NodeList {

    class NodeListnode {
    public:
        char *nodeName;
        EdgeList *edges;
        NodeListnode *nextNode;
        NodeListnode(char *nodeName, EdgeList *edges);
        ~NodeListnode();
    };

    int size;
    NodeListnode *firstNode;
public:
    NodeList();
    ~NodeList();
};

#endif
