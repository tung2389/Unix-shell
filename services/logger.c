#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "parser.h"

void logCmdInfo(char *fullCmd, ParserResult res) {
    printf("full cmd: %s\n", fullCmd);
    printf("executable: %s\n", res.argv[0]);
    if (res.argc > 1) {
        printf("arguments: ");
        for (int i = 1; i < res.argc; i++) {
            printf("%s ", res.argv[i]);
        }
        printf("\n");
    }
    if (res.redirection != NULL) {
        printf("redirected to: %s\n", res.redirection);
    }
}

void printError() {
    char error_message[30] = "An error has occurred\n";
    write(STDERR_FILENO, error_message, strlen(error_message)); 
}
