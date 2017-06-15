
#include "EscapeTechnion.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>


/**-------------------- Command Interpreter-----------------------------------*/

MtmErrorCode parserAnalyzeCommand(char* buffer,FILE* input_c,FILE* output_c) {
    char command[MAX_LINE_SIZE];
    if (fscanf(input_c, "%s", command) == 0) {
        printf("empty line\n");
        continue;
    }
    if (*command == '#') {
        printf("comment line\n");
        continue;
    }
    printf("%s", command);
}