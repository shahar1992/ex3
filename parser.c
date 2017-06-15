
#include "EscapeTechnion.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

static MtmErrorCode getInputOutputChannels(char** input_file,char** output_file,
                                           FILE** input_c,FILE** output_c);
static MtmErrorCode getOneChannel(char** argv,FILE** input_c,FILE** output_c);
static MtmErrorCode getTwoChannels(char** argv,FILE** input_c,FILE** output_c);

MtmErrorCode getChannels2(int args_c, char **args_v, FILE **input_c,
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
        *input_c = fopen(argv[0],"r");
        if(!(*input_c) ){
            return MTM_CANNOT_OPEN_FILE;
        }
        *output_c = fopen(argv[2],"w");
        if(!(*output_c) ){
            fclose(*input_c);
            return MTM_CANNOT_OPEN_FILE;
        }
        return MTM_SUCCESS;
    }
}
