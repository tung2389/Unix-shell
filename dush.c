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

#include<stdio.h>
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
        while ((fullCmd = strsep(&line, "&")) != NULL) {
            char **args = NULL;
            /*
            - parse command and arguments
            - if cmd is a valid exit, exit(0)
            - otherwise, spawn new process to handle the new cmd.
            - after that, wait for all to complete.
            */
            ParserResult parsedCmd = parseCmd(fullCmd);
            char *cmd = parsedCmd.argv[0];
            if (strcmp(cmd, "exit") == 0) {
                if (parsedCmd.argc > 1) {
                    printError(STDERR_FILENO);
                    continue;
                }
                exit(0);
            }
        }
    }
}

ParserResult parseCmd(char *fullCmd) {
    ParserResult result;
    result.argc = 0;

    char *rest = fullCmd;
    while (strtok_r(rest, " ", &rest)) {
        result.argc += 1;
    }
    result.argv = (char **) malloc(sizeof(char *) * result.argc);
    rest = fullCmd;
    for (int i = 0; i < result.argc; i++) {
        char *arg = strtok_r(rest, " ", &rest);
        int argLen = strlen(arg);
        result.argv[i] = mallocStr(arg);
    }
    return result;
}

// bool executeCmd(char *cmd) {   
//     if (strcmp(cmd, "exit") {
//         exit(0);
//     }
// }

char * mallocStr(char *str) {
    int len = strlen(str);
    char *p = malloc(len + 1);
    p[len] = '\0';
    strcpy(p, str);
    return p;
}

void printError(int stderr_fd) {
    char error_message[30] = "An error has occurred\n";
    write(stderr_fd, error_message, strlen(error_message)); 
}