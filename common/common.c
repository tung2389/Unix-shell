#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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