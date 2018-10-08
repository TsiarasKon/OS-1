#include <iostream>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include "util.h"
#include "lists.h"

using namespace std;

void interface(Graph *graph) {
    size_t bufsize;
    char *buffer = NULL, *bufferptr = NULL;
    char *Ni, *Nj, *weight;
    char *command;
    const char cmdErrorMsg[] = " Invalid command format - Type \"h\" for the correct format\n";
    cout << endl << "Type a command: ";
    try {
        while (getline(&buffer, &bufsize, stdin) != -1) {           // effectively an infinite loop
            // Until "e(xit)" is given, read current line and attempt to execute it as a command
            bufferptr = buffer;
            // TODO: no \r - we're on linux!
            strtok(buffer, "\r\n");     // remove trailing newline character
            command = strtok(buffer, " ");
            if (!strcmp(command, "i")) {
                Ni = strtok(NULL, " ");
                if (Ni == NULL) {
                    cout << cmdErrorMsg;
                } else {
                    graph->insertNode(Ni);
                    if (graph->insertNode(Ni) != NULL) {
                        cout << " Inserted |" << Ni << "|" << endl;
                    } else {
                        cout << " Node |" << Ni << "| already exists;" << endl;
                    }
                }
            } else if (!strcmp(command, "n")) {
                Ni = strtok(NULL, " ");
                Nj = strtok(NULL, " ");
                weight = strtok(NULL, " ");
                if (Ni == NULL || Nj == NULL || weight == NULL) {
                    cout << cmdErrorMsg;
                } else {
                    graph->insertEdge(Ni, Nj, atoi(weight));
                    cout << " Inserted |" << Ni << "|--" << weight << "-->|" << Nj << "|" << endl;
                }
            } else if (!strcmp(command, "d")) {
                Ni = strtok(NULL, " ");
                if (Ni == NULL) {
                    cout << cmdErrorMsg;
                } else {
                    if (graph->deleteNode(Ni)) {
                        cout << " Deleted |" << Ni << "|" << endl;
                    } else {
                        cout << " Node |" << Ni << "| does not exist - abort-d;" << endl;
                    }
                }
            } else if (!strcmp(command, "l")) {
                Ni = strtok(NULL, " ");
                Nj = strtok(NULL, " ");
                weight = strtok(NULL, " ");
                if (Ni == NULL || Nj == NULL) {
                    cout << cmdErrorMsg;
                } else {
                    int res;
                    if (weight == NULL) {
                        res = graph->deleteAllEdges(Ni, Nj);
                        if (res == 0) {
                            cout << " Del-all-vertices |" << Ni << "|--->|" << Nj << "|" << endl;
                        } else if (res == -3) {     // can only happen if weight was specified
                            cout << " No |" << Ni << "|--->|" << Nj << "| exists - abort-l;" << endl;
                        }
                    } else {
                        res = graph->deleteEdgesWithWeight(Ni, Nj, atoi(weight));
                        if (res == 0) {
                            cout << " Del-vertex |" << Ni << "|--" << weight << "-->|" << Nj << "|" << endl;
                        } else if (res == -3) {     // can only happen if weight was specified
                            cout << " |" << Ni << "|--" << weight << "-->|" << Nj << "| does not exist - abort-l;" << endl;
                        }
                    }
                    if (res == -1) {
                        cout << " Node |" << Ni << "| does not exist - abort-l;" << endl;
                    } else if (res == -2) {
                        cout << " Node |" << Nj << "| does not exist - abort-l;" << endl;
                    }
                }
            } else if (!strcmp(command, "m")) {
                Ni = strtok(NULL, " ");
                Nj = strtok(NULL, " ");
                weight = strtok(NULL, " ");
                char *nweight = strtok(NULL, " ");
                if (Ni == NULL || Nj == NULL || weight == NULL || nweight == NULL) {
                    cout << cmdErrorMsg;
                } else {
                    int res = graph->modifyEdge(Ni, Nj, atoi(weight), atoi(nweight));
                    if (res == 0) {
                        cout << " Mod-vertex |" << Ni << "|--" << nweight << "-->|" << Nj << "|" << endl;
                    } else if (res == -1) {
                        cout << " Node |" << Ni << "| does not exist - abort-m;" << endl;
                    } else if (res == -2) {
                        cout << " Node |" << Nj << "| does not exist - abort-m;" << endl;
                    } else if (res == -3) {
                        cout << " |" << Ni << "|--" << weight << "-->|" << Nj << "| does not exist - abort-m;" << endl;
                    }
                }
            } else if (!strcmp(command, "r")) {
                Ni = strtok(NULL, " ");
                if (Ni == NULL) {
                    cout << cmdErrorMsg;
                } else {
                    graph->printReceiving(Ni);
                }
            } else if (!strcmp(command, "c")) {
                Ni = strtok(NULL, " ");
                if (Ni == NULL) {
                    cout << cmdErrorMsg;
                } else {
                    graph->circlefind(Ni);
                }
            } else if (!strcmp(command, "f")) {
                Ni = strtok(NULL, " ");
                weight = strtok(NULL, " ");
                if (Ni == NULL || weight == NULL) {
                    cout << cmdErrorMsg;
                } else {
//                    graph->findcircles(Ni, weight);
                }
            } else if (!strcmp(command, "t")) {
                Ni = strtok(NULL, " ");
                Nj = strtok(NULL, " ");
                char *len = strtok(NULL, " ");
                if (Ni == NULL || Nj == NULL || len == NULL) {
                    cout << cmdErrorMsg;
                } else {
//                    graph->traceflow(Ni, Nj, len);
                }
            } else if (!strcmp(command, "p")) {         // bonus: p(rint) command
                graph->print(cout);
            } else if (!strcmp(command, "h")) {         // bouns: h(elp) command
                cout << " Available commands (use without quotes):" << endl;
                cout << "  'i Ni' - insert a new node Ni" << endl;
                cout << "  'n Ni Nj w' - insert a new edge from Ni to Nj with weight w" << endl;
                cout << "  'd Ni' - delete node Ni (as well as all its edges)" << endl;
                cout << "  'l Ni Nj [w]' - delete an edge from Ni to Nj with weight w;" << endl
                     << "  \tif w is not provided then delete all such edges regardless of weight" << endl;
                cout << "  'm Ni Nj w nw' - modify an edge from Ni to Nj with weight w to have weight nw" << endl;
                cout << "  'r Ni' - print all incoming transactions to node Ni" << endl;
                cout << "  'c Ni' - print all simple cycles in which Ni appears" << endl;
                cout << "  'f Ni k' - print all simple cycles in which Ni appears and each edge has at least weight k"
                     << endl;
                cout << "  't Ni Nj l' - print all possible transaction flows from Ni to Nj traversing at most l edges"
                     << endl;
                cout << "  'p' - pretty print the current graph" << endl;
                cout << "  'h' - print the list you're seeing right now" << endl;
                cout << "  'e' - save the graph to a file and exit the program" << endl;
            } else if (!strcmp(command, "e")) {
                cout << endl;
                return;
            } else {
                cout << " Unknown command - Type \"h\" for a list of available commands;" << endl;
            }
            buffer = bufferptr;     // used to avoid memory leaks due to strtok()
            cout << endl << "Type a command: ";
        }
    } catch (bad_alloc&) { throw; }
    // Should never get here
    throw bad_alloc();
}
