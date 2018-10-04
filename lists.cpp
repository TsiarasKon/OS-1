#include <iostream>
#include <cstring>
#include "lists.h"
#include "util.h"

using namespace std;

// EdgeListNode methods:
EdgeList::EdgeListnode::EdgeListnode(char *receivingNodeName, int weight, EdgeListnode *nextEdge) :
        weight(weight), nextEdge(nextEdge) {
    try {
        this->receivingNodeName = new char[strlen(receivingNodeName) + 1];
        strcpy(this->receivingNodeName, receivingNodeName);
    } catch (bad_alloc& e) {
        cerr << __func__ << ": " << e.what() << endl;
        delete[] this->receivingNodeName;
        throw;
    }
}

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

void EdgeList::print() const {
    EdgeListnode *current = firstEdge;
    while (current != NULL) {
        cout << " >" << current->receivingNodeName << "(" << current->weight << ")";
    }
    cout << " >/" << endl;
}


// NodeListNode methods:
NodeList::NodeListnode::NodeListnode(char *nodeName, NodeListnode *nextNode) :
        nextNode(nextNode) {
    try {
        this->nodeName = new char[strlen(nodeName) + 1];
        strcpy(this->nodeName, nodeName);
        edges = new EdgeList();
    } catch (bad_alloc& e) {
        cerr << __func__ << ": " << e.what() << endl;
        delete[] this->nodeName;
        throw;
    }
}

NodeList::NodeListnode::~NodeListnode() {
    delete edges;
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

bool NodeList::insertInOrder(char *nodeName) {
    try {
        NodeListnode *current = firstNode, *prev = firstNode;
        while (current != NULL && strcmp(current->nodeName, nodeName) < 0) {
            prev = current;
            current = current->nextNode;
        }
        if (current == firstNode && (current == NULL || strcmp(current->nodeName, nodeName) != 0)) {     // insert at start
            firstNode = new NodeListnode(nodeName, firstNode);
            return true;
        }
        if (current != NULL) {
            if (strcmp(current->nodeName, nodeName) == 0) {
//                cout << "Warning: Attempted to insert node with a name that already exists!" << endl;
                return false;
            } else {        // just surpassed where the node would have been found, if it existed
                prev->nextNode = new NodeListnode(nodeName, current);
                return true;
            }
        } else {    // reached the end of the list
            prev->nextNode = new NodeListnode(nodeName, NULL);
            return true;
        }
    } catch (bad_alloc&) { throw; }
}

bool NodeList::deleteNode(char *nodeName) {
    if (firstNode == NULL) return false;        // if list is empty
    NodeListnode *current = firstNode, *prev = firstNode;
    while (current != NULL && strcmp(current->nodeName, nodeName) < 0) {
        prev = current;
        current = current->nextNode;
    }
    // if reached end of list or surpassed node assumed position
    if (current == NULL || strcmp(current->nodeName, nodeName) != 0) {
//        cout << "Warning: Node not found!" << endl;
        return false;
    }
    if (strcmp(current->nodeName, nodeName) == 0) {     // node found
        if (current == firstNode) {
            firstNode = current->nextNode;
        } else {
            prev->nextNode = current->nextNode;
        }
        delete current;
        return true;
    }

}

void NodeList::print() const {
    NodeListnode *current = firstNode;
    while (current != NULL) {
        cout << current->nodeName;
        current->edges->print();
        current = current->nextNode;
    }
    cout << '/' << endl;
}

