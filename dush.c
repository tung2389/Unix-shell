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

int main(int argc, char *argv[]) {
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

    while (!feof(stdin)) {
        if (mode == INTERACTIVE) {
            printf("dush> ");
        }
        char *cmd = NULL;
        size_t cmdLen = 0;
        ssize_t cntRead;

        cntRead = getline(&cmd, &cmdLen, input);
        if (cntRead <= 0) {
            exit(0);
        }
        // Remove newline character and terminate the c-string
        if (cmd[cntRead - 1] == '\n') {
            cmd[cntRead - 1] = '\0';
        }
        fwrite(cmd, cntRead, 1, stdout);
        printf("\n");
    }
}