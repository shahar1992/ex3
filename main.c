
#include <stdio.h>
#include <string.h>
#include "parser.c"
#include "assert.h"
#include "EscapeTechnion.h"
#include "mtm_ex3.h"


/**======================Macros and structs===================================*/
#define MAX_LINE_SIZE 256

#define KILL_PROG(error,system,output_c,input_c)\
            mtmPrintErrorMessage((stderr),(error)); \
          //  escapeTechnionDestroy((system)); \
            fclose((output_c)); \
            fclose((input_c)); \
           // escapeTechnionDestroy((system)); \
            return 0; \

/**======================Static Functions Decleration ========================*/
static MtmErrorCode getChannels(int args_c, char **args_v, FILE **input_c,
                                FILE **output_c);
static MtmErrorCode getOneChannel(char** argv,FILE** input_c,FILE** output_c);
static MtmErrorCode getTwoChannels(char** argv,FILE** input_c,FILE** output_c);


/** -----------------------Main-Function------------------------------------**/

int main(int argc,  char** argv) {
    EscapeTechnion sys;
    escapeTechnionCreate(&sys);
    FILE *input_c,*output_c;
    MtmErrorCode result;
    char* array[] = {"main","-i","test1.in","-o","output.txt"};///////////
    result = getChannels(5, array, &input_c, &output_c);///////////////////////////
   // result = getChannels(argc,argv,&input_c, &output_c);
    if(result != MTM_SUCCESS){//Invalid command or open file problem
        KILL_PROG(result,system,output_c,input_c);
    }
    char buffer[MAX_LINE_SIZE]={0};
    while(fgets(buffer,MAX_LINE_SIZE,input_c) != NULL){//while !EOF
        result = parserAnalyzeCommand(sys,buffer,input_c,output_c);
        /*Analyze next command and return error message*/
        if(result != MTM_SUCCESS){
            if(result==MTM_INVALID_COMMAND_LINE_PARAMETERS||
                    result==MTM_OUT_OF_MEMORY){
                KILL_PROG(result,sys,output_c,input_c);
            }
            mtmPrintErrorMessage(stderr,result);//print error msg
        }
    }
    escapeTechnionDestroy(sys);
    fclose(output_c);
    fclose(input_c);
    return 0;
}




/**======================Static function implemantation=======================*/

/**-------------------- Input/Output Handle-----------------------------------*/
static MtmErrorCode getChannels(int args_c, char **args_v, FILE **input_c,
                         FILE **output_c){
    MtmErrorCode result;
    switch (args_c){
        case 1:
            *input_c = stdin;
            *output_c = stdout;
            break;
        case 3:
            result = getOneChannel(args_v+1,input_c,output_c);
            if(result!=MTM_SUCCESS){
                return result;
            }
            break;
        case 5:
            result = getTwoChannels(args_v+1,input_c,output_c);
            if(result != MTM_SUCCESS){
                return result;
            }
            break;
        default:
            return MTM_INVALID_COMMAND_LINE_PARAMETERS;
    }
    return  MTM_SUCCESS;
}

static MtmErrorCode getOneChannel(char** argv,FILE** input_c,FILE** output_c){
    assert(argv && input_c && output_c);
    if(strcmp(argv[0],"-i") == 0){
        if(strcmp(argv[1],"-o") == 0 || strcmp(argv[1],"-i") == 0){
            return MTM_INVALID_COMMAND_LINE_PARAMETERS;
        }
        *input_c = fopen(argv[1],"r");
        *output_c = stdout;
    }
    else if(strcmp(argv[0],"-o") == 0){
        if(strcmp(argv[1],"-i") == 0 || strcmp(argv[1],"-o") == 0){
            return MTM_INVALID_COMMAND_LINE_PARAMETERS;
        }
        *output_c = fopen(argv[1],"w");
        *input_c = stdin;
    }
    else{
        return MTM_INVALID_COMMAND_LINE_PARAMETERS;
    }
    if(!*input_c || !*output_c){
        return MTM_CANNOT_OPEN_FILE;
    }
    return MTM_SUCCESS;
}

static MtmErrorCode getTwoChannels(char** argv,FILE** input_c,FILE** output_c){
    assert(argv && input_c && output_c);
    if(strcmp(argv[0],"-i") == 0){
        if(strcmp(argv[2],"-o") != 0){
            return MTM_INVALID_COMMAND_LINE_PARAMETERS;
        }
        *input_c = fopen(argv[1],"r");
        if(!(*input_c) ){
            return MTM_CANNOT_OPEN_FILE;
        }
        *output_c = fopen(argv[3],"w");
        if(!(*output_c) ){
            fclose(*input_c);
            return MTM_CANNOT_OPEN_FILE;
        }
        return MTM_SUCCESS;
    }
    else if(strcmp(argv[0],"-o") == 0){
        if(strcmp(argv[2],"-i") != 0){
            return MTM_INVALID_COMMAND_LINE_PARAMETERS;
        }
        *input_c = fopen(argv[3],"r");
        if(!(*input_c) ){
            return MTM_CANNOT_OPEN_FILE;
        }
        *output_c = fopen(argv[1],"w");
        if(!(*output_c) ){
            fclose(*input_c);
            return MTM_CANNOT_OPEN_FILE;
        }
        return MTM_SUCCESS;
    }
}

