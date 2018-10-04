#include <cstring>
#include "lists.h"

// EdgeListNode methods:
EdgeList::EdgeListnode::EdgeListnode(char *receivingNodeName, int weight) : receivingNodeName(receivingNodeName), weight(weight), nextEdge(NULL) {}

EdgeList::EdgeListnode::~EdgeListnode() {
    delete[] receivingNodeName;
}


// EdgeList methods:
EdgeList::EdgeList() : size(0), firstEdge(NULL) {}

EdgeList::~EdgeList() {
    EdgeListnode *current = firstEdge, *next;
    while (current != NULL) {
        next = current->nextEdge;
        delete current;
        current = next;
    }
}


// NodeListNode methods:
NodeList::NodeListnode::NodeListnode(char *nodeName, EdgeList *edges) : nodeName(nodeName), edges(edges), nextNode(NULL) {}

NodeList::NodeListnode::~NodeListnode() {
    delete[] nodeName;
}


// NodeList methods:
NodeList::NodeList() : size(0), firstNode(NULL) {}

NodeList::~NodeList() {
    NodeListnode *current = firstNode, *next;
    while (current != NULL) {
        next = current->nextNode;
        delete current;
        current = next;
    }
}

