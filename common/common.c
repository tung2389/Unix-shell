#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

/*
allocate memory for a string to copy the input string
input: target string to copy
output: return string allocated in heap 
*/
char * mallocStr(char *str) {
    /*
    Edge cases:
        - str is NULL
    */
    if (str == NULL) {
        return NULL;
    }
    int len = strlen(str);
    char *p = malloc(len + 1);
    p[len] = '\0';
    strcpy(p, str);
    return p;
}

/*
strip the string(user input) including leading and trail space.
input: a string
output: a stripped string
*/
char * strip(char *str) { 
    /*
    Remove leading and trailing whitespace
    Edge cases:
        - str is NULL
    */
    if (str == NULL) {
        return NULL;
    }

    // Trim leading space
    char * begin = str;
    while (begin != NULL && isspace(*begin)) {
        begin += 1;
    }

    // Trim trailing space
    char * end = str + strlen(str) - 1;
    while (end >= begin && isspace(*end)) {
        end -= 1;
    }
    *(end + 1) = '\0';

    return begin;
}

/*
check a cmd is build-in or non build-in 
input: cmd
output: true if build-in false otherwise
*/
bool isBuiltinCmd(char *str) {
    char *builtinCmds[] = {"exit", "cd", "path"};
    int cntBuiltinCmds = 3;
    for (int i = 0; i < cntBuiltinCmds; i++) {
        if (strcmp(str, builtinCmds[i]) == 0) {
            return true;
        }
    }
    return false;
}

/*
Free an array of cstring created by malloc
input: array of cstring and its len
*/
void freeArrStr(char **arr, int len) {
    /*
    Edge cases:
        - arr = NULL
        - len = 0
    */
    if (arr == NULL) {
        return;
    }
    for (int i = 0; i < len; i++) {
        free(arr[i]);
    }
    free(arr);
}