#include <stdlib.h>
#include <stdio.h>
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
        - Command has a redirection symbol but it's not redirected to anything
        - Command has a redirection symbol without a command.
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

    int redirSymbolCnt = 0;
    for (int i = 0; i < strlen(fullCmd); i++) {
        if (fullCmd[i] == '>') {
            redirSymbolCnt += 1;
        }
    }
    if (redirSymbolCnt > 1) {
        result.isValid = false;
        return result;
    }

    /*
    Check if command is redirected to multiple files and extract file that is redirected to.
    */
    char *redirFile = strchr(fullCmd, '>'); 
    if (redirFile != NULL) {
        //string after 1st '>'
        redirFile = strip(redirFile + 1);
    }
    int redirFileCnt = 0;
    char *copyCmd = mallocStr(redirFile);
    char *copyCmdPtr = copyCmd;
    char *savePtr = NULL;  
    
    if (redirFile != NULL && strlen(redirFile) > 0) {
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
    
    // Check if command has redirection symbol but is not redirected to anything
    if (redirSymbolCnt > 0 && redirFileCnt <= 0) {
        result.isValid = false;
        return result;
    }

    /*
    Extract argc
    */
    copyCmd = mallocStr(fullCmd);
    copyCmdPtr = copyCmd;
    savePtr = NULL;  
    char *arg;
    
    // Terminate the string at the position of '>' character so that we can correclty count number of arguments. 
    removeRedirChar(copyCmd);
    while ((arg = strtok_r(copyCmd, " ", &savePtr))) {
        result.argc += 1;
        copyCmd = NULL; // Set pointer to NULL for strtok_r to work
    }
    free(copyCmdPtr);
    
    // Check if redirection with no command
    if (result.argc == 0 && result.redirection != NULL) {
        result.isValid = false;
        return result;
    }

    /*
    Extract argv
    */
    // +1 for the NULL pointer
    result.argv = (char **) malloc(sizeof(char *) * result.argc + 1);
    copyCmd = mallocStr(fullCmd);
    copyCmdPtr = copyCmd;
    savePtr = NULL;  

    removeRedirChar(copyCmd);
    for (int i = 0; i < result.argc; i++) {
        char *arg = strip(strtok_r(copyCmd, " ", &savePtr));
        int argLen = strlen(arg);
        result.argv[i] = mallocStr(arg);
        copyCmd = NULL; // Set pointer to NULL for strtok_r to work
    }
    result.argv[result.argc] = NULL;
    free(copyCmdPtr);
    return result;
}

void removeRedirChar(char *str) {
    /*
    Edge casess:
        - str is NULL
        - str has no '>' character
    */
    if (str == NULL) {
        return;
    }
    char *redirPtr = strchr(str, '>');
    if (redirPtr != NULL) {
        *(redirPtr) = '\0';
    }
}