#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "executor.h"
#include "logger.h"
#include "../common/common.h"

/*
redirect redinginput from stdout to target file
input: file to write output to
output: file that's already written on
*/
void redirect(char *redirection) {
    /*
    Edge cases:
        - redirection is NULL (no need for redirection)
    */
    if (redirection == NULL) {
        return;
    }
    FILE * out = NULL; 
    out = fopen(redirection, "w");
    if (out == NULL) {
        return; 
    }
    int outFd = fileno(out);
    // Point stduout and stderr to the file description of target file.
    dup2(outFd, STDOUT_FILENO);
    dup2(outFd, STDERR_FILENO);
    fclose(out);
}

/*
execute build-in (exit, cmd, path) and non build-in commands
input: args user input, num of args, redirection file, paths included, num of paths
output: no return, execute user cmd normally. If error, print error
*/
void executeCmd(int argc, char **argv, char *redirection, int *pathCnt, char ***paths, int *cntProc) {
    /*
    Edge cases:
        - argc <= 0
    Logic:
        - validate each command before running 
    */
    if (argc <= 0) {
        return;
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
        char *path = argv[1];
        if (argc == 2) {
            if (chdir(path) == -1){
                printError();
            }
        }
        else {
            printError();
        }
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
        int pid = fork();
        if (pid < 0){
            printError();
            // Fork failed, decrement the number of process. 
            *(cntProc) -= 1;
            exit(1);
        }
        else if (pid == 0){
            /*
            Always call exit if failed to exec, to prevent the child process from messing up with file
            descriptors of parent process 
            */
            if (redirection != NULL){
                redirect(redirection);
            }
            //config path
            char *path;
            //execute path
            for (int i = 0; i < *(pathCnt); i++) {
                char *path = (*paths)[i];
                int fullPathLen = strlen(path) + strlen(cmd) + 1;
                char fullPath[fullPathLen + 1];
                snprintf(fullPath, fullPathLen + 1, "%s/%s", path, cmd);
                fullPath[fullPathLen] = '\0';
                if (access(fullPath, X_OK) == 0){
                    execv(fullPath, argv);
                    // If exec return, that means an error has occured
                    printError();
                    exit(1);
                }
            }
            // No path matches, this is an error.
            printError();
            exit(1);
        }
    }
}


