#include <iostream>
#include <cstring>
#include <iomanip>
#include "datastructures.h"
#include "util.h"

using namespace std;

// EdgeListNode methods:
Edge::Edge(Node *receivingNode, int weight, Edge *nextEdge) :
        receivingNode(receivingNode), weight(weight), visited(false), nextEdge(nextEdge) {}

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
        outstream << "  -" << current->getWeight() << "->|" << current->getReceivingNode()->getNodeName() << "|" << endl;
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
    // iterate until edge's assumed position
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
            cout << "Rec-edges ";
        }
        cout << "|" << fromNodeName << "|->" << current->getWeight() << "->|" << toNodeName << "|" << endl;
        *printed = true;
        current = current->getNextEdge();
    }
}


// EdgeStack methods
EdgeStack::EdgeStack(Node *startingNode) : startingNode(startingNode), headEdge(NULL) {}

EdgeStack::~EdgeStack() {
    Edge *current = headEdge, *next;
    while (current != NULL) {
        next = current->getNextEdge();
        delete current;
        current = next;
    }
}

Node *EdgeStack::getStartingNode() const {
    return startingNode;
}

Edge *EdgeStack::getHeadEdge() const {
    return headEdge;
}

void EdgeStack::push(Node *toNode, int weight) {
    try {
        headEdge = new Edge(toNode, weight, headEdge);
    } catch (bad_alloc& e) { throw; }
}

void EdgeStack::deleteLast() {
    if (headEdge == NULL) {
        cerr << "Attempted to pop() from empty EdgeStack?" << endl;
    } else {
        Edge *nextEdge = headEdge->getNextEdge();
        delete headEdge;
        headEdge = nextEdge;
    }
}

EdgeStack* EdgeStack::getReverseStack() const {
    Edge *current = this->headEdge;
    EdgeStack *reverseStack = NULL;
    try {
        reverseStack = new EdgeStack(startingNode);
        while (current != NULL) {
            reverseStack->push(current->getReceivingNode(), current->getWeight());
            current = current->getNextEdge();
        }
    } catch (bad_alloc& e) { throw; }
    return reverseStack;
}

void EdgeStack::printCycle() const {
    // reverse the stack so that the edges are printed in order
    EdgeStack *reverseStack = NULL;
    try {
        reverseStack = this->getReverseStack();
    } catch (bad_alloc& e) { throw; }
    // then traverse the ordered stack and print it according to the requested format
    cout << "|" << startingNode->getNodeName() << "|";
    Edge *current = reverseStack->getHeadEdge();
    while (current != NULL) {
        cout << "->" << current->getWeight() << "->|" << current->getReceivingNode()->getNodeName() << "|";
        current = current->getNextEdge();
    }
    delete reverseStack;
    cout << endl;
}


