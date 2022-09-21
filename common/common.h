#ifndef COMMON_H
#define COMMON_H
#include <stdbool.h>

char * mallocStr(char *str);
char * strip(char *str);
bool isBuiltinCmd(char *str);
void freeArrStr(char **arr, int len); 

#endif