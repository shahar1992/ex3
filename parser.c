
#include "EscapeTechnion.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

MtmErrorCode GetChannels(int argc,char** argv,FILE** input_c,FILE** output_c){
    if (argc==1){
        *input_c=stdin;
        *output_c=stdout;
    }
    else if(argc==5){
        if(strcmp(argv[1],"-i")==0 && strcmp(argv[3],"-o")==0){
            MtmErrorCode result = getInputOutputChannels
                    (argv+2,argv+4,input_c,output_c);
        }
        else if(strcmp(argv[1],"-o")==0 && strcmp(argv[3],"-i")==0){
            MtmErrorCode result = getInputOutputChannels(argv+4,argv+2,
                                                         input_c,output_c);
        }
        else{
            return MTM_INVALID_COMMAND_LINE_PARAMETERS;
        }
    }
    else if (argc == 3){

    }
    else{

    }
}

static MtmErrorCode getInputOutputChannels(char** input_file,char** output_file,
                                           FILE** input_c,FILE** output_c){

    *input_c = fopen(*input_file,"r");
    if(!input_file){
        return MTM_CANNOT_OPEN_FILE;
    }
    *output_c = fopen(*output_file,"a")
}
