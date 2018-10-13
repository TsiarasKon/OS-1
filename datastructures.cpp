#include <iostream>
#include <cstring>
#include <iomanip>
#include "datastructures.h"
#include "util.h"

using namespace std;

// EdgeListNode methods:
Edge::Edge(Node *receivingNode, int weight, Edge *nextEdge) :
        receivingNode(receivingNode), weight(weight), nextEdge(nextEdge), visited(false) {}

Edge::~Edge() {}

Node *Edge::getReceivingNode() const {
    return receivingNode;
}

int Edge::getWeight() const {
    return weight;
}

void Edge::setWeight(int weight) {
    Edge::weight = weight;
}

Edge *Edge::getNextEdge() const {
    return nextEdge;
}

void Edge::setNextEdge(Edge *nextEdge) {
    Edge::nextEdge = nextEdge;
}

bool Edge::getVisited() const {
    return visited;
}

void Edge::setVisited(bool visited) {
    Edge::visited = visited;
}


// EdgeList methods:
EdgeList::EdgeList() : firstEdge(NULL) {}

EdgeList::~EdgeList() {
    Edge *current = firstEdge, *next;
    while (current != NULL) {
        next = current->getNextEdge();
        delete current;
        current = next;
    }
}

Edge *EdgeList::getFirstEdge() const {
    return firstEdge;
}

void EdgeList::print(std::ostream& outstream) const {
    Edge *current = firstEdge;
    outstream << endl;
    while (current != NULL) {
        outstream << "\t\t-" << current->getWeight() << "->|" << current->getReceivingNode()->getNodeName() << "|" << endl;
        current = current->getNextEdge();
    }
    outstream << endl;
}

void EdgeList::insertEdge(Node *toNode, int weight) {
    try {
        Edge *current = firstEdge, *prev = firstEdge;
        while (current != NULL && strcmp(current->getReceivingNode()->getNodeName(), toNode->getNodeName()) < 0) {
            prev = current;
            current = current->getNextEdge();
        }
        if (current == firstEdge) {     // insert at start
            firstEdge = new Edge(toNode, weight, firstEdge);
            return;
        }
        if (current != NULL) {
            // just reached edge's assumed position
            prev->setNextEdge(new Edge(toNode, weight, current));
        } else {    // reached the end of the list
            prev->setNextEdge(new Edge(toNode, weight, NULL));
        }
    } catch (bad_alloc& e) { throw; }
}

