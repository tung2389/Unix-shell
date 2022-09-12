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

void printError() {
    char error_message[30] = "An error has occurred\n";
    write(STDERR_FILENO, error_message, strlen(error_message)); 
}