#include <iostream>
#include <getopt.h>
#include <cstring>
#include <cstdio>
#include <fstream>
#include <cstdlib>
#include "util.h"
#include "datastructures.h"

using namespace std;

void interface(Graph *graph);

void cleanup(FILE* inputfp, char **inputfilename, char **outputfilename, char **bufferptr, Graph **graph) {
    if (inputfp != NULL) fclose(inputfp);
    if (*inputfilename != NULL) delete[] *inputfilename;
    if (*outputfilename != NULL) delete[] *outputfilename;
    if (*bufferptr != NULL) free(*bufferptr);
    *inputfilename = *outputfilename = *bufferptr = NULL;
    if (*graph != NULL) delete *graph;
    *graph = NULL;
}

int main(int argc, char *argv[]) {
    const char argErrorMsg[] = " Invalid arguments. Please run \"$ ./mygraph [-i inputfile] [-o outfile]\"\n";
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

    Graph *graph = NULL;
    try {
        graph = new Graph(INITIAL_BUCKETS_NUM);
    } catch (bad_alloc&) {
        cleanup(NULL, &inputfilename, &outputfilename, NULL, &graph);
        return EC_MEM;
    }

    // Load graph from inputfile
    size_t bufsize;
    char *buffer = NULL, *bufferptr = NULL;
    char *Ni, *Nj, *weightStr, *strtolEndptr;
    int weight;
    if (inputfilename != NULL) {
        cout << "Loading graph from inputfile ..." << endl;
        FILE *inputfp;
        inputfp = fopen(inputfilename, "r");
        if (inputfp == NULL) {
            cout << "Failed to open inputfile." << endl;
            cleanup(NULL, &inputfilename, &outputfilename, NULL, &graph);
            return EC_FILE;
        }
        try {
            while (getline(&buffer, &bufsize, inputfp) != -1) {
                if (!strcmp(buffer, "") || !strcmp(buffer, "\n") || !strcmp(buffer, "\r\n")) continue;       // skip empty lines
                bufferptr = buffer;
                strtok(buffer, "\r\n");
                Ni = strtok(buffer, " \t");
                Nj = strtok(NULL, " \t");
                weightStr = strtok(NULL, " \t");
                if (Ni == NULL || Nj == NULL || weightStr == NULL) {
                    cout << "Invalid inputfile format." << endl;
                    cleanup(inputfp, &inputfilename, &outputfilename, &bufferptr, &graph);
                    return EC_FILE;
                }
                weight = (int) strtol(weightStr, &strtolEndptr, 10);
                if (*strtolEndptr != '\0' || (!ALLOW_NEGATIVE_WEIGHTS && weight < 0)) {
                    cout << "Invalid inputfile format - Weight must be a non-negative integer." << endl;
                    cleanup(inputfp, &inputfilename, &outputfilename, &bufferptr, &graph);
                    return EC_FILE;
                } else {
                    graph->insertEdge(Ni, Nj, weight);
                }
                buffer = bufferptr;     // used to avoid memory leaks due to strtok()
            }
        } catch (bad_alloc &e) {
            cerr << "Encountered an error while reading inputfile." << endl;
            cleanup(inputfp, &inputfilename, &outputfilename, &bufferptr, &graph);
            return EC_MEM;
        }
        if (feof(inputfp)) {
            cout << "Loaded graph from inputfile successfully!" << endl << endl;
        } else {
            cerr << "Encountered an error while reading inputfile." << endl;
            cleanup(inputfp, &inputfilename, &outputfilename, NULL, &graph);
            return EC_MEM;
        }
        fclose(inputfp);
    }

    try {
        interface(graph);     // <-- This is the main program, right here!
    } catch (bad_alloc&) {
        cleanup(NULL, &inputfilename, &outputfilename, &bufferptr, &graph);
        return EC_MEM;
    }

    if (outputfilename != NULL) {
        cout << "Saving graph to outputfile ..." << endl;
        ofstream outputfile;
        outputfile.open(outputfilename);
        if (outputfile.is_open()) {
            graph->print(outputfile);
        } else {
            cout << "Failed to open outputfile." << endl;
            cleanup(NULL, &inputfilename, &outputfilename, &bufferptr, &graph);
            return EC_FILE;
        }
        outputfile.close();
        cout << "Saved graph to outputfile successfully!" << endl << endl;
    }

    cleanup(NULL, &inputfilename, &outputfilename, &bufferptr, &graph);
    cout << "Program completed successfully!" << endl;
    return EC_OK;
}