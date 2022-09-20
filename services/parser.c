#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h> 
#include "../common/common.h"
#include "parser.h"

/*
validate user cmd (null, multiple redirect, or redirect to multiple files)
input: user cmd
output: a Parser object with valid property.
*/
ParserResult validateAndParse(char *fullCmd) {
    /*
    Edge cases:
        - Command is null
        - Command has a redirection symbol without a command.
        - Command has multiple redirection symbols
        - Command has a redirection symbol but it's not redirected to anything
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
    fullCmd = strip(fullCmd);

    /*
    Check if command has a redirection symbol without a command.
    */
    char *redirSymbolPtr = strchr(fullCmd, '>');
    if (redirSymbolPtr == fullCmd) {
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

    // Extract redirection file
    char *redirFile = NULL; 
    if (redirSymbolPtr != NULL) {
        //string after 1st '>'
        redirFile = strip(redirSymbolPtr + 1);
    }
    result.redirection = redirFile;

    /*
    Check if command has redirection symbol but is not redirected to anything
    */
    if (redirSymbolPtr != NULL && strlen(redirFile) == 0) {
        result.isValid = false;
        return result;
    }

    /*
    Check if command is redirected to multiple files and extract file that is redirected to.
    */
    char *charPtr = redirFile;
    while (charPtr != NULL && *charPtr != '\0') {
        if (*charPtr == ' ') {
            result.isValid = false;
            return result;
        }
        charPtr++;
    }

    /*
    Extract argc
    */
    char *copyCmd = mallocStr(fullCmd);
    char *copyCmdPtr = copyCmd;
    char *savePtr = NULL;  
    
    // Terminate the string at the position of '>' character so that we can correclty count number of arguments. 
    removeRedirChar(copyCmd);
    while ((strtok_r(copyCmd, " ", &savePtr))) {
        result.argc += 1;
        copyCmd = NULL; // Set pointer to NULL for strtok_r to work
    }
    free(copyCmdPtr);
    
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

/*
remove the '>' char in user cmd
input: a full cmd from user
output: user command no longer has '>'
*/
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