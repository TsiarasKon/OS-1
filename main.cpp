#include <iostream>
#include <getopt.h>
#include <cstring>
#include <cstdio>
#include "util.h"

using namespace std;

int main(int argc, char *argv[]) {
    const char argErrorMsg[] = "Invalid arguments. Please run \"$ ./mygraph -i inputfile -o outfile\"\n";
    if (argc != 5) {
        cout << argErrorMsg;
        return EC_ARG;
    }
    char *inputfile = NULL, *outputfile = NULL;
    int option;
    while ((option = getopt(argc, argv,"i:o:")) != -1) {
        switch (option) {
            case 'i' :
                try {
                    inputfile = new char[strlen(optarg) + 1];
                } catch (bad_alloc& error) {
                    cerr << "Error: " << error.what() << endl;
                    delete[] outputfile;
                    return EC_MEM;
                }
                strcpy(inputfile, optarg);
                break;
            case 'o' :
                try {
                    outputfile = new char[strlen(optarg) + 1];
                } catch (bad_alloc& error) {
                    cerr << "Error: " << error.what() << endl;
                    delete[] inputfile;
                    return EC_MEM;
                }
                strcpy(outputfile, optarg);
                break;
            default:
                cout << argErrorMsg;
                return EC_ARG;
        }
    }
    if (inputfile == NULL || outputfile == NULL) {
        cout << argErrorMsg;
        // In case one of them managed to be allocated:
        delete[] inputfile;
        delete[] outputfile;
        return EC_ARG;
    }

    // TODO: Load graph from inputfile

    size_t bufsize;
    char *buffer = NULL, *bufferptr = NULL, *command;
    cout << "Type a command:" << endl;
    while (getline(&buffer, &bufsize, stdin) != -1) {           // effectively an infinite loop
        // Until "e(xit)" is given, read current line and attempt to execute it as a command
        bufferptr = buffer;
        // TODO: no \r - we're on linux!
        strtok(buffer, "\r\n");     // remove trailing newline character
        command = strtok(buffer, " ");
        if (!strcmp(command, "i")) {

        } else if (!strcmp(command, "n")) {

        } else if (!strcmp(command, "d")) {

        } else if (!strcmp(command, "l")) {

        } else if (!strcmp(command, "m")) {

        } else if (!strcmp(command, "r")) {

        } else if (!strcmp(command, "c")) {

        } else if (!strcmp(command, "f")) {

        } else if (!strcmp(command, "t")) {

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
            cout << " 'f Ni k' - print all simple cycles in which Ni appears and each edge has at least weight k" << endl;
            cout << " 't Ni Nj l' - print all possible transaction flows from Ni to Nj traversing at most l edges" << endl;
            cout << " 'e' - save the graph to a file and exit the program" << endl;
        } else if (!strcmp(command, "e")) {
            // TODO: Save graph to outputfile
            // TODO: Free all the memory!
            delete[] inputfile;
            delete[] outputfile;
            delete[] bufferptr;
            cout << "Program completed successfully." << endl;
            return EC_OK;
        } else {
            cout << "Unknown command - Type \"h\" for a list of available commands" << endl;
        }
        buffer = bufferptr;     // used to avoid leaks due to strtok
        cout << "Type a command:" << endl;
    }
    // Should never get here
    cerr << "Failed to read command" << endl;
    return EC_MEM;
}
