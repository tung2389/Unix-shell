#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "executor.h"
#include "logger.h"
#include "../common/common.h"

void executeCmd(int argc, char **argv, char *redirection, int *pathCnt, char ***paths) {
    /*
    Edge cases:
        - argc <= 0
    Logic:
        - validate each command before running 
    */
    if (argc <= 0) {
        exit(1);
    }
    char *cmd = argv[0];
    if (strcmp(cmd, "exit") == 0) {
        if (argc > 1) {
            printError();
            return;
        }
        exit(0);
    }
    else if (strcmp(cmd, "cd") == 0) {
        // TODO

        // if (argc == 2) {
        //     printf("\ntest");
        // }
        // else {
        //     printError();
        // }
    }
    else if (strcmp(cmd, "path") == 0) {
        /*
        Logic:
            - free old memory that store paths.
            - realloc the paths pointer to match the size of new array of paths and assign new paths.
        */
        for (int i = 0; i < (*pathCnt); i++) {
            free((*paths)[i]);
        }
        (*pathCnt) = argc - 1; 
        (*paths) = realloc((*paths), sizeof(char *) * (*pathCnt));
        for (int i = 0; i < (*pathCnt); i++) {
            (*paths)[i] = mallocStr(argv[i + 1]);
        }
    }
    // Execute non built-in commands
    else {

    }
}

FILE * redirect(char *redirection) {
    /*
    Edge cases:
        - redirection is NULL (no need for redirection)
    */
    if (redirection == NULL) {
        return NULL;
    }
    FILE * out = NULL; 
    out = fopen(redirection, "w");
    if (out == NULL) {
        return NULL;
    }
    int outFd = fileno(out);
    dup2(outFd, STDOUT_FILENO);
    dup2(outFd, STDERR_FILENO);
    return out;
}