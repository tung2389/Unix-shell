/*
1. Running mode:
- interactive mode: keep prompting users
- batch mode: reading commands from file and execute
- invoke with invalid parameter: exit(1)

2. Built-in commands:
- exit: basically call exit
- cd: chdir() sys call
- path: overwrite paths state of the shell

3. Redirection:
- Only non-built-in command 

4. Parallel commands:
- basically count the number of commands, then wait() until all commands complete.

5. Error handling:
- print a message when syntax error
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>
#include "dush.h"

int main(int argc, char *argv[]) {
    setbuf(stdout, NULL);
    enum Mode {INTERACTIVE, BATCH};
    enum Mode mode;
    FILE * input = NULL;

    if (argc == 1) {
        mode = INTERACTIVE;
        input = stdin;
    }
    else if (argc == 2) {
        mode = BATCH;
        char *file = argv[1];
        input = fopen(file, "r");
    }
    else {
        exit(1);
    }

    if (input == NULL) {
        exit(1);
    }

    while (!feof(input)) {
        if (mode == INTERACTIVE) {
            printf("dush> ");
        }
        char *line = NULL;
        size_t len = 0;
        ssize_t cntRead;

        cntRead = getline(&line, &len, input);
        if (cntRead <= 0) {
            exit(0);
        }
        // Remove newline character and terminate the c-string
        if (line[cntRead - 1] == '\n') {
            line[cntRead - 1] = '\0';
        }
        
        char *fullCmd = NULL;
        int cntCmd = 0;
        while ((fullCmd = strsep(&line, "&")) != NULL) {
            char **args = NULL;
            /*
            - parse command and arguments
            - if cmd is a valid exit, exit(0)
            - otherwise, spawn new process to handle the new cmd.
            - after that, wait for all to complete.
            
            */
            ParserResult parserRes = parseAndValidateCmd(fullCmd);
            if (!parserRes.isValid) {
                printError();
                continue;
            }
            char *cmd = parserRes.argv[0];
            if (strcmp(cmd, "exit") == 0) {
                if (parserRes.argc > 1) {
                    printError();
                    continue;
                }
                exit(0);
            }
            printf("full cmd: %s\n", fullCmd);
            printf("main cmd: %s\n", cmd);
            // executeCmd()
        }
    }
}

ParserResult parseAndValidateCmd(char *fullCmd) {
    /*
    Edge cases:
        - Command is null
        - Command has multiple redirection symbols
        - Command is redirected to multiple files
    */ 
    ParserResult result;
    result.isValid = true;
    result.argc = 0;
    result.redirection = NULL;

    if (fullCmd == NULL) {
        result.isValid = false;
        return result;
    }

    /*
    Check if command has multiple redirection symbols.
    */
    char *copyCmd = mallocStr(fullCmd);
    char *copyCmdPtr = copyCmd; // we need to keep the original pointer so that we can free the block later 
    char *savePtr = NULL;   // internal pointer for strtok_r

    int redirSymbolCnt= -1;
    while (strtok_r(copyCmd, ">", &savePtr)) {
        redirSymbolCnt += 1;
        copyCmd = NULL; // Set pointer to NULL for strtok_r to work
    }
    free(copyCmdPtr);
    if (redirSymbolCnt > 1) {
        result.isValid = false;
        return result;
    }

    /*
    Check if command is redirected to multiple files.
    */
    char *redirFile = strchr(fullCmd, '>');
    if (redirFile != NULL) {
        redirFile += 1;
        int redirFileCnt = 0;

        copyCmd = mallocStr(redirFile);
        copyCmdPtr = copyCmd;
        savePtr = NULL;  
        result.redirection = mallocStr(copyCmd);
        while (strtok_r(copyCmd, " ", &savePtr)) {
            redirFileCnt += 1;
            copyCmd = NULL; // Set pointer to NULL for strtok_r to work
        }
        free(copyCmdPtr);
        if (redirFileCnt > 1) {
            result.isValid = false;
            return result;
        }
    }
    /*
    Extract argc
    */
    copyCmd = mallocStr(fullCmd);
    copyCmdPtr = copyCmd;
    savePtr = NULL;  
    char *arg;
    while ((arg = strtok_r(copyCmd, " ", &savePtr))) {
        if (strcmp(arg, ">") == 0) {
            break;
        }
        result.argc += 1;
        copyCmd = NULL; // Set pointer to NULL for strtok_r to work
    }
    free(copyCmdPtr);

    /*
    Extract argv
    */
    result.argv = (char **) malloc(sizeof(char *) * result.argc);
    copyCmd = mallocStr(fullCmd);
    copyCmdPtr = copyCmd;
    savePtr = NULL;  
    for (int i = 0; i < result.argc; i++) {
        char *arg = strtok_r(copyCmd, " ", &savePtr);
        int argLen = strlen(arg);
        result.argv[i] = mallocStr(arg);
        copyCmd = NULL; // Set pointer to NULL for strtok_r to work
    }
    free(copyCmdPtr);
    return result;
}

void executeCmd(int argc, char **argv) {
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
    if (strcmp(cmd, "cd") == 0) {
        if (argc <= 1 || argc > 2) {
            printf("test");
        }
    }
}

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