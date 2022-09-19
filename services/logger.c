#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "parser.h"

/*
log out user cmd information (args of cmd and redirection file if there exists)
input: user cmd and a Parser obj
output: related information
*/
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

/*
Error message 
*/
void printError() {
    char error_message[30] = "An error has occurred\n";
    write(STDERR_FILENO, error_message, strlen(error_message)); 
}
