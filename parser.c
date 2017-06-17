
#include <string.h>
#include <assert.h>

#include "EscapeTechnion.h"
#include "mtm_ex3.h"
#include "stdlib.h"

#define MAX_LINE_SIZE 256
#define MAX_ARGUMENTS_NUM 6
#define TAB_ASCII_CODE 11

MtmErrorCode parserAnalyzeCommand(EscapeTechnion sys,char* buffer,
                                  FILE* input_c,FILE* output_c);
MtmErrorCode handleCommand(EscapeTechnion sys,
                           char* command,char* sub_command,char**arg_Array);
MtmErrorCode handleCompanyCommand(EscapeTechnion sys,
                                  char* sub_command,char**arg_Array);
MtmErrorCode handleRoomCommand(EscapeTechnion sys,
                               char* sub_command,char**arg_Array);
MtmErrorCode handleEscaperCommand(EscapeTechnion sys,
                                  char* sub_command,char**arg_Array);
MtmErrorCode handleReportCommand(EscapeTechnion sys,
                                 char* sub_command);
static MtmErrorCode reportDay(EscapeTechnion system);


/**-------------------- Command Interpreter-----------------------------------*/

MtmErrorCode parserAnalyzeCommand(EscapeTechnion system, char* buffer,
                                  const FILE* input_c,const FILE* output_c) {
    char* command;
    char* subCommand;
    char* arg_Array[MAX_ARGUMENTS_NUM]={0};

    const char delim[] = " \t\n";//
    char *token;

    /* get the Command string */
    token = strtok(buffer, delim);
    command=token;
    /* get the Subcommand string */
    if(command==NULL||*command=='#') return MTM_SUCCESS;
    token = strtok(NULL, delim);
    subCommand=token;

    /* Get arguments */
    int count=0;
    token = strtok(NULL, delim);
    while( token != NULL )
    {
        arg_Array[count]=token;
        count++;
        token = strtok(NULL, delim);
    }
    return handleCommand(system,command,subCommand,arg_Array);

}

MtmErrorCode handleCommand(EscapeTechnion system,
                           char* command,char* subcomand,char**arg_Array){
    if(strcmp(command,"company")==0){
        return handleCompanyCommand(system,subcomand,arg_Array);
    }
    /*
    if(strcmp(command,"room")==0){
        return handleRoomCommand;
    }
    if(strcmp(command,"escaper")==0){
        return handleEscaperommand;
    }
    if(strcmp(command,"report")==0){
        return handleReportommand;
    }
    return MTM_INVALID_COMMAND_LINE_PARAMETERS;
     */
    return MTM_SUCCESS;
}

MtmErrorCode handleCompanyCommand(EscapeTechnion system,
                                  char* subcomand,char**arg_Array){
    if(strcmp(subcomand,"add")==0){
        char *ptr;
        long faculty;
        strtol(arg_Array[1],&ptr,10);
        return escapeTechnionAddCompany(system,arg_Array[0],faculty);
    }
}

MtmErrorCode handleRoomCommand(EscapeTechnion system,
                               char* sub_command,char**arg_Array);
MtmErrorCode handleEscaperCommand(EscapeTechnion system,
                                  char* sub_command,char**arg_Array);

MtmErrorCode handleReportCommand(EscapeTechnion system, char* sub_command,
                                 const FILE* output_c) {
    assert(system);
    MtmErrorCode result = MTM_INVALID_COMMAND_LINE_PARAMETERS;
    if(strcmp(sub_command,"day") == 0){
        result = reportDay(system,output_c);
    }
    else if(strcmp(sub_command,"best") == 0){
        result = reportBest(system,output_c);
    }
    return result;
}



static MtmErrorCode reportDay(EscapeTechnion system,const FILE* output_c){
    assert(system);
    int day = escapeTechnionGetDay(system);
    int orders_num;
    MtmErrorCode result = EscapeTechnionOrdersNumByDay(system,day);
    mtmPrintDayHeader(output_c,day,orders_num);
    mtmPrintDayFooter(output_c,escapeTechnionGetDay(system));
}