#include "util.h"

/* Taken from here: http://www.cse.yorku.ca/~oz/hash.html */
unsigned long hashFunc(char *str) {
    // djb2 implementation
//    unsigned long hash = 5381;
//    int c;
//
//    while (c = *str++)
//        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
//
//    return hash;

    // sdbm implementation
    unsigned long hash = 0;
    unsigned int c;

    while (c = *str++)
        hash = c + (hash << 6) + (hash << 16) - hash;

    return hash;
}