int EdgeList::deleteAllEdges(char *toNodeName) {
    Edge *current = firstEdge, *prev = firstEdge;
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
    Edge *current = firstEdge, *prev = firstEdge;
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
    Edge *current = firstEdge;
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

void EdgeList::printTransactionsTo(char *fromNodeName, char *toNodeName, bool *printed) const {
    Edge *current = firstEdge;
    while (current != NULL && strcmp(current->getReceivingNode()->getNodeName(), toNodeName) < 0) {
        current = current->getNextEdge();
    }
    while (current != NULL && current->getReceivingNode() != NULL && !strcmp(current->getReceivingNode()->getNodeName(), toNodeName)) {
        if (! *printed) {
            cout << " Rec-edges";
        } else {
            cout << "          ";
        }
        cout << " |" << fromNodeName << "|->" << current->getWeight() << "->|" <<
             toNodeName << "|" << endl;
        *printed = true;
        current = current->getNextEdge();
    }
}


// Cycle methods
Cycle::Cycle(Node *startingNode) : EdgeList(), startingNode(startingNode), lastEdge(NULL) {}

Node *Cycle::getStartingNode() const {
    return startingNode;
}

Edge *Cycle::getLastEdge() const {
    return lastEdge;
}

void Cycle::insertUnordered(Node *toNode, int weight) {
    try {
        if (firstEdge == NULL) {
            firstEdge = new Edge(toNode, weight, NULL);
            lastEdge = firstEdge;
        } else {
            lastEdge->setNextEdge(new Edge(toNode, weight, NULL));
            lastEdge = lastEdge->getNextEdge();
        }
    } catch (bad_alloc& e) { throw; }
}

void Cycle::deleteLast() {
    Edge *current = firstEdge;
    while (current != NULL && current != lastEdge && current->getNextEdge() != lastEdge) {     // current should be the penultimate edge (or firstedge)
        current = current->getNextEdge();
    }
    if (current == NULL) {
        cerr << "Attempted to deleteLast() from empty cycle?" << endl;
    } else if (current == lastEdge) {
        delete lastEdge;
        firstEdge = lastEdge = NULL;
    } else {
        delete lastEdge;
        current->setNextEdge(NULL);
        lastEdge = current;
    }
}

/* 0: not exists; 1: is startingNode; 2: is another node */
int Cycle::nodeExists(Node *node) const {
    if (startingNode == node) return 1;
    Edge *current = firstEdge;
    while (current != NULL) {
        if (current->getReceivingNode() == node) return 2;
        current = current->getNextEdge();
    }
    return 0;
}

void Cycle::printCycle() const {
    cout << "|" << startingNode->getNodeName() << "|";
    Edge *current = firstEdge;
    while (current != NULL) {
        cout << "->" << current->getWeight() << "->|" << current->getReceivingNode()->getNodeName() << "|";
        current = current->getNextEdge();
    }
    cout << endl;
}


// Node methods:
Node::Node(char *nodeName, Node *nextNode) :
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

Node::~Node() {
    delete edges;
    delete[] nodeName;
}

char *Node::getNodeName() const {
    return nodeName;
}

EdgeList *Node::getEdges() const {
    return edges;
}

Node *Node::getNextNode() const {
    return nextNode;
}

void Node::setNextNode(Node *nextNode) {
    Node::nextNode = nextNode;
}

bool Node::simpleCycleCheck(Cycle *visited) {
    bool foundCycle = false;
    Edge *currentEdge = edges->getFirstEdge();
    while (currentEdge != NULL) {
        int existsRes = visited->nodeExists(currentEdge->getReceivingNode());
        try {
            if (existsRes == 1) {         // cycle found with startingNode
                foundCycle = true;
                visited->insertUnordered(currentEdge->getReceivingNode(),
                                         currentEdge->getWeight());
                cout << " Cir-found ";
                visited->printCycle();
                visited->deleteLast();
            } else if (existsRes == 0) {        // no cycle - check next
                visited->insertUnordered(currentEdge->getReceivingNode(),
                                         currentEdge->getWeight());
                foundCycle = currentEdge->getReceivingNode()->simpleCycleCheck(
                        visited) || foundCycle;
                visited->deleteLast();
            }
        } catch (bad_alloc&) { throw; }
        currentEdge = currentEdge->getNextEdge();
    }
    return foundCycle;
}

bool Node::cyclicTransactionCheck(Cycle *visited, int k) {
    bool foundCycle = false;
    Edge *currentEdge = edges->getFirstEdge();
    while (currentEdge != NULL) {
        if (currentEdge->getWeight() >= k) {         // if less than k, abandon path
            try {
                // if already visited edge or currentNode is the same as the starting one, circle found
                if (currentEdge->getVisited() || (visited->getLastEdge() != NULL && visited->getLastEdge()->getReceivingNode() == visited->getStartingNode())) {
                    if (visited->getLastEdge()->getReceivingNode() == visited->getStartingNode()) {
                        cout << " Cir-found ";
                        visited->printCycle();
                        return true;
                    }
                } else {
                    currentEdge->setVisited(true);
                    visited->insertUnordered(currentEdge->getReceivingNode(),
                                             currentEdge->getWeight());
                    foundCycle =
                            currentEdge->getReceivingNode()->cyclicTransactionCheck(
                                    visited, k) || foundCycle;
                    visited->deleteLast();
                    currentEdge->setVisited(false);
                }
            } catch (bad_alloc &) { throw; }
        }
        currentEdge = currentEdge->getNextEdge();
    }
    return foundCycle;
}

bool Node::traceflowCheck(Cycle *visited, Node *toNode, int len) {
    if (len < 0) return false;
    bool foundCycle = false;
    Edge *currentEdge = edges->getFirstEdge();
    while (currentEdge != NULL) {
        try {
            // if already visited edge or currentNode is toNode, circle found
            if (currentEdge->getVisited() || (visited->getLastEdge() != NULL && visited->getLastEdge()->getReceivingNode() == toNode)) {
                if (visited->getLastEdge()->getReceivingNode() == toNode) {
                    cout << " Tra-found ";
                    visited->printCycle();
                    return true;
                }
            } else {
                currentEdge->setVisited(true);
                visited->insertUnordered(currentEdge->getReceivingNode(),
                                         currentEdge->getWeight());
                foundCycle =
                        currentEdge->getReceivingNode()->traceflowCheck(visited, toNode, len - 1) || foundCycle;
                visited->deleteLast();
                currentEdge->setVisited(false);
            }
        } catch (bad_alloc &) { throw; }
        currentEdge = currentEdge->getNextEdge();
    }
    return foundCycle;
}


// Graph methods:
Graph::Graph() : firstNode(NULL) {}

Graph::~Graph() {
    Node *current = firstNode, *next;
    while (current != NULL) {
        next = current->getNextNode();
        delete current;
        current = next;
    }
}

void Graph::print(std::ostream& outstream) const {
    Node *current = firstNode;
    while (current != NULL) {
        outstream << " |" << current->getNodeName() << "|";
        current->getEdges()->print(outstream);
        current = current->getNextNode();
    }
}

Node *Graph::getNodeByName(char *nodeName) const {
    Node *current = firstNode;
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

Node *Graph::insertNode(char *nodeName) {        // returns the created node
    try {
        Node *current = firstNode, *prev = firstNode;
        while (current != NULL && strcmp(current->getNodeName(), nodeName) < 0) {
            prev = current;
            current = current->getNextNode();
        }
        if (current == firstNode && (current == NULL || strcmp(current->getNodeName(), nodeName) != 0)) {     // insert at start
            firstNode = new Node(nodeName, firstNode);
            return firstNode;
        }
        if (current != NULL) {
            if (strcmp(current->getNodeName(), nodeName) == 0) {        // node already exists
                return NULL;
            } else {        // just surpassed where the node would have been found, if it existed
                prev->setNextNode(new Node(nodeName, current));
                return prev->getNextNode();
            }
        } else {    // reached the end of the list
            prev->setNextNode(new Node(nodeName, NULL));
            return prev->getNextNode();
        }
    } catch (bad_alloc&) { throw; }
}

void Graph::insertEdge(char *fromNodeName, char *toNodeName, int weight) {
    /* Similar algorithm to insertNode() - if fromNode isn't found we create it and then insert the edge.
     * In either case, toNode is first created if it doesn't exist. */
    try {
        Node *toNode = insertNode(toNodeName);
        if (toNode == NULL) {       // toNode already existed
            toNode = getNodeByName(toNodeName);
        }
        Node *current = firstNode, *prev = firstNode;
        while (current != NULL && strcmp(current->getNodeName(), fromNodeName) < 0) {
            prev = current;
            current = current->getNextNode();
        }
        if (current == firstNode && (current == NULL || strcmp(current->getNodeName(), fromNodeName) != 0)) {     // insert at start
            firstNode = new Node(fromNodeName, firstNode);
            firstNode->getEdges()->insertEdge(toNode, weight);
            return;
        }
        if (current != NULL) {
            if (strcmp(current->getNodeName(), fromNodeName) == 0) {
                current->getEdges()->insertEdge(toNode, weight);
            } else {        // just surpassed where the node would have been found, if it existed
                prev->setNextNode(new Node(fromNodeName, current));
                prev->getNextNode()->getEdges()->insertEdge(toNode, weight);
            }
        } else {    // reached the end of the list
            prev->setNextNode(new Node(fromNodeName, NULL));
            prev->getNextNode()->getEdges()->insertEdge(toNode, weight);
        }
    } catch (bad_alloc&) { throw; }
}

bool Graph::deleteNode(char *nodeName) {
    if (firstNode == NULL) return false;        // if list is empty
    Node *current = firstNode, *prev = firstNode;
    while (current != NULL && strcmp(current->getNodeName(), nodeName) < 0) {
        prev = current;
        current = current->getNextNode();
    }
    // if reached end of list or surpassed node assumed position
    if (current == NULL || strcmp(current->getNodeName(), nodeName) != 0) {
        return false;
    }
    // node found
    Node *i = firstNode;
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

int Graph::deleteAllEdges(char *fromNodeName, char *toNodeName) {
    Node *fromNode = getNodeByName(fromNodeName);
    if (fromNode == NULL) {
        return -1;
    }
    if (getNodeByName(toNodeName) == NULL) {
        return -2;
    }
    return fromNode->getEdges()->deleteAllEdges(toNodeName);
}

int Graph::deleteEdgesWithWeight(char *fromNodeName, char *toNodeName, int weight) {
    Node *fromNode = getNodeByName(fromNodeName);
    if (fromNode == NULL) {
        return -1;
    }
    if (getNodeByName(toNodeName) == NULL) {
        return -2;
    }
    return fromNode->getEdges()->deleteEdgesWithWeight(toNodeName, weight);
}

int Graph::modifyEdge(char *fromName, char *toNodeName, int weight, int nweight) {
    Node *fromNode = getNodeByName(fromName);
    if (fromNode == NULL) {
        return -1;
    }
    if (getNodeByName(toNodeName) == NULL) {
        return -2;
    }
    return fromNode->getEdges()->modifyEdge(toNodeName, weight, nweight);
}

void Graph::printReceiving(char *nodeName) const {
    Node *toNode = getNodeByName(nodeName);
    if (toNode == NULL) {
        cout << " |" << nodeName << "| does not exist - abort-r;" << endl;
        return;
    }
    Node *current = firstNode;
    bool printed = false;
    while (current != NULL) {
        current->getEdges()->printTransactionsTo(current->getNodeName(), nodeName, &printed);      // prints and updates flag
        current = current->getNextNode();
    }
    if (!printed) {
        cout << " No-rec-edges |" << nodeName << "|" << endl;
    }
}

void Graph::circlefind(char *nodeName) const {
    Node *node = getNodeByName(nodeName);
    if (node == NULL) {
        cout << " |" << nodeName << "| does not exist - abort-c;" << endl;
        return;
    }
    Cycle *visited;
    try {
        visited = new Cycle(node);
        if (!node->simpleCycleCheck(visited)) {
            cout << " No-circle-found |" << nodeName << "|" << endl;
        }
    } catch (bad_alloc&) { throw; }
    delete visited;
}

void Graph::findcircles(char *nodeName, int k) const {
    Node *node = getNodeByName(nodeName);
    if (node == NULL) {
        cout << " |" << nodeName << "| does not exist - abort-f;" << endl;
        return;
    }
    Cycle *visited;
    try {
        visited = new Cycle(node);
        if (!node->cyclicTransactionCheck(visited, k)) {
            cout << " No-circle-found invloving |" << nodeName << "| " << k << endl;
        }
    } catch (bad_alloc&) { throw; }
    delete visited;
}

void Graph::traceflow(char *fromNodeName, char *toNodeName, int len) const {
    Node *fromNode = getNodeByName(fromNodeName);
    if (fromNode == NULL) {
        cout << " |" << fromNodeName << "| does not exist - abort-t;" << endl;
        return;
    }
    Node *toNode = getNodeByName(toNodeName);
    if (toNode == NULL) {
        cout << " |" << toNodeName << "| does not exist - abort-t;" << endl;
        return;
    }
    Cycle *visited;
    try {
        visited = new Cycle(fromNode);
        if (!fromNode->traceflowCheck(visited, toNode, len)) {
            cout << " No-trace from |" << fromNodeName << "| to |" << toNodeName << "|" << endl;
        }
    } catch (bad_alloc&) { throw; }
    delete visited;
}
