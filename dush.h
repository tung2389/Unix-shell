#include <stdbool.h>

typedef struct ParserResult {
    bool isValid; // Is this command valid 
    int argc; // Number of arguments in this command (including command name) 
    char **argv; // Array of arguments of this command
    char *redirection; // Name of the file that is redirected to
} ParserResult;

// bool isB
ParserResult parseAndValidateCmd(char *fullCmd);
void executeCmd(int argc, char **argv);

char * mallocStr(char *str);
void printError();