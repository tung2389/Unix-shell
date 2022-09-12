#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "executor.h"
#include "logger.h"

void executeCmd(int argc, char **argv, char *redirection) {
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
        if (argc == 2) {
            printf("\ntest");
        }
        else {
            printError();
        }
    }
}