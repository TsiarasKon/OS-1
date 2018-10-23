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

void Cycle::push(Node *toNode, int weight) {
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
        nextNode(nextNode), visited(false) {
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


bool Node::simpleCycleCheck(Cycle *visited) {
    bool foundCycle = false;
    Edge *currentEdge = edges->getFirstEdge();
    while (currentEdge != NULL) {
        try {
            if (! currentEdge->getReceivingNode()->getVisited()) {         // no cycle - check next
                this->setVisited(true);
                visited->push(currentEdge->getReceivingNode(),
                              currentEdge->getWeight());
                foundCycle = currentEdge->getReceivingNode()->simpleCycleCheck(
                        visited) || foundCycle;
                visited->deleteLast();
                this->setVisited(false);
            } else if (currentEdge->getReceivingNode() == visited->getStartingNode()) {        // cycle found with startingNode
                visited->push(currentEdge->getReceivingNode(),
                              currentEdge->getWeight());
                if (! foundCycle) {     // TODO: rec carry foundCycle
                    cout << " Cir-found ";
                    foundCycle = true;
                }
                visited->printCycle();
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
        // if weight is less than k or currentEdge has already been visited, abandon path
        if (!currentEdge->getVisited() && currentEdge->getWeight() >= k) {
            try {
                visited->push(currentEdge->getReceivingNode(), currentEdge->getWeight());
                currentEdge->setVisited(true);
                // if currentNode is the same as the starting one, circle found
                if (currentEdge->getReceivingNode() == visited->getStartingNode()) {
                    cout << " Cir-found ";
                    visited->printCycle();
                    foundCycle = true;
                }
                foundCycle = currentEdge->getReceivingNode()->cyclicTransactionCheck(visited, k) || foundCycle;
                visited->deleteLast();
                currentEdge->setVisited(false);
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
                visited->push(currentEdge->getReceivingNode(),
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
Graph::Graph(int bucketsnum) :  nodesnum(0), bucketsnum(bucketsnum) {
    collisions = 0;     // TODO del
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
        cout << i << endl;
        Node *current = nodeTable[i];
        while (current != NULL) {
            outstream << " |" << current->getNodeName() << "|";
            current->getEdges()->print(outstream);
            current = current->getNextNode();
        }
    }
    cout << endl << "Nodes: " << nodesnum << endl;       // TODO del
    cout << endl << "Collisions: " << collisions << endl;       // TODO del
}

Node *Graph::getNodeByName(char *nodeName) const {
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
            cout << " >> Inserted dup at bucket: " << currentBucket << endl;
            nodesnum++;
            collisions++;   // TODO del
            return prev->getNextNode();
        }
    } catch (bad_alloc&) { throw; }
}

void Graph::rehashNodeTable() {
    cout << "!! REHASHING !!" << endl;
    cout << " currentNodes: " << nodesnum << endl;
    cout << " currentBucketsnum: " << bucketsnum << endl;
    collisions = 0;   // TODO del
    Node **oldNodeTable = nodeTable;
    int oldBucketsnum = bucketsnum;
    bucketsnum *= 2;
    try {
        nodeTable = new Node *[bucketsnum]();       // initialize buckets to null
    } catch (bad_alloc& e) {
        cerr << __func__ << ": " << e.what() << endl;
        throw;
    }
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
        cout << " !>> Rehashing dup at bucket: " << currentBucket << endl;
        collisions++;   // TODO del
        return;
    }
}

void Graph::insertEdge(char *fromNodeName, char *toNodeName, int weight) {
    try {
        Node *toNode = insertNode(toNodeName);
        if (toNode == NULL) {       // toNode already existed
            toNode = getNodeByName(toNodeName);
        }
        Node *fromNode = insertNode(fromNodeName);
        if (fromNode == NULL) {       // toNode already existed
            fromNode = getNodeByName(fromNodeName);
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
        cout << " |" << nodeName << "| does not exist - abort-r;" << endl;
        return;
    }
    bool printed = false;
    for (int i = 0; i < bucketsnum; i++) {
        Node *current = nodeTable[i];
        while (current != NULL) {
            current->getEdges()->printTransactionsTo(current->getNodeName(), nodeName,
                                                     &printed);      // prints and updates flag
            current = current->getNextNode();
        }
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
