#include <iostream>
#include <cstring>
#include <iomanip>
#include "lists.h"
#include "util.h"

using namespace std;

// EdgeListNode methods:
EdgeListnode::EdgeListnode(GraphNode *receivingNode, int weight, EdgeListnode *nextEdge) :
        receivingNode(receivingNode), weight(weight), nextEdge(nextEdge) {}

EdgeListnode::~EdgeListnode() {};

GraphNode *EdgeListnode::getReceivingNode() const {
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

void EdgeList::insertEdge(GraphNode *toNode, int weight) {
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

bool EdgeList::printTransactionsTo(char *fromNodeName, char *toNodeName) const {
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


// Cycle methods
Cycle::Cycle(char *startingNodeName) : EdgeList(), startingNodeName(startingNodeName), lastEdge(NULL) {}

char *Cycle::getStartingNodeName() const {
    return startingNodeName;
}

void Cycle::insertUnordered(GraphNode *toNode, int weight) {
    try {
        if (firstEdge == NULL) {
            firstEdge = new EdgeListnode(toNode, weight, NULL);
            lastEdge = firstEdge;
        } else {
            lastEdge->setNextEdge(new EdgeListnode(toNode, weight, NULL));
            lastEdge = lastEdge->getNextEdge();
        }
    } catch (bad_alloc& e) { throw; }
}

void Cycle::deleteLast() {
    EdgeListnode *current = firstEdge;
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

void Cycle::printCycle() const {
    cout << "Cir-found |" << startingNodeName << "|";
    EdgeListnode *current = firstEdge;
    while (current != NULL) {
        cout << "--" << current->getWeight() << "-->|" << current->getReceivingNode()->getNodeName() << "|";
        current = current->getNextEdge();
    }
    cout << endl;
}


// NodeListNode methods:
GraphNode::GraphNode(char *nodeName, GraphNode *nextNode) :
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

GraphNode::~GraphNode() {
    delete edges;
    delete[] nodeName;
}

char *GraphNode::getNodeName() const {
    return nodeName;
}

EdgeList *GraphNode::getEdges() const {
    return edges;
}

GraphNode *GraphNode::getNextNode() const {
    return nextNode;
}

void GraphNode::setNextNode(GraphNode *nextNode) {
    GraphNode::nextNode = nextNode;
}

bool GraphNode::checkNextForCycle(Cycle *visited) {
    EdgeListnode *currentEdge = edges->getFirstEdge();
    while (currentEdge != NULL) {
        visited->insertUnordered(currentEdge->getReceivingNode(), currentEdge->getWeight());
        if (!strcmp(currentEdge->getReceivingNode()->getNodeName(), visited->getStartingNodeName())) {         // cycle found
            visited->printCycle();
            return true;
        }
        currentEdge->getReceivingNode()->checkNextForCycle(visited);
        visited->deleteLast();
        currentEdge = currentEdge->getNextEdge();
    }
    return false;
}


// Graph methods:
Graph::Graph() : firstNode(NULL) {}

Graph::~Graph() {
    GraphNode *current = firstNode, *next;
    while (current != NULL) {
        next = current->getNextNode();
        delete current;
        current = next;
    }
}

GraphNode *Graph::getFirstNode() const {
    return firstNode;
}

void Graph::setFirstNode(GraphNode *firstNode) {
    Graph::firstNode = firstNode;
}

void Graph::print(std::ostream& outstream) const {
    GraphNode *current = firstNode;
    while (current != NULL) {
        outstream << " |" << current->getNodeName() << "|";
        current->getEdges()->print(outstream);
        current = current->getNextNode();
    }
}

GraphNode *Graph::getNodeByName(char *nodeName) const {
    GraphNode *current = firstNode;
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

GraphNode *Graph::insertNode(char *nodeName) {        // returns the created node
    try {
        GraphNode *current = firstNode, *prev = firstNode;
        while (current != NULL && strcmp(current->getNodeName(), nodeName) < 0) {
            prev = current;
            current = current->getNextNode();
        }
        if (current == firstNode && (current == NULL || strcmp(current->getNodeName(), nodeName) != 0)) {     // insert at start
            firstNode = new GraphNode(nodeName, firstNode);
            return firstNode;
        }
        if (current != NULL) {
            if (strcmp(current->getNodeName(), nodeName) == 0) {        // node already exists
                return NULL;
            } else {        // just surpassed where the node would have been found, if it existed
                prev->setNextNode(new GraphNode(nodeName, current));
                return prev->getNextNode();
            }
        } else {    // reached the end of the list
            prev->setNextNode(new GraphNode(nodeName, NULL));
            return prev->getNextNode();
        }
    } catch (bad_alloc&) { throw; }
}

void Graph::insertEdge(char *fromNodeName, char *toNodeName, int weight) {
    /* Similar algorithm to insertNode() - if fromNode isn't found we create it and then insert the edge.
     * In either case, toNode is first created if it doesn't exist. */
    try {
        GraphNode *toNode = insertNode(toNodeName);
        if (toNode == NULL) {       // toNode already existed
            toNode = getNodeByName(toNodeName);
        }
        GraphNode *current = firstNode, *prev = firstNode;
        while (current != NULL && strcmp(current->getNodeName(), fromNodeName) < 0) {
            prev = current;
            current = current->getNextNode();
        }
        if (current == firstNode && (current == NULL || strcmp(current->getNodeName(), fromNodeName) != 0)) {     // insert at start
            firstNode = new GraphNode(fromNodeName, firstNode);
            firstNode->getEdges()->insertEdge(toNode, weight);
            return;
        }
        if (current != NULL) {
            if (strcmp(current->getNodeName(), fromNodeName) == 0) {
                current->getEdges()->insertEdge(toNode, weight);
            } else {        // just surpassed where the node would have been found, if it existed
                prev->setNextNode(new GraphNode(fromNodeName, current));
                prev->getNextNode()->getEdges()->insertEdge(toNode, weight);
            }
        } else {    // reached the end of the list
            prev->setNextNode(new GraphNode(fromNodeName, NULL));
            prev->getNextNode()->getEdges()->insertEdge(toNode, weight);
        }
    } catch (bad_alloc&) { throw; }
}

bool Graph::deleteNode(char *nodeName) {
    if (firstNode == NULL) return false;        // if list is empty
    GraphNode *current = firstNode, *prev = firstNode;
    while (current != NULL && strcmp(current->getNodeName(), nodeName) < 0) {
        prev = current;
        current = current->getNextNode();
    }
    // if reached end of list or surpassed node assumed position
    if (current == NULL || strcmp(current->getNodeName(), nodeName) != 0) {
        return false;
    }
    // node found
    GraphNode *i = firstNode;
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
    GraphNode *fromNode = getNodeByName(fromNodeName);
    if (fromNode == NULL) {
        return -1;
    }
    if (getNodeByName(toNodeName) == NULL) {
        return -2;
    }
    return fromNode->getEdges()->deleteAllEdges(toNodeName);
}

int Graph::deleteEdgesWithWeight(char *fromNodeName, char *toNodeName, int weight) {
    GraphNode *fromNode = getNodeByName(fromNodeName);
    if (fromNode == NULL) {
        return -1;
    }
    if (getNodeByName(toNodeName) == NULL) {
        return -2;
    }
    return fromNode->getEdges()->deleteEdgesWithWeight(toNodeName, weight);
}

int Graph::modifyEdge(char *fromName, char *toNodeName, int weight, int nweight) {
    GraphNode *fromNode = getNodeByName(fromName);
    if (fromNode == NULL) {
        return -1;
    }
    if (getNodeByName(toNodeName) == NULL) {
        return -2;
    }
    return fromNode->getEdges()->modifyEdge(toNodeName, weight, nweight);
}

void Graph::printReceiving(char *nodeName) const {
    GraphNode *toNode = getNodeByName(nodeName);
    if (toNode == NULL) {
        cout << " |" << nodeName << "| does not exist - abort-r;" << endl;
        return;
    }
    GraphNode *current = firstNode;
    bool printed = false;
    while (current != NULL) {
        printed = (current->getEdges()->printTransactionsTo(current->getNodeName(), nodeName)) || printed;      // prints and updates flag
        current = current->getNextNode();
    }
    if (!printed) {
        cout << " No-rec-edges |" << nodeName << "|" << endl;
    }
}

void Graph::circlefind(char *nodeName) const {
    GraphNode *node = getNodeByName(nodeName);
    if (node == NULL) {
        cout << " |" << nodeName << "| does not exist - abort-c;" << endl;
        return;
    }
    Cycle *visited = new Cycle(nodeName);
    node->checkNextForCycle(visited);


    delete visited;
}
