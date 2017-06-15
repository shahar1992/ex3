
#include "EscapeTechnion.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "parser.c"

int main(int argc,  char** argv) {
    FILE *input_c,*output_c;
    char* array[] = {argv[0],"-i","input.txt","-o","output.txt"};
    //char* array1[] = {"-i","input.txt","-o","output.txt"};
   // MtmErrorCode result;
    //result = getChannels2(5, array, &input_c, &output_c);
    //assert(result != MTM_CANNOT_OPEN_FILE);
   // assert(result != MTM_INVALID_COMMAND_LINE_PARAMETERS);
    return 0;
}


