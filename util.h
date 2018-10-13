#ifndef OS_1_UTIL_H
#define OS_1_UTIL_H

#define ALLOW_NEGATIVE_WEIGHTS false

enum ErrorCodes {
    EC_OK,       // Success
    EC_ARG,      // Invalid command line arguments
    EC_FILE,     // Failed to open/read/create text file
    EC_MEM,      // Failed to allocate memory
    EC_UNKNOWN   // An unexpected error
};

#endif
