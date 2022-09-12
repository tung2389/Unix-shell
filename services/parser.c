#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h> 
#include "../common/common.h"
#include "parser.h"

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
    Check if command is redirected to multiple files and extract file that is redirected to.
    */
    char *redirFile = strchr(fullCmd, '>');
    if (redirFile != NULL) {
        redirFile += 1;
        int redirFileCnt = 0;

        copyCmd = mallocStr(redirFile);
        copyCmdPtr = copyCmd;
        savePtr = NULL;  
        result.redirection = strip(mallocStr(copyCmd));
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