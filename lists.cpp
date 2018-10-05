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

void EdgeListnode::setWeight(int weight) {
    EdgeListnode::weight = weight;
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
        current = current->getNextEdge();
    }
    cout << " >/" << endl;
}

void EdgeList::insertEdge(NodeListnode *toNode, int weight) {
    try {
        EdgeListnode *current = firstEdge, *prev = firstEdge;
        while (current != NULL && strcmp(current->getReceivingNode()->getNodeName(), toNode->getNodeName()) < 0) {
            prev = current;
            current = current->getNextEdge();
        }
        if (current == firstEdge) {     // insert at start
            firstEdge = new EdgeListnode(toNode, weight, firstEdge);
            return;
        }
        if (current != NULL) {
            // just reached edge's assumed position
            prev->setNextEdge(new EdgeListnode(toNode, weight, current));
        } else {    // reached the end of the list
            prev->setNextEdge(new EdgeListnode(toNode, weight, NULL));
        }
    } catch (bad_alloc& e) { throw; }
}

void EdgeList::deleteAllEdges(char *toNodeName) {
    EdgeListnode *current = firstEdge, *prev = firstEdge;
    while (current != NULL && strcmp(current->getReceivingNode()->getNodeName(), toNodeName) < 0) {
        prev = current;
        current = current->getNextEdge();
    }
    if (current == firstEdge) {
        while (current != NULL && current->getReceivingNode() != NULL && !strcmp(current->getReceivingNode()->getNodeName(), toNodeName)) {    // there may exist multiple such edges
            firstEdge = current->getNextEdge();
            delete current;
            current = firstEdge;
        }
    } else {
        while (current != NULL && current->getReceivingNode() != NULL && !strcmp(current->getReceivingNode()->getNodeName(), toNodeName)) {    // there may exist multiple such edges
            prev->setNextEdge(current->getNextEdge());
            delete current;
            current = prev->getNextEdge();
        }
    }
}

bool EdgeList::deleteEdgesWithWeight(char *toNodeName, int weight) {
    EdgeListnode *current = firstEdge, *prev = firstEdge;
    while (current != NULL && strcmp(current->getReceivingNode()->getNodeName(), toNodeName) < 0) {
        prev = current;
        current = current->getNextEdge();
    }
    while (current != NULL && current->getReceivingNode() != NULL && !strcmp(current->getReceivingNode()->getNodeName(), toNodeName)) {    // there may exist multiple such edges
        if (current->getWeight() == weight) {
            if (current == firstEdge) {
                firstEdge = current->getNextEdge();
            } else {
                prev->setNextEdge(current->getNextEdge());
            }
            delete current;
            return true;
        }
        prev = current;
        current = current->getNextEdge();
    }
    return false;
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

NodeListnode *NodeList::insertNode(char *nodeName) {        // also acts as a node getter
    try {
        NodeListnode *current = firstNode, *prev = firstNode;
        while (current != NULL && strcmp(current->getNodeName(), nodeName) < 0) {
            prev = current;
            current = current->getNextNode();
        }
        if (current == firstNode && (current == NULL || strcmp(current->getNodeName(), nodeName) != 0)) {     // insert at start
            firstNode = new NodeListnode(nodeName, firstNode);
            return firstNode;
        }
        if (current != NULL) {
            if (strcmp(current->getNodeName(), nodeName) == 0) {
                return current;
            } else {        // just surpassed where the node would have been found, if it existed
                prev->setNextNode(new NodeListnode(nodeName, current));
                return prev->getNextNode();
            }
        } else {    // reached the end of the list
            prev->setNextNode(new NodeListnode(nodeName, NULL));
            return prev->getNextNode();
        }
    } catch (bad_alloc&) { throw; }
}

void NodeList::insertEdge(char *fromNodeName, char *toNodeName, int weight) {
    // Similar algorithm to insertNode() - if node isn't found we create it and then insert the edge
    try {
        NodeListnode *toNode = insertNode(toNodeName);
        NodeListnode *current = firstNode, *prev = firstNode;
        while (current != NULL && strcmp(current->getNodeName(), fromNodeName) < 0) {
            prev = current;
            current = current->getNextNode();
        }
        if (current == firstNode && (current == NULL || strcmp(current->getNodeName(), fromNodeName) != 0)) {     // insert at start
            firstNode = new NodeListnode(fromNodeName, firstNode);
            firstNode->getEdges()->insertEdge(toNode, weight);
            return;
        }
        if (current != NULL) {
            if (strcmp(current->getNodeName(), fromNodeName) == 0) {
                current->getEdges()->insertEdge(toNode, weight);
            } else {        // just surpassed where the node would have been found, if it existed
                prev->setNextNode(new NodeListnode(fromNodeName, current));
                prev->getNextNode()->getEdges()->insertEdge(toNode, weight);
            }
        } else {    // reached the end of the list
            prev->setNextNode(new NodeListnode(fromNodeName, NULL));
            prev->getNextNode()->getEdges()->insertEdge(toNode, weight);
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
        return false;
    }
    // node found
    NodeListnode *i = firstNode;
    while (i != NULL) {
        i->getEdges()->deleteAllEdges(nodeName);
        i = i->getNextNode();
    }
    if (current == firstNode) {
        firstNode = current->getNextNode();
    } else {
        prev->setNextNode(current->getNextNode());
    }
    delete current;
    return true;
}

bool NodeList::deleteAllEdges(char *fromName, char *toNodeName) {
    NodeListnode *current = firstNode;
    while (current != NULL && strcmp(current->getNodeName(), fromName) < 0) {
        current = current->getNextNode();
    }
    // if reached end of list or surpassed node assumed position
    if (current == NULL || strcmp(current->getNodeName(), fromName) != 0) {
        return false;
    }
    // node found
    current->getEdges()->deleteAllEdges(toNodeName);
    return true;
}

bool NodeList::deleteEdgesWithWeight(char *fromName, char *toNodeName, int weight) {
    NodeListnode *current = firstNode;
    while (current != NULL && strcmp(current->getNodeName(), fromName) < 0) {
        current = current->getNextNode();
    }
    // if reached end of list or surpassed node assumed position
    if (current == NULL || strcmp(current->getNodeName(), fromName) != 0) {
        return false;
    }
    // node found
    current->getEdges()->deleteEdgesWithWeight(toNodeName, weight);
    return true;
}
