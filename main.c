
#include "EscapeTechnion.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "parser.c"

#define MAX_LINE_SIZE 256

int main(int argc,  char** argv) {
    EscapeTechnion system;
    escapeTechnionCreate(&system);
    FILE *input_c,*output_c;
    MtmErrorCode result;
    char* array[] = {"main","-i","input.txt","-o","output.txt"};
    result = getChannels(5, array, &input_c, &output_c);
    if(result != MTM_SUCCESS){
        mtmPrintErrorMessage(stderr,result);
    }
    char buffer[MAX_LINE_SIZE]={0};
    while(fgets(buffer,MAX_LINE_SIZE,input_c) != NULL){//while !EOF

        char command[MAX_LINE_SIZE];
        if(fscanf(input_c,"%s",command) == 0){
            printf("empty line\n");
            continue;
        }
        if(*command == '#'){
            printf("comment line\n");
            continue;
        }
        printf("%s",command);
        break;

    }
    fclose(output_c);
    fclose(input_c);
    escapeTechnionDestroy(system);
    return 0;
}


