

/**====================Include files==============================*/
#include <assert.h>
#include <string.h>
#include <stdlib.h>

#include "EscapeTechnion.h"


/**======================Macros===================================*/

#define FACULTY_NUM 19

#define SYSTEM_ALLOCATION_CHECK(condition,system_ptr_to_destroy){ \
    if(!(condition)) { \
        systemDestroy(system_ptr_to_destroy); \
        return MTM_OUT_OF_MEMORY; \
    } \
}

#define SYSTEM_PARAMETERS_CHECK(condition){ \
    if(!(condition)){ \
        return MTM_INVALID_PARAMETER; \
    } \
}

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
static MtmErrorCode getSystemTime(char *time_format, long* hour, long* day);
static bool isEmailAlreadyExist(System system, char* email);
static bool isCompanyHasReservation(System system, char* email);
static Company systemGetCompany(System system, char* email);
static bool checkRoomInput(char *email, long id, long price, int num_ppl,
                           int open_hour, int close_hour, int difficulty);
static bool isIdAlreadyExistInFaculty(System system, long id,
                                      TechnionFaculty faculty);
static bool isRoomHasReservation(System system, Company company, Room room);



/**===================System ADT functions implementation=====================*/

/**===========================SystemCreate====================================*/
MtmErrorCode systemCreate(System* system){
    *system = malloc(sizeof(**system));
    if(!*system){
        return MTM_OUT_OF_MEMORY;
    }
    (*system)->companies = setCreate(companyCopy,companyDestroy,companyCompare);
    SYSTEM_ALLOCATION_CHECK((*system)->companies, *system);
    (*system)->escapers = setCreate(EscaperCopy, EscaperDestroy, EscaperCompare);
    SYSTEM_ALLOCATION_CHECK((*system)->escapers ,*system);
    (*system)->orders = listCreate(OrderCopy,OrderDestroy);
    SYSTEM_ALLOCATION_CHECK((*system)->orders, *system);
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
MtmErrorCode systemAddCompany(System system, char* email,
                              TechnionFaculty faculty){
    assert(system && email);
    SYSTEM_PARAMETERS_CHECK(checkCompanyInput(email,faculty));
    if(isEmailAlreadyExist(system, email)){
        return MTM_EMAIL_ALREADY_EXISTS;
    }
    Company company = companyCreate(email,faculty);
    SYSTEM_ALLOCATION_CHECK(!company,system);
    SetResult result = setAdd(system->companies,company);
    free(company);
    SYSTEM_ALLOCATION_CHECK(result != SET_OUT_OF_MEMORY ,system)
    assert(result == SET_SUCCESS);
    return MTM_SUCCESS;
}

/**===================System Remove Company===================================*/
MtmErrorCode systemRemoveCompany(System system, char* email){
    assert(system);
    SYSTEM_PARAMETERS_CHECK(sysMailCheck(email));
    Company company = systemGetCompany(system, email);
    if(!company){
        return MTM_COMPANY_EMAIL_DOES_NOT_EXIST;
    }
    if(isCompanyHasReservation(system, email)){
        return MTM_RESERVATION_EXISTS;
    }
    setRemove(system->companies,company);
    companyDestroy(company);
    return MTM_SUCCESS;
}

/**======================System Add Room======================================*/
MtmErrorCode systemAddRoom(System system, char* email, long id, long price,
                           int num_ppl, int open_hour, int close_hour,
                           int difficulty){
    SYSTEM_PARAMETERS_CHECK(checkRoomInput(email,id,price,num_ppl,open_hour,
                             close_hour,difficulty));
    Company company = systemGetCompany(system, email);
    if(!company){
        return MTM_COMPANY_EMAIL_DOES_NOT_EXIST;
    }
    if(isIdAlreadyExistInFaculty(system, id, companyGetFaculty(company))){
        return MTM_ID_ALREADY_EXIST;
    }
    Room room = roomCreate(id,price,num_ppl,open_hour,close_hour,difficulty);
    SYSTEM_ALLOCATION_CHECK(room,system);
    CompanyResult result = companyAddRoom(company,room);
    SYSTEM_ALLOCATION_CHECK(result != COMPANY_OUT_OF_MEMORY,system);
    assert(result==COMPANY_SUCCESS);
    return MTM_SUCCESS;
}

MtmErrorCode systemRemoveRoom(System system, TechnionFaculty faculty, long id){
    SYSTEM_PARAMETERS_CHECK((id > 0)&&(faculty >= 0)&&(faculty < FACULTY_NUM));
    SET_FOREACH(Company,current_company,system->companies) {
        if(companyGetFaculty(current_company) == faculty) {
            Room room = companyFindRoom(current_company, id);
            if(room != NULL){
                if(isRoomHasReservation(system,current_company,room)){
                    return MTM_RESERVATION_EXISTS;
                }
                companyRemoveRoom(current_company,room);
                return MTM_SUCCESS;
            }
        }
    }
    return MTM_ID_DOES_NOT_EXIST;
}

/**===================Static functions implementation=========================*/
static bool checkCompanyInput(char* mail, TechnionFaculty faculty){
    return (sysMailCheck(mail) && facultyCheck(faculty));
}

static bool sysMailCheck(char* mail){
    char* ptr = mail;
    bool email_check = false;
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
    return (faculty >= 0) && (faculty < FACULTY_NUM);
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

static bool isCompanyHasReservation(System system, char* email){
    assert(system && email);
    LIST_FOREACH(Order, current_order, system->orders){
        if(strcmp(email, orderGetEmail(current_order)) == 0 ){
            return true;
        }
    }
    return false;
}

static bool isRoomHasReservation(System system, Company company, Room room){
    LIST_FOREACH(Order, currrent_order, system->orders){
        if(strcmp(companyGetEmail(company),orderGetEmail(currrent_order)) == 0){
            if(orderGetId(currrent_order) == roomGetId(room)){
                return true;
            }
        }
    }
    return false;
}

static Company systemGetCompany(System system, char* email){
    assert(system && email);
    SET_FOREACH(Company,current_company,system->companies){
        if(strcmp(companyGetEmail(current_company),email) == 0){
            return current_company;
        }
    }
    return NULL;
}

static bool checkRoomInput(char *email, long id, long price, int num_ppl,
                           int open_hour, int close_hour, int difficulty){
    assert(email);
    return ((sysMailCheck(email)) && (id > 0) && (price > 0) && (price%4 == 0)
            && (num_ppl > 0) && (open_hour >= 0) && (close_hour < 24) &&
            (close_hour > open_hour) && (difficulty > 0) && (difficulty <= 10));
}

static bool isIdAlreadyExistInFaculty(System system, long id,
                                      TechnionFaculty faculty){
    assert(system && (faculty < 19) && (faculty >= 0));
    assert(id > 0);
    SET_FOREACH(Company, current_company, system->companies){
        if(companyGetFaculty(current_company) == faculty){
            if(companyIsIdIn(current_company, id)){
                return true;
            }
        }
    }
    return false;
}

