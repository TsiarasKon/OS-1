#include "util.h"

/* Taken from here: http://www.cse.yorku.ca/~oz/hash.html
 * and here: https://github.com/haipome/fnv */
unsigned long hashFunc(char *str) {

    /// djb2
//    unsigned long hash = 5381;
//    char c;
//
//    while ((c = *str++))
//        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
//
//    return hash;


    /// sdbm
    unsigned long hash = 0;
    char c;

    while ((c = *str++))
        hash = c + (hash << 6) + (hash << 16) - hash;

    return hash;


    /// fnv-1_32
//    unsigned long hval = 16777619;
//    unsigned char *s = (unsigned char *) str;	/* unsigned string */
//
//    while (*s) {
//
//        /* multiply by the 32 bit FNV magic prime mod 2^32 */
//        hval += (hval<<1) + (hval<<4) + (hval<<7) + (hval<<8) + (hval<<24);
//
//        /* xor the bottom with the current octet */
//        hval ^= (unsigned long) *s++;
//    }
//
//    return hval;


    /// fnv-1a_64
//    unsigned long hval = 1099511628211;
//    unsigned char *s = (unsigned char *) str;	/* unsigned string */
//
//    while (*s) {
//
//        /* xor the bottom with the current octet */
//        hval ^= (unsigned long) *s++;
//
//        /* multiply by the 64 bit FNV magic prime mod 2^64 */
//        hval += (hval << 1) + (hval << 4) + (hval << 5) +
//                (hval << 7) + (hval << 8) + (hval << 40);
//    }
//
//    return hval;

}
