
#include "EscapeTechnion.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define TAB_ASCII_CODE  9


/**-------------------- Command Interpreter-----------------------------------*/

MtmErrorCode parserAnalyzeCommand(char* buffer,FILE* input_c,FILE* output_c) {
    if(*buffer == '\n' || *buffer == ' ' || *buffer == '#' ||
            *buffer == TAB_ASCII_CODE){
        return MTM_SUCCESS;
    }
    char* token;
    char* delim = " ";;
    token = strtok(buffer,delim);
    if(*token == '#'){
        return MTM_SUCCESS;
    }
    while (token != NULL){
        printf("%s\n",token);
        token = strtok(NULL,delim);
    }
    return MTM_SUCCESS;
}