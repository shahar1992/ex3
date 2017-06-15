
#include "EscapeTechnion.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "parser.c"

int main(int argc,  char** argv) {
    FILE *input_c,*output_c;
    MtmErrorCode result;
    result = getChannels(argc, argv, &input_c, &output_c);
    fclose(output_c);
    fclose(input_c);
    return 0;
}