// Node methods:
Node::Node(char *nodeName, Node *nextNode) : visited(false), nextNode(nextNode) {
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

bool Node::getVisited() const {
    return visited;
}

void Node::setVisited(bool visited) {
    Node::visited = visited;
}

void Node::simpleCycleCheck(EdgeStack *visited, bool *foundCycle) {
    if (this->visited) {
        if (this == visited->getStartingNode()) {        // cycle found with startingNode
            if (! *foundCycle) {
                cout << "Cir-found ";
                *foundCycle = true;
            }
            visited->printCycle();
        }
        return;
    }
    Edge *currentEdge = edges->getFirstEdge();
    while (currentEdge != NULL) {
        try {
            if (! currentEdge->getReceivingNode()->getVisited()) {         // no cycle - push currentEdge and recursively continue searching
                this->setVisited(true);
                visited->push(currentEdge->getReceivingNode(), currentEdge->getWeight());
                currentEdge->getReceivingNode()->simpleCycleCheck(visited, foundCycle);
                visited->deleteLast();
                this->setVisited(false);
            } else if (currentEdge->getReceivingNode() == visited->getStartingNode()) {        // cycle found with startingNode
                visited->push(currentEdge->getReceivingNode(), currentEdge->getWeight());
                if (! *foundCycle) {
                    cout << "Cir-found ";
                    *foundCycle = true;
                }
                visited->printCycle();
                visited->deleteLast();
            }
        } catch (bad_alloc&) { throw; }
        currentEdge = currentEdge->getNextEdge();
    }
}

void Node::cyclicTransactionCheck(EdgeStack *visited, int k, bool *foundCycle) {
    Edge *currentEdge = edges->getFirstEdge();
    while (currentEdge != NULL) {
        // if weight is less than k or currentEdge has already been visited, abandon path
        if (!currentEdge->getVisited() && currentEdge->getWeight() >= k) {
            try {
                visited->push(currentEdge->getReceivingNode(), currentEdge->getWeight());
                currentEdge->setVisited(true);
                // if currentNode is the same as the starting one, circle found
                if (currentEdge->getReceivingNode() == visited->getStartingNode()) {
                    if (! *foundCycle) {
                        cout << "Cir-found ";
                        *foundCycle = true;
                    }
                    visited->printCycle();
                }
                currentEdge->getReceivingNode()->cyclicTransactionCheck(visited, k, foundCycle);
                visited->deleteLast();
                currentEdge->setVisited(false);
            } catch (bad_alloc &) { throw; }
        }
        currentEdge = currentEdge->getNextEdge();
    }
}

void Node::traceflowCheck(EdgeStack *visited, Node *toNode, int len, bool *foundTrace) {
    if (len <= 0) return;
    Edge *currentEdge = edges->getFirstEdge();
    while (currentEdge != NULL) {
        // if already visited edge, abandon path
        if (! currentEdge->getVisited()) {
            try {
                visited->push(currentEdge->getReceivingNode(), currentEdge->getWeight());
                currentEdge->setVisited(true);
                // if currentNode is toNode, trace found
                if (currentEdge->getReceivingNode() == toNode) {
                    if (!*foundTrace) {
                        cout << "Tra-found ";
                        *foundTrace = true;
                    }
                    visited->printCycle();
                }
                currentEdge->getReceivingNode()->traceflowCheck(visited, toNode, len - 1, foundTrace);
                visited->deleteLast();
                currentEdge->setVisited(false);
            } catch (bad_alloc &) { throw; }
        }
        currentEdge = currentEdge->getNextEdge();
    }
}


// Graph methods:
Graph::Graph(int bucketsnum) :  nodesnum(0), bucketsnum(bucketsnum) {
    try {
        nodeTable = new Node*[bucketsnum]();       // initialize buckets to null
    } catch (bad_alloc& e) {
        cerr << __func__ << ": " << e.what() << endl;
        throw;
    }
}

Graph::~Graph() {
    for (int i = 0; i < bucketsnum; i++) {
        Node *current = nodeTable[i], *next;
        while (current != NULL) {
            next = current->getNextNode();
            delete current;
            current = next;
        }
    }
    delete[] nodeTable;
}

void Graph::print(std::ostream& outstream) const {
    for (int i = 0; i < bucketsnum; i++) {
        Node *current = nodeTable[i];
        while (current != NULL) {
            outstream << "|" << current->getNodeName() << "|";
            current->getEdges()->print(outstream);
            current = current->getNextNode();
        }
    }
}

Node *Graph::getNodeByName(char *nodeName) const {          // on average O(1)
    unsigned long currentBucket = hashFunc(nodeName) % bucketsnum;
    Node *current = nodeTable[currentBucket];
    while (current != NULL && strcmp(current->getNodeName(), nodeName) != 0) {
        current = current->getNextNode();
    }
    return current;         // if node doesn't exist, current will be null
}

Node *Graph::insertNode(char *nodeName) {        // returns the created node
    if (getNodeByName(nodeName) != NULL) return NULL;     // node exists; return immediately
    try {
        if (nodesnum + 1 > bucketsnum * HASHING_LOAD_FACTOR) {     // rehashing needed
            rehashNodeTable();
        }
        unsigned long currentBucket = hashFunc(nodeName) % bucketsnum;
        Node *current = nodeTable[currentBucket], *prev = NULL;
        while (current != NULL) {
            prev = current;
            current = current->getNextNode();
        }
        if (prev == NULL) {         // currentBucket is empty
            nodeTable[currentBucket] = new Node(nodeName, NULL);
            nodesnum++;
            return nodeTable[currentBucket];
        } else {        // append in currentBucket after last Node
            prev->setNextNode(new Node(nodeName, NULL));
            nodesnum++;
            return prev->getNextNode();
        }
    } catch (bad_alloc&) { throw; }
}

void Graph::rehashNodeTable() {
    Node **oldNodeTable = nodeTable;
    int oldBucketsnum = bucketsnum;
    bucketsnum *= 2;        // new hash table will be double in size
    try {
        nodeTable = new Node*[bucketsnum]();       // initialize buckets to null
    } catch (bad_alloc& e) {
        cerr << __func__ << ": " << e.what() << endl;
        throw;
    }
    // transfer ever node from oldNodeTable to their appropriate position nodeTable
    for (int i = 0; i < oldBucketsnum; i++) {
        Node *current = oldNodeTable[i], *next;
        while (current != NULL) {
            next = current->getNextNode();
            insertNodeReference(current);
            current = next;
        }
    }
    delete[] oldNodeTable;
}

void Graph::insertNodeReference(Node *node) {
    node->setNextNode(NULL);
    unsigned long currentBucket = hashFunc(node->getNodeName()) % bucketsnum;
    Node *current = nodeTable[currentBucket], *prev = NULL;
    while (current != NULL) {
        prev = current;
        current = current->getNextNode();
    }
    if (prev == NULL) {         // currentBucket is empty
        nodeTable[currentBucket] = node;
        return;
    } else {        // append in currentBucket after last Node
        prev->setNextNode(node);
        return;
    }
}

void Graph::insertEdge(char *fromNodeName, char *toNodeName, int weight) {
    try {
        Node *toNode = getNodeByName(toNodeName);
        if (toNode == NULL) {       // toNode doesn't exist - create it
            toNode = insertNode(toNodeName);
        }
        Node *fromNode = getNodeByName(fromNodeName);
        if (fromNode == NULL) {       // fromNode doesn't exist - create it
            fromNode = insertNode(fromNodeName);
        }
        fromNode->getEdges()->insertEdge(toNode, weight);
    } catch (bad_alloc&) { throw; }
}

bool Graph::deleteNode(char *nodeName) {
    unsigned long currentBucket = hashFunc(nodeName) % bucketsnum;
    if (nodeTable[currentBucket] == NULL) return false;        // if list is empty
    Node *current = nodeTable[currentBucket], *prev = NULL;
    while (current != NULL && strcmp(current->getNodeName(), nodeName) != 0) {
        prev = current;
        current = current->getNextNode();
    }
    if (current == NULL) return false;      // node doesn't exist
    // node found
    for (int i = 0; i < bucketsnum; i++) {      // delete incoming edges to current Node
        Node *n = nodeTable[i];
        while (n != NULL) {
            n->getEdges()->deleteAllEdges(nodeName);
            n = n->getNextNode();
        }
    }
    if (prev == NULL) {
        nodeTable[currentBucket] = current->getNextNode();
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
        cout << "|" << nodeName << "| does not exist - abort-r;" << endl;
        return;
    }
    bool printed = false;
    for (int i = 0; i < bucketsnum; i++) {
        Node *current = nodeTable[i];
        while (current != NULL) {
            current->getEdges()->printTransactionsTo(current->getNodeName(), nodeName, &printed);
            current = current->getNextNode();
        }
    }
    if (!printed) {
        cout << "No-rec-edges " << nodeName << endl;
    }
}

void Graph::circlefind(char *nodeName) const {
    Node *node = getNodeByName(nodeName);
    if (node == NULL) {
        cout << "|" << nodeName << "| does not exist - abort-c;" << endl;
        return;
    }
    EdgeStack *visited = NULL;
    bool foundCycle = false;
    try {
        visited = new EdgeStack(node);
        node->simpleCycleCheck(visited, &foundCycle);
    } catch (bad_alloc&) { throw; }
    if (! foundCycle) {
        cout << "No-circle-found |" << nodeName << "|" << endl;
    }
    delete visited;
}

void Graph::findcircles(char *nodeName, int k) const {
    Node *node = getNodeByName(nodeName);
    if (node == NULL) {
        cout << "|" << nodeName << "| does not exist - abort-f;" << endl;
        return;
    }
    EdgeStack *visited = NULL;
    bool foundCycle = false;
    try {
        visited = new EdgeStack(node);
        node->cyclicTransactionCheck(visited, k, &foundCycle);
    } catch (bad_alloc&) { throw; }
    if (! foundCycle) {
        cout << "No-circle-found invloving |" << nodeName << "| " << k << endl;
    }
    delete visited;
}

void Graph::traceflow(char *fromNodeName, char *toNodeName, int len) const {
    Node *fromNode = getNodeByName(fromNodeName);
    if (fromNode == NULL) {
        cout << "|" << fromNodeName << "| does not exist - abort-t;" << endl;
        return;
    }
    Node *toNode = getNodeByName(toNodeName);
    if (toNode == NULL) {
        cout << "|" << toNodeName << "| does not exist - abort-t;" << endl;
        return;
    }
    EdgeStack *visited = NULL;
    bool foundTrace = false;
    try {
        visited = new EdgeStack(fromNode);
        fromNode->traceflowCheck(visited, toNode, len, &foundTrace);
    } catch (bad_alloc&) { throw; }
    if (! foundTrace) {
        cout << "No-trace from |" << fromNodeName << "| to |" << toNodeName << "|" << endl;
    }
    delete visited;
}
