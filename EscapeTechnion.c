

/**====================Include files==============================*/
#include <assert.h>
#include <string.h>
#include <stdlib.h>

#include "EscapeTechnion.h"


/**======================Macros===================================*/

#define FACULTY_NUM 19

#define SYSTEM_ALLOCATION_CHECK(condition){ \
    if(!(condition)) { \
        systemDestroy(*system); \
        return MTM_OUT_OF_MEMORY; \
    } \
}


/**====================Main entity decleration==============================*/

struct system_t{
    CompanySet companies;
    OrdersList orders;
    EscaperSet escapers;
    RoomSet rooms;
    long day;
    long faculties_profits[FACULTY_NUM];
};


/**=====================Static functions declarations=========================*/

static bool checkCompanyInput(char* mail, TechnionFaculty faculty);
static bool sysMailCheck(char* mail);
static bool facultyCheck(TechnionFaculty faculty);
static MtmErrorCode getSystemTime(char *time_format,long* hour,long* days_left);



/**===================System ADT functions implementation=====================*/

/**===========================SystemCreate====================================*/
MtmErrorCode systemCreate(System* system){
    *system = malloc(sizeof(**system));
    if(!*system){
        return MTM_OUT_OF_MEMORY;
    }
    (*system)->companies = setCreate(companyCopy,companyDestroy,companyCompare);
    SYSTEM_ALLOCATION_CHECK((*system)->companies);
    (*system)->escapers = setCreate(EscaperCopy, EscaperDestroy, EscaperCompare);
    SYSTEM_ALLOCATION_CHECK((*system)->escapers);
    (*system)->orders = listCreate(OrderCopy,OrderDestroy);
    SYSTEM_ALLOCATION_CHECK((*system)->orders);
    (*system)->rooms = setCreate(roomCopy,roomDestroy,roomCompare);
    SYSTEM_ALLOCATION_CHECK((*system)->rooms);
    for(int i=0 ; i < FACULTY_NUM ; i++) {
        (*system)->faculties_profits[i] = 0;
    }
    (*system)->day = 0;
    return MTM_SUCCESS;
}

/**========================System destroy=====================================*/
void systemDestroy(System system){
    if(system != NULL) {
        setDestroy(system->companies);
        listDestroy(system->orders);
        setDestroy(system->escapers);
        setDestroy(system->rooms);
        free(system);
    }
    return;
}

/**======================System Add Company===================================*/
MtmErrorCode systemAddCompany(System system, char* email,
                              TechnionFaculty faculty){
    assert(system && email);
    if(!checkCompanyInput(email,faculty)){
        return MTM_INVALID_PARAMETER;
    }
    if(isEmailAlreadyExist(system, email)){
        return MTM_EMAIL_ALREADY_EXISTS;
    }
    Company company = companyCreate(email,faculty);
    if(!company){
        return MTM_OUT_OF_MEMORY;
    }

    SetResult result = setAdd(system->companies,company);
    if(result == SET_OUT_OF_MEMORY){
        return MTM_OUT_OF_MEMORY;
    }

    return MTM_SUCCESS;
}

/**===================Static functions implementation==========================*/
static bool checkCompanyInput(char* mail, TechnionFaculty faculty){
    return sysMailCheck(mail) && facultyCheck(faculty);
}

static bool sysMailCheck(char* mail){
    char* ptr = mail;
    bool email_check;
    while(*ptr){
        if(*(ptr++) == '@') {
            if(email_check){
                return false;
            }
            email_check = true;
        }
    }
    return true;
}

static bool facultyCheck(TechnionFaculty faculty){
    return (faculty >= 0) && (faculty <= FACULTY_NUM-1);
}

static bool isEmailAlreadyExist(System system, char* email){
    assert((system != NULL) && (email != NULL));
    SET_FOREACH(Company , current_company, system->companies) {
        if (strcmp(companyGetEmail(current_company),email) == 0 ) {
            return true;
        }
    }
    SET_FOREACH(Escaper, current_escaper, system->escapers){
        if(strcmp(EscaperGetEmail(current_escaper),email) == 0 ){
            return true;
        }
    }
    return false;
}

