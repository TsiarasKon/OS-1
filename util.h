#ifndef OS_1_UTIL_H
#define OS_1_UTIL_H

enum ErrorCodes {
    EC_OK,       // Success
    EC_ARG,      // Invalid command line arguments
    EC_FILE,     // Failed to open/create text file
    EC_MEM,      // Failed to allocate memory
    EC_UNKNOWN   // An unexpected error
};

#endif
