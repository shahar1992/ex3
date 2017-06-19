
#include <string.h>
#include <assert.h>

#include "EscapeTechnion.h"
#include "mtm_ex3.h"
#include "stdlib.h"

#define MAX_LINE_SIZE 256
#define MAX_ARGUMENTS_NUM 6
#define TAB_ASCII_CODE 11

MtmErrorCode parserAnalyzeCommand(EscapeTechnion system,char* buffer,
                                  FILE* input_c, FILE* output_c);
MtmErrorCode handleCommand(EscapeTechnion sys, FILE* output_c,
                           char* command,char* sub_command,char**arg_Array);
MtmErrorCode handleCompanyCommand(EscapeTechnion sys,
                                  char* sub_command,char**arg_Array);
MtmErrorCode handleRoomCommand(EscapeTechnion sys,
                               char* sub_command,char**arg_Array);

MtmErrorCode handleEscaperCommand(EscapeTechnion sys,
                                  char* sub_command,char**arg_Array);

MtmErrorCode handleReportCommand(EscapeTechnion system, char* sub_command,
                                 FILE* output_c);
static MtmErrorCode reportDay(EscapeTechnion system, FILE* output_c);
static MtmErrorCode reportBest(EscapeTechnion system, FILE* output_c);
static MtmErrorCode orderPrint(EscapeTechnion system, Order order,
                               FILE* output_c);
static MtmErrorCode ConvertResult(EscapeTechnionResult result);
/**-------------------- Command Interpreter-----------------------------------*/

MtmErrorCode parserAnalyzeCommand(EscapeTechnion system, char* buffer,
                                  FILE* input_c, FILE* output_c) {
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
    return handleCommand(system,output_c,command,subCommand,arg_Array);

}

MtmErrorCode handleCommand(EscapeTechnion system, FILE* output_c,
                           char* command,char* subcomand,char**arg_Array){
    if(strcmp(command,"company")==0){
        return handleCompanyCommand(system,subcomand,arg_Array);
    }

    if(strcmp(command,"room")==0){
        return handleRoomCommand(system,subcomand,arg_Array);
    }
    if(strcmp(command,"escaper")==0){
        return handleEscaperCommand(system,subcomand,arg_Array);
    }
    if(strcmp(command,"report")==0){
        return handleReportCommand(system,subcomand,output_c);
    }//
    return MTM_SUCCESS;
}

MtmErrorCode handleCompanyCommand(EscapeTechnion system,
                                  char* subcomand,char**arg_Array){
    EscapeTechnionResult result;
    if(strcmp(subcomand,"add")==0){
        char *ptr;
        long faculty=strtol(arg_Array[1],&ptr,10);
        result=escapeTechnionAddCompany(system,arg_Array[0],faculty);
         return ConvertResult(result);
    }
    if(strcmp(subcomand,"remove")==0){
        result=escapeTechnionRemoveCompany(system,arg_Array[0]);
         return ConvertResult(result);
    }
    return MTM_INVALID_COMMAND_LINE_PARAMETERS;
}

MtmErrorCode handleRoomCommand(EscapeTechnion system,
                               char* sub_command,char**arg_Array){
    EscapeTechnionResult result;
    if(strcmp(sub_command,"add")==0){
        char *ptr;
        long id=strtol(arg_Array[1],&ptr,10);
        long price=strtol(arg_Array[2],&ptr,10);
        long num_ppl=strtol(arg_Array[3],&ptr,10);
        long open_hour=strtol(arg_Array[4],&ptr,10);
        ptr++;
        long close_hour=strtol(ptr,&(ptr),10);
        long difficulty=strtol(arg_Array[5],&ptr,10);
        result=escapeTechnionAddRoom(system,arg_Array[0],id,price,num_ppl,open_hour,close_hour,difficulty);
        return ConvertResult(result);
    }
    if(strcmp(sub_command,"remove")==0){
        char *ptr;
        long faculty=strtol(arg_Array[0],&ptr,10);
        long id=strtol(arg_Array[1],&ptr,10);
        result=escapeTechnionRemoveRoom(system,faculty,id);
        return ConvertResult(result);
    }
    return MTM_INVALID_COMMAND_LINE_PARAMETERS;
}

MtmErrorCode handleEscaperCommand(EscapeTechnion system,
                                  char* sub_command,char**arg_Array){
    EscapeTechnionResult result;
    if(strcmp(sub_command,"add")==0){
        char *ptr;
        long faculty=strtol(arg_Array[1],&ptr,10);
        long skill_level=strtol(arg_Array[2],&ptr,10);
        result=escapeTechnionAddClient(system,arg_Array[0],faculty,skill_level);
        return ConvertResult(result);
    }
    if(strcmp(sub_command,"remove")==0){
        result=escapeTechnionRemoveClient(system,arg_Array[0]);
        return ConvertResult(result);
    }
    if(strcmp(sub_command,"order")==0){
        char *ptr;
        long faculty=strtol(arg_Array[1],&ptr,10);
        long id=strtol(arg_Array[2],&ptr,10);
        long day=strtol(arg_Array[3],&ptr,10);
        ptr++;
        long hour=strtol(ptr,&(ptr),10);
        long num_ppl=strtol(arg_Array[4],&ptr,10);
        result=escapeTechnionAddOrder(system,arg_Array[0],faculty,id,day,hour,num_ppl);
        return ConvertResult(result);
    }
    if(strcmp(sub_command,"recommend")==0){
        char *ptr;
        long num_ppl=strtol(arg_Array[1],&ptr,10);
        result=escapeTechnionRecommendedRoomOrder(system,arg_Array[0],num_ppl);
        return ConvertResult(result);
    }
    return MTM_INVALID_COMMAND_LINE_PARAMETERS;
}//

