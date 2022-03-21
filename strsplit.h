#ifndef STRSPLIT_GUARD__H
#define STRSPLIT_GUARD__H

#define MAX_STR_LEN 128
#define MAX_STR_COUNT 8

typedef struct _StrSplit {
    char (*strings) [MAX_STR_LEN]; // pointer to the split string
    int count; // number of split strings
    void (* free)(struct _StrSplit); // pointer to the method of releasing memory
} StrSplit;

// split string by another string
// return NULL if there is no match
StrSplit str_split(const char* str, const char* delim);

#endif
