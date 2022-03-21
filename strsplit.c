#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "strsplit.h"

static void str_free();

StrSplit str_split(const char* str, const char* delim) {
    char (*strs) [MAX_STR_LEN] = (char (*) [MAX_STR_LEN]) malloc(sizeof(char) * MAX_STR_COUNT * MAX_STR_LEN);
    int length = 0;
    char* dump = strdup(str);
    char* token = strtok(dump, delim);
    while (token != NULL || length > MAX_STR_COUNT) {
        strcpy(strs[length++], token);
        token = strtok(NULL, delim);
    }
    
    StrSplit result = {strs, length, str_free};
    return result;
}

static void str_free(StrSplit strSplit) {
    free(strSplit.strings);
}
