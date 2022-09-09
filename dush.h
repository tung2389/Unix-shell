#include <stdbool.h>

typedef struct ParserResult {
    int argc;
    char **argv;
} ParserResult;

// bool isB
ParserResult parseCmd(char *fullCmd);
bool executeCmd(char *cmd);

char * mallocStr(char *str);
void printError(int stderr_fd);