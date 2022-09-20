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
#include <sys/wait.h>
#include "common/common.h"
#include "services/services.h"

int main(int argc, char *argv[]) {
    //set up buffer to stdout
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
        printError();
        exit(1);
    }

    if (input == NULL) {
        printError();
        exit(1);
    }
    // Initialize paths variable
    int pathCnt = 1;
    char **paths = malloc(sizeof(char *) * pathCnt);
    paths[0] = mallocStr("/bin");

    //feof -> end of file works for all kind of stream
    while (!feof(input)) {
        if (mode == INTERACTIVE) {
            printf("dush> ");
        }
        char *line = NULL;
        size_t len = 0;
        ssize_t cntRead;

        //user input
        cntRead = getline(&line, &len, input);
        if (cntRead <= 0) {
            exit(0);
        }
        line = strip(line);
        if (strlen(line) == 0) {
            continue;
        }
        // Remove newline character and terminate the c-string
        if (line[cntRead - 1] == '\n') {
            line[cntRead - 1] = '\0';
        }
        char *fullCmd = NULL;
        // Number of processes running paralell
        int cntProc = 0;
        //look for a line + delimeter is & meaning parallel commands
        while ((fullCmd = strsep(&line, "&")) != NULL) {
            char **args = NULL;
            /*
            - parse command and arguments
            - if cmd is a valid exit, exit(0)
            - otherwise, spawn new process to handle the new cmd.
            - after that, wait for all to complete.
            
            */
            //validate input
            ParserResult parserRes = validateAndParse(fullCmd);
            if (!parserRes.isValid) {
                printError();
                continue;
            }
            if (parserRes.argc == 0) {
                continue;
            }
            char *cmd = parserRes.argv[0];
            // Built-in command will not be run parallel 
            if (!isBuiltinCmd(cmd)) {
                cntProc += 1;
            }
            //logCmdInfo(fullCmd, parserRes); 
            executeCmd(parserRes.argc, parserRes.argv, parserRes.redirection, &pathCnt, &paths, &cntProc);
        }

        int cntFinish = 0;
        while (cntFinish < cntProc) {
            wait(NULL);
            cntFinish += 1;
        }
    }
}