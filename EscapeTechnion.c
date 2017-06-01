

/**====================Include files==============================*/
#include <assert.h>
#include <string.h>
#include <stdlib.h>

#include "EscapeTechnion.h"


/**======================Macros===================================*/

#define FACULTY_NUM 19


/**====================Main entity decleration==============================*/

struct system_t{
    CompanySet companies;
    OrdersList orders;
    EscaperSet escapers;
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
    if(!(*system)->companies){
        systemDestroy(*system);
    }
    (*system)->escapers = setCreate(EscaperCopy, EscaperDestroy, EscaperCompare);
    if(!(*system)->escapers){
        systemDestroy(*system);
    }
    (*system)->orders = listCreate(OrderCopy,OrderDestroy);
    if(!(*system)->orders){
        systemDestroy(*system);
    }
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
        free(system);
    }
    return;
}

/**======================System Add Company===================================*/
/*MtmErrorCode systemAddCompany(System system, char* email,
                              TechnionFaculty faculty){
    assert(system && email);
    if(!checkCompanyInput(email,faculty)){
        return MTM_INVALID_PARAMETER;
    }
    Company company = companyCreate(email,faculty);
    if(!company){
        return MTM_OUT_OF_MEMORY;
    }
    if(isMailAlreadyExist())///Not finished!
    SetResult result = setAdd(system->companies,company);
    if(result == SET_OUT_OF_MEMORY){
        return MTM_OUT_OF_MEMORY;
    }
    if(result == SET_ITEM_ALREADY_EXISTS){

    }
    return MTM_SUCCESS;
}
*/
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

