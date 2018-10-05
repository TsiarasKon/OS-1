#include <iostream>
#include <cstring>
#include "lists.h"
#include "util.h"

using namespace std;

// EdgeListNode methods:
EdgeListnode::EdgeListnode(NodeListnode *receivingNode, int weight, EdgeListnode *nextEdge) :
        receivingNode(receivingNode), weight(weight), nextEdge(nextEdge) {}

EdgeListnode::~EdgeListnode() {};

NodeListnode *EdgeListnode::getReceivingNode() const {
    return receivingNode;
}

int EdgeListnode::getWeight() const {
    return weight;
}

EdgeListnode *EdgeListnode::getNextEdge() const {
    return nextEdge;
}

void EdgeListnode::setNextEdge(EdgeListnode *nextEdge) {
    EdgeListnode::nextEdge = nextEdge;
}


// EdgeList methods:
EdgeList::EdgeList() : firstEdge(NULL) {}

EdgeList::~EdgeList() {
    EdgeListnode *current = firstEdge, *next;
    while (current != NULL) {
        next = current->getNextEdge();
        delete current;
        current = next;
    }
}

EdgeListnode *EdgeList::getFirstEdge() const {
    return firstEdge;
}

void EdgeList::setFirstEdge(EdgeListnode *firstEdge) {
    EdgeList::firstEdge = firstEdge;
}

void EdgeList::print() const {
    EdgeListnode *current = firstEdge;
    while (current != NULL) {
        cout << " >" << current->getReceivingNode()->getNodeName() << "(" << current->getWeight() << ")";
    }
    cout << " >/" << endl;
}


// NodeListNode methods:
NodeListnode::NodeListnode(char *nodeName, NodeListnode *nextNode) :
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

NodeListnode::~NodeListnode() {
    delete edges;
    delete[] nodeName;
}

char *NodeListnode::getNodeName() const {
    return nodeName;
}

EdgeList *NodeListnode::getEdges() const {
    return edges;
}

NodeListnode *NodeListnode::getNextNode() const {
    return nextNode;
}

void NodeListnode::setNextNode(NodeListnode *nextNode) {
    NodeListnode::nextNode = nextNode;
}


// NodeList methods:
NodeList::NodeList() : firstNode(NULL) {}

NodeList::~NodeList() {
    NodeListnode *current = firstNode, *next;
    while (current != NULL) {
        next = current->getNextNode();
        delete current;
        current = next;
    }
}

NodeListnode *NodeList::getFirstNode() const {
    return firstNode;
}

void NodeList::setFirstNode(NodeListnode *firstNode) {
    NodeList::firstNode = firstNode;
}

void NodeList::print() const {
    NodeListnode *current = firstNode;
    while (current != NULL) {
        cout << current->getNodeName();
        current->getEdges()->print();
        current = current->getNextNode();
    }
    cout << '/' << endl;
}

bool NodeList::insertInOrder(char *nodeName) {
    try {
        NodeListnode *current = firstNode, *prev = firstNode;
        while (current != NULL && strcmp(current->getNodeName(), nodeName) < 0) {
            prev = current;
            current = current->getNextNode();
        }
        if (current == firstNode && (current == NULL || strcmp(current->getNodeName(), nodeName) != 0)) {     // insert at start
            firstNode = new NodeListnode(nodeName, firstNode);
            return true;
        }
        if (current != NULL) {
            if (strcmp(current->getNodeName(), nodeName) == 0) {
//                cout << "Warning: Attempted to insert node with a name that already exists!" << endl;
                return false;
            } else {        // just surpassed where the node would have been found, if it existed
                prev->setNextNode(new NodeListnode(nodeName, current));
                return true;
            }
        } else {    // reached the end of the list
            prev->setNextNode(new NodeListnode(nodeName, NULL));
            return true;
        }
    } catch (bad_alloc&) { throw; }
}

bool NodeList::deleteNode(char *nodeName) {
    if (firstNode == NULL) return false;        // if list is empty
    NodeListnode *current = firstNode, *prev = firstNode;
    while (current != NULL && strcmp(current->getNodeName(), nodeName) < 0) {
        prev = current;
        current = current->getNextNode();
    }
    // if reached end of list or surpassed node assumed position
    if (current == NULL || strcmp(current->getNodeName(), nodeName) != 0) {
//        cout << "Warning: Node not found!" << endl;
        return false;
    }
    if (strcmp(current->getNodeName(), nodeName) == 0) {     // node found
        if (current == firstNode) {
            firstNode = current->getNextNode();
        } else {
            prev->setNextNode(current->getNextNode());
        }
        delete current;
        return true;
    }

}

