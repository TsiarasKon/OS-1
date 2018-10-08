#include <iostream>
#include <cstring>
#include <iomanip>
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

void EdgeList::print(std::ostream& outstream) const {
    EdgeListnode *current = firstEdge;
    outstream << endl;
    while (current != NULL) {
        outstream << "\t\t--" << setfill('-') << setw(4) << current->getWeight() << "-->|" << current->getReceivingNode()->getNodeName() << "|" << endl;
        current = current->getNextEdge();
    }
    outstream << endl;
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

int EdgeList::deleteAllEdges(char *toNodeName) {
    EdgeListnode *current = firstEdge, *prev = firstEdge;
    while (current != NULL && strcmp(current->getReceivingNode()->getNodeName(), toNodeName) < 0) {
        prev = current;
        current = current->getNextEdge();
    }
    int result = -3;    // No edges found
    if (current == firstEdge) {
        while (current != NULL && current->getReceivingNode() != NULL && !strcmp(current->getReceivingNode()->getNodeName(), toNodeName)) {    // there may exist multiple such edges
            firstEdge = current->getNextEdge();
            delete current;
            result = 0;
            current = firstEdge;
        }
    } else {
        while (current != NULL && current->getReceivingNode() != NULL && !strcmp(current->getReceivingNode()->getNodeName(), toNodeName)) {    // there may exist multiple such edges
            prev->setNextEdge(current->getNextEdge());
            delete current;
            result = 0;
            current = prev->getNextEdge();
        }
    }
    return result;
}

int EdgeList::deleteEdgesWithWeight(char *toNodeName, int weight) {
    EdgeListnode *current = firstEdge, *prev = firstEdge;
    while (current != NULL && strcmp(current->getReceivingNode()->getNodeName(), toNodeName) < 0) {
        prev = current;
        current = current->getNextEdge();
    }
    int result = -3;    // edge with specified weight not found
    while (current != NULL && current->getReceivingNode() != NULL && !strcmp(current->getReceivingNode()->getNodeName(), toNodeName)) {    // there may exist multiple such edges
        if (current->getWeight() == weight) {
            if (current == firstEdge) {
                firstEdge = current->getNextEdge();
            } else {
                prev->setNextEdge(current->getNextEdge());
            }
            delete current;
            return 0;
        }
        prev = current;
        current = current->getNextEdge();
    }
    return result;
}

int EdgeList::modifyEdge(char *toNodeName, int weight, int nweight) {
    EdgeListnode *current = firstEdge;
    while (current != NULL && strcmp(current->getReceivingNode()->getNodeName(), toNodeName) < 0) {
        current = current->getNextEdge();
    }
    int result = -3;    // edge with specified weight not found
    while (current != NULL && current->getReceivingNode() != NULL && !strcmp(current->getReceivingNode()->getNodeName(), toNodeName)) {    // there may exist multiple such edges
        if (current->getWeight() == weight) {
            current->setWeight(nweight);
            return 0;
        }
        current = current->getNextEdge();
    }
    return result;
}

bool EdgeList::printTransactionsTo(char *fromNodeName, char *toNodeName) {
    EdgeListnode *current = firstEdge;
    while (current != NULL && strcmp(current->getReceivingNode()->getNodeName(), toNodeName) < 0) {
        current = current->getNextEdge();
    }
    bool printed = false;
    while (current != NULL && current->getReceivingNode() != NULL && !strcmp(current->getReceivingNode()->getNodeName(), toNodeName)) {
        cout << " |" << fromNodeName << "|--" << current->getWeight() << "-->|" <<
             toNodeName << "|" << endl;
        printed = true;
        current = current->getNextEdge();
    }
    return printed;
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

void NodeList::print(std::ostream& outstream) const {
    NodeListnode *current = firstNode;
    while (current != NULL) {
        outstream << " |" << current->getNodeName() << "|";
        current->getEdges()->print(outstream);
        current = current->getNextNode();
    }
}

NodeListnode *NodeList::getNodeByName(char *nodeName) {
    NodeListnode *current = firstNode;
    while (current != NULL && strcmp(current->getNodeName(), nodeName) < 0) {
        current = current->getNextNode();
    }
    // if reached end of list or surpassed node assumed position
    if (current == NULL || strcmp(current->getNodeName(), nodeName) != 0) {
        return NULL;
    }
    // node found
    return current;
}

NodeListnode *NodeList::insertNode(char *nodeName) {        // returns the created node
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
            if (strcmp(current->getNodeName(), nodeName) == 0) {        // node already exists
                return NULL;
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
    /* Similar algorithm to insertNode() - if fromNode isn't found we create it and then insert the edge.
     * In either case, toNode is first created if it doesn't exist. */
    try {
        NodeListnode *toNode = insertNode(toNodeName);
        if (toNode == NULL) {       // toNode already existed
            toNode = getNodeByName(toNodeName);
        }
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

int NodeList::deleteAllEdges(char *fromNodeName, char *toNodeName) {
    NodeListnode *fromNode = getNodeByName(fromNodeName);
    if (fromNode == NULL) {
        return -1;
    }
    if (getNodeByName(toNodeName) == NULL) {
        return -2;
    }
    return fromNode->getEdges()->deleteAllEdges(toNodeName);
}

int NodeList::deleteEdgesWithWeight(char *fromNodeName, char *toNodeName, int weight) {
    NodeListnode *fromNode = getNodeByName(fromNodeName);
    if (fromNode == NULL) {
        return -1;
    }
    if (getNodeByName(toNodeName) == NULL) {
        return -2;
    }
    return fromNode->getEdges()->deleteEdgesWithWeight(toNodeName, weight);
}

int NodeList::modifyEdge(char *fromName, char *toNodeName, int weight, int nweight) {
    NodeListnode *fromNode = getNodeByName(fromName);
    if (fromNode == NULL) {
        return -1;
    }
    if (getNodeByName(toNodeName) == NULL) {
        return -2;
    }
    return fromNode->getEdges()->modifyEdge(toNodeName, weight, nweight);
}

void NodeList::printReceiving(char *nodeName) {
    NodeListnode *toNode = getNodeByName(nodeName);
    if (toNode == NULL) {
        cout << " |" << nodeName << "| does not exist - abort-r;" << endl;
        return;
    }
    NodeListnode *current = firstNode;
    bool printed = false;
    while (current != NULL) {
        printed = (current->getEdges()->printTransactionsTo(current->getNodeName(), nodeName)) || printed;      // prints and updates flag
        current = current->getNextNode();
    }
    if (!printed) {
        cout << " No-rec-edges |" << nodeName << "|" << endl;
    }
}
