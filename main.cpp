#include <iostream>
#include <getopt.h>
#include <cstring>
#include <cstdio>
#include <fstream>
#include <cstdlib>
#include "util.h"
#include "lists.h"

using namespace std;

void cleanup(char **inputfilename, char **outputfilename, char **bufferptr, NodeList **graph) {
    delete[] *inputfilename;
    delete[] *outputfilename;
    delete[] *bufferptr;
    *inputfilename = *outputfilename = *bufferptr = NULL;
    delete *graph;
    *graph = NULL;
}

int main(int argc, char *argv[]) {
    const char argErrorMsg[] = "Invalid arguments. Please run \"$ ./mygraph [-i inputfile] [-o outfile]\"\n";
    if ((argc != 1 && argc != 3 && argc != 5) || (argc >= 2 && argv[1][0] != '-') || (argc >= 4 && argv[3][0] != '-')) {
        cout << argErrorMsg;
        return EC_ARG;
    }
    char *inputfilename = NULL, *outputfilename = NULL;
    int option;
    while ((option = getopt(argc, argv,"i:o:")) != -1) {
        switch (option) {
            case 'i' :
                try {
                    inputfilename = new char[strlen(optarg) + 1];
                } catch (bad_alloc& e) {
                    cerr << __func__ << ": " << e.what() << endl;
                    delete[] outputfilename;
                    return EC_MEM;
                }
                strcpy(inputfilename, optarg);
                break;
            case 'o' :
                try {
                    outputfilename = new char[strlen(optarg) + 1];
                } catch (bad_alloc& e) {
                    cerr << __func__ << ": " << e.what() << endl;
                    delete[] inputfilename;
                    return EC_MEM;
                }
                strcpy(outputfilename, optarg);
                break;
            default:
                cout << argErrorMsg;
                return EC_ARG;
        }
    }

    NodeList *graph;
    try {
        graph = new NodeList();
    } catch (bad_alloc&) {
        cleanup(&inputfilename, &outputfilename, NULL, &graph);
        return EC_MEM;
    }

    // Load graph from inputfile
    size_t bufsize;
    char *buffer = NULL, *bufferptr = NULL;
    char *Ni, *Nj, *weight;
    if (inputfilename != NULL) {
        FILE *inputfp;
        inputfp = fopen(inputfilename, "r");
        if (inputfp == NULL) {
            cout << "Failed to open inputfile." << endl;
            cleanup(&inputfilename, &outputfilename, NULL, &graph);
            return EC_FILE;
        }
        try {
            while (getline(&buffer, &bufsize, inputfp) != -1) {
                if (!strcmp(buffer, "") || !strcmp(buffer, "\n")) continue;       // skip empty lines
                bufferptr = buffer;
                Ni = strtok(buffer, " \t");
                Nj = strtok(NULL, " \t");
                weight = strtok(NULL, " \t");
                if (Ni == NULL || Nj == NULL || weight == NULL) {
                    cerr << "Invalid inputfile format." << endl;
                    cleanup(&inputfilename, &outputfilename, &bufferptr, &graph);
                    return EC_FILE;
                }
                graph->insertEdge(Ni, Nj, atoi(weight));
                buffer = bufferptr;     // used to avoid memory leaks due to strtok()
            }
        } catch (bad_alloc &e) {
            cerr << "Encountered an error while reading inputfile." << endl;
            cleanup(&inputfilename, &outputfilename, &bufferptr, &graph);
            return EC_MEM;
        }
        if (feof(inputfp)) {
            cout << "Loaded graph from inputfile successfully." << endl;
        } else {
            cerr << "Encountered an error while reading inputfile." << endl;
            cleanup(&inputfilename, &outputfilename, NULL, &graph);
            return EC_MEM;
        }
        fclose(inputfp);
    }

    char *command;
    const char cmdErrorMsg[] = "Invalid command format - Type \"h\" for the correct format\n";
    cout << "Type a command:" << endl;
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
                    // TODO: size comparison instead?
//                    if (!graph->insertNode(Ni)) {
//                        cout << "Insertion failed: A node named \"" << Ni << "\" already exists!" << endl;
//                    }
                }
            } else if (!strcmp(command, "n")) {
                Ni = strtok(NULL, " ");
                Nj = strtok(NULL, " ");
                weight = strtok(NULL, " ");
                if (Ni == NULL || Nj == NULL || weight == NULL) {
                    cout << cmdErrorMsg;
                } else {
                    graph->insertEdge(Ni, Nj, atoi(weight));
                }
            } else if (!strcmp(command, "d")) {
                Ni = strtok(NULL, " ");
                if (Ni == NULL) {
                    cout << cmdErrorMsg;
                } else {
                    if (!graph->deleteNode(Ni)) {
                        cout << "Deletion failed: Node not found!" << endl;
                    }
                }
            } else if (!strcmp(command, "l")) {
                Ni = strtok(NULL, " ");
                Nj = strtok(NULL, " ");
                weight = strtok(NULL, " ");
                if (Ni == NULL || Nj == NULL) {
                    cout << cmdErrorMsg;
                } else {
                    if (weight == NULL) {
                        graph->deleteAllEdges(Ni, Nj);
                    } else {
                        graph->deleteEdgesWithWeight(Ni, Nj, atoi(weight));
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
                     graph->modifyEdge(Ni, Nj, atoi(weight), atoi(nweight));
                }
            } else if (!strcmp(command, "r")) {

            } else if (!strcmp(command, "c")) {

            } else if (!strcmp(command, "f")) {

            } else if (!strcmp(command, "t")) {

            } else if (!strcmp(command, "p")) {         // bonus: p(rint) command
                graph->print();
            } else if (!strcmp(command, "h")) {         // bouns: h(elp) command
                cout << "Available commands (use without quotes):" << endl;
                cout << " 'i Ni' - insert a new node Ni" << endl;
                cout << " 'n Ni Nj w' - insert a new edge from Ni to Nj with weight w" << endl;
                cout << " 'd Ni' - delete node Ni (as well as all its edges)" << endl;
                cout << " 'l Ni Nj [w]' - delete an edge from Ni to Nj with weight w;" << endl
                     << " \tif w is not provided then delete all such edges regardless of weight" << endl;
                cout << " 'm Ni Nj w nw' - modify an edge from Ni to Nj with weight w to have weight nw" << endl;
                cout << " 'r Ni' - print all incoming transactions to node Ni" << endl;
                cout << " 'c Ni' - print all simple cycles in which Ni appears" << endl;
                cout << " 'f Ni k' - print all simple cycles in which Ni appears and each edge has at least weight k"
                     << endl;
                cout << " 't Ni Nj l' - print all possible transaction flows from Ni to Nj traversing at most l edges"
                     << endl;
                cout << " 'p' - pretty print the current graph" << endl;
                cout << " 'h' - print the list you're seeing right now" << endl;
                cout << " 'e' - save the graph to a file and exit the program" << endl;
            } else if (!strcmp(command, "e")) {
                // TODO: Save graph to outputfile
                cleanup(&inputfilename, &outputfilename, &bufferptr, &graph);
                cout << "Program completed successfully." << endl;
                return EC_OK;
            } else {
                cout << "Unknown command - Type \"h\" for a list of available commands" << endl;
            }
            buffer = bufferptr;     // used to avoid memory leaks due to strtok()
            cout << "Type a command:" << endl;
        }
    } catch (bad_alloc&) {
        cleanup(&inputfilename, &outputfilename, &bufferptr, &graph);
        return EC_MEM;
    }
    // Should never get here
    cleanup(&inputfilename, &outputfilename, &bufferptr, &graph);
    return EC_MEM;
}