#include <iostream>
#include <getopt.h>
#include <cstring>
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
                inputfile = new char[strlen(optarg) + 1];
                strcpy(inputfile, optarg);
                break;
            case 'o' :
                outputfile = new char[strlen(optarg) + 1];
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

    cout << inputfile << '|' << outputfile << endl;


    return 0;
}