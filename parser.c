//
// Created by Amir on 15/06/2017.
//

#include "EscapeTechnion.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

MtmErrorCode GetChannels(int argc,char** argv,FILE** input_c,FILE** output_c){
    if()
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
    else

}

static MtmErrorCode Check_CL_Parameters(int argc,char** argv){
    if(argc==1){
        return MTM_SUCCESS;
    }
    if(argc==3){
        CheckCLValidilty(argv,1)==
    }
    if(argc==5){
        ( (CheckCLValidilty(argv,1)==true) && (CheckCLValidilty(argv,3)==true) )
        ?

    }
    else return MTM_INVALID_COMMAND_LINE_PARAMETERS;
}
static bool CheckIndexValidilty(char **argv, int index){
    if( (strcmp(argv[index],"-i"==0) || (strcmp(argv[index],"-o")==0) ){
        /*Check if index string is "-i" or "-o" */
        if( (strcmp(argv[index+1],"-i"!=0) && (strcmp(argv[index+1],"-o")!=0) ){
            /*Check if index+1 string is not "-i" or "-o" */
            return true;
        }
    }
    return false;


}