MtmErrorCode handleReportCommand(EscapeTechnion system, char* sub_command,
                                  FILE* output_c) {
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

static MtmErrorCode reportDay(EscapeTechnion system, FILE* output_c){
    assert(system);
    int day = escapeTechnionGetDay(system);
    OrdersList orders_list = escapeTechnionGetTodayOrdersList(system);
    if(!orders_list){
        return MTM_OUT_OF_MEMORY;
    }
    mtmPrintDayHeader(output_c,day,listGetSize(orders_list));
    LIST_FOREACH(Order,order,orders_list){
        orderPrint(system,order,output_c);
        long profit = orderCalculatePrice(order);
        TechnionFaculty faculty;
        orderGetFaculty(order,&faculty);
        escapeTechnionIncreaseFacultyProfit(system,profit,faculty);
    }
    listDestroy(orders_list);
    mtmPrintDayFooter(output_c,escapeTechnionGetDay(system));
    escapeTechnionIncreaseDay(system);
    return MTM_SUCCESS;
}

static MtmErrorCode orderPrint(EscapeTechnion system,Order order,
                               FILE* output_c){
    Room room = orderGetRoom(order);
    TechnionFaculty room_faculty,escaper_faculty;
    orderGetFaculty(order,&room_faculty);
    Company company = escapeTechnionFindCompanyByRoomAndFaculty
            (system,room,room_faculty);
    Escaper escaper = orderGetEscaper(order);
    char *email,*company_email;
    escaperGetEmail(escaper,&email);
    if(!email){
        return MTM_OUT_OF_MEMORY;
    }
    companyGetEmail(company,&company_email);
    if(!company_email){
        free(email);
        return MTM_OUT_OF_MEMORY;
    }
    escaperGetFaculty(escaper,&escaper_faculty);
    long price = orderCalculatePrice(order);
    mtmPrintOrder(output_c,email,escaperGetSkillLevel(escaper),escaper_faculty,
                  company_email,room_faculty,roomGetId(room),orderGetHour(order),
                  orderGetDifficulty(order),orderGetNumOfPeople(order),price);
    return MTM_SUCCESS;
}

static MtmErrorCode reportBest(EscapeTechnion system, FILE* output_c){
    assert(system);
    int best_faculties_num = 3;
    long total_revenue = escapeTechnionCalculateTotalRevenue(system);
    mtmPrintFacultiesHeader(
            output_c,FACULTY_NUM,escapeTechnionGetDay(system),total_revenue);
    TechnionFaculty faculties[best_faculties_num];
    escapeTechnionGetBestFaculties(system,faculties,best_faculties_num);
    for(int i = 0 ; i < 3 ; i++) {
        mtmPrintFaculty(output_c, faculties[i],
                        escapeTechnionGetFacultyProfit(system, faculties[i]));
    }
    mtmPrintFacultiesFooter(output_c);
    return MTM_SUCCESS;
}

static MtmErrorCode ConvertResult(EscapeTechnionResult result){
    switch(result){
        case ESCAPE_TECHNION_SUCCESS:
            return MTM_SUCCESS;
        case ESCAPE_TECHNION_ROOM_NOT_AVAILABLE:
            return MTM_ROOM_NOT_AVAILABLE;
        case ESCAPE_TECHNION_NO_ROOMS_AVAILABLE:
            return MTM_NO_ROOMS_AVAILABLE;
        case ESCAPE_TECHNION_CLIENT_EMAIL_DOES_NOT_EXIST:
            return MTM_CLIENT_EMAIL_DOES_NOT_EXIST;
        case ESCAPE_TECHNION_CLIENT_IN_ROOM:
            return MTM_CLIENT_IN_ROOM;
        case ESCAPE_TECHNION_ID_DOES_NOT_EXIST:
            return MTM_ID_DOES_NOT_EXIST;
        case ESCAPE_TECHNION_INVALID_PARAMETER:
            return MTM_INVALID_PARAMETER;
        case ESCAPE_TECHNION_NULL_PARAMETER:
            return MTM_NULL_PARAMETER;
        case ESCAPE_TECHNION_EMAIL_ALREADY_EXISTS:
            return MTM_EMAIL_ALREADY_EXISTS;
        case ESCAPE_TECHNION_ID_ALREADY_EXIST:
            return MTM_ID_ALREADY_EXIST;
        case ESCAPE_TECHNION_COMPANY_EMAIL_DOES_NOT_EXIST:
            return MTM_COMPANY_EMAIL_DOES_NOT_EXIST;
        case ESCAPE_TECHNION_RESERVATION_EXISTS:
            return MTM_RESERVATION_EXISTS;
        case ESCAPE_TECHNION_OUT_OF_MEMORY:
            return MTM_OUT_OF_MEMORY;

    }
}