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
    // if (strcmp(res.argv[0],"cd") == 0){
    //     char dir[100];
    //     printf("Current Dir: %s\n", getcwd(dir, 100));
    // }
}

void printError() {
    char error_message[30] = "An error has occurred\n";
    write(STDERR_FILENO, error_message, strlen(error_message)); 
}
