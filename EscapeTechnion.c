#include <assert.h>
#include <string.h>
#include <stdlib.h>

#include "EscapeTechnion.h"

/**===============================Macros======================================*/

#define FACULTY_NUM 19
#define BEST_FACULTIES_NUM 3
#define RANDOM_FACULTY ELECTRICAL_ENGINEERING

#define MEMORY_CHECK(condition,system_ptr_to_destroy) \
    if(!(condition)) { \
        escapeTechnionDestroy(system_ptr_to_destroy); \
        return ESCAPE_TECHNION_OUT_OF_MEMORY; \
    } \


#define NULL_ARGUMENT_CHECK(condition) \
    if(!(condition)) { \
        return ESCAPE_TECHNION_NULL_PARAMETER; \
    } \


#define MAIL_EXISTANCE_CHECK(email) \
    if(isEmailAlreadyExist(system, email)){ \
        return ESCAPE_TECHNION_EMAIL_ALREADY_EXISTS; \
    }

/**====================Main entity decleration================================*/

struct EscapeTechnion_t{
    CompanySet companies;
    OrdersList orders;
    EscaperSet escapers;
    int day;
    long faculty_profit[FACULTY_NUM];
};

/**=====================Static functions declarations=========================*/

static bool sysMailCheck(char* mail);
static bool facultyCheck(TechnionFaculty faculty);
static MtmErrorCode getSystemTime(char *time_format, long* hour, long* day);
static bool isEmailAlreadyExist(EscapeTechnion system, char* email);
static bool isCompanyHasReservation(EscapeTechnion system, Company company);
static Company getCompany(EscapeTechnion system, char *email);
static bool isIdAlreadyExistInFaculty(EscapeTechnion system, Room room);
static bool isRoomHasReservation(EscapeTechnion system, Room room);
static EscapeTechnionResult convertFromCompanyResult( CompanyResult result);
static EscapeTechnionResult convertFromRoomResult(RoomResult result);
static EscapeTechnionResult convertFromEscaperResult(EscaperResult result);
static bool isOrderFacultyMatch(ListElement order, ListFilterKey faculty);
static long calculate_total_profit(EscapeTechnion system);
static void findBestFaculties(EscapeTechnion system, TechnionFaculty* faculty);
static void removeClientOrders(EscapeTechnion system, Escaper escaper);

/**===================System ADT functions implementation=====================*/

/**-------------------------System Create-------------------------------------*/
EscapeTechnionResult escapeTechnionCreate(EscapeTechnion *system){
    NULL_ARGUMENT_CHECK(*system);
    *system = malloc(sizeof(**system));
    MEMORY_CHECK(*system, *system);
    (*system)->companies = setCreate(companyCopy,companyDestroy,
                                             companyCompare);
    MEMORY_CHECK((*system)->companies, *system);
    (*system)->escapers = setCreate(escaperCopy,
                                    escaperDestroy, EscaperCompare);
    MEMORY_CHECK((*system)->escapers, *system);
    (*system)->orders = listCreate(orderCopy, orderDestroy);
    MEMORY_CHECK((*system)->orders, *system);
    for(int i=0 ; i < FACULTY_NUM ; i++) {
        (*system)->faculty_profit[i] = 0;
    }
    (*system)->day = 0;
    return ESCAPE_TECHNION_SUCCESS;
}

/**------------------------System destroy-------------------------------------*/
void escapeTechnionDestroy(EscapeTechnion system){
    if(system != NULL) {
        setDestroy(system->companies);
        listDestroy(system->orders);
        setDestroy(system->escapers);
        free(system);
    }
    return;
}

/**----------------------System Add Company-----------------------------------*/
EscapeTechnionResult escapeTechnionAddCompany(EscapeTechnion system,
                                              char *email,
                                              TechnionFaculty faculty){
    Company company;
    EscapeTechnionResult result = convertFromCompanyResult(
            companyCreate(email, faculty, &company));
    if(result != ESCAPE_TECHNION_SUCCESS){
        return result;
    }
    if(isEmailAlreadyExist(system,email)){
        companyDestroy(company);
        return ESCAPE_TECHNION_EMAIL_ALREADY_EXISTS;
    }
    if(setAdd(system->companies,company) == SET_OUT_OF_MEMORY) {
        companyDestroy(company);
        return ESCAPE_TECHNION_OUT_OF_MEMORY;
    }
    companyDestroy(company);
    return ESCAPE_TECHNION_SUCCESS;
}

/**-------------------System Remove Company-----------------------------------*/
EscapeTechnionResult escapeTechnionRemoveCompany(EscapeTechnion system,
                                                 char *email){
    NULL_ARGUMENT_CHECK(system && email);
    Company company;
    EscapeTechnionResult result = convertFromCompanyResult(
            companyCreate(email,RANDOM_FACULTY,&company));
    if(!result){
        companyDestroy(company);
        return result;
    }
    if(!setIsIn(system->companies,company)){
        companyDestroy(company);
        return ESCAPE_TECHNION_COMPANY_EMAIL_DOES_NOT_EXIST;
    }
    if(isCompanyHasReservation(system, company)){
        companyDestroy(company);
        return ESCAPE_TECHNION_RESERVATION_EXISTS;
    }
    setRemove(system->companies,company);
    companyDestroy(company);
    return ESCAPE_TECHNION_SUCCESS;
}

/**----------------------System Add Room--------------------------------------*/
EscapeTechnionResult escapeTechnionAddRoom(EscapeTechnion system, char *email,
                                           long id, long price,int num_ppl,
                                           int open_hour, int close_hour,
                                           int difficulty){
    /*NULL_ARGUMENT_CHECK(system && email);
    Company company = getCompany(system, email);
    Room room;
    EscapeTechnionResult result = convertFromRoomResult(
            roomCreate(id,price,num_ppl,open_hour,close_hour,difficulty,&room));
    if(result != ESCAPE_TECHNION_SUCCESS){
        return result;
    }
    if(!company){
        roomDestroy(room);
        return ESCAPE_TECHNION_COMPANY_EMAIL_DOES_NOT_EXIST;
    }
    if(isIdAlreadyExistInFaculty(system, room)){
        roomDestroy(room);
        return ESCAPE_TECHNION_ID_ALREADY_EXIST;
    }
    roomDestroy(room);
    return ESCAPE_TECHNION_SUCCESS;*/
}

/**----------------------System Remove Room-----------------------------------*/
EscapeTechnionResult escapeTechnionRemoveRoom(EscapeTechnion system,
                                              TechnionFaculty faculty, long id){
    /*if((faculty >= FACULTY_NUM) || (faculty < 0) || (id < 0)){
        return ESCAPE_TECHNION_INVALID_PARAMETER;
    }
    SET_FOREACH(Company,current_company,system->companies){
        TechnionFaculty faculty1;
        companyGetFaculty(current_company,&faculty1);
        if(faculty1 == faculty) {
            if (companyIsRoomAlreadyExist(current_company, id) ==
                    COMPANY_ROOM_ALREADY_EXIST){
                return ESCAPE_TECHNION_ID_DOES_NOT_EXIST;
            }
        }
    }
    //check reservations and remove room from order.
    return ESCAPE_TECHNION_SUCCESS;*/
}

EscapeTechnionResult escapeTechnionAddClient(EscapeTechnion system, char* email,
                                             TechnionFaculty faculty,
                                             int skill_level){
    NULL_ARGUMENT_CHECK(system && email);
    Escaper escaper;
    EscapeTechnionResult result = convertFromEscaperResult(
            escaperCreate(email, faculty, skill_level, &escaper));
    if(result != ESCAPE_TECHNION_SUCCESS){
        return result;
    }
    MAIL_EXISTANCE_CHECK(email);
    SetResult result1 = setAdd(system->escapers,escaper);
    escaperDestroy(escaper);
    if(result1 == SET_OUT_OF_MEMORY){
        return ESCAPE_TECHNION_OUT_OF_MEMORY;
    }
    return ESCAPE_TECHNION_SUCCESS;
}

EscapeTechnionResult escapeTechnionRemoveClient(EscapeTechnion system,
                                                char* email){
    NULL_ARGUMENT_CHECK(system && email);
    Escaper escaper;
    EscapeTechnionResult result = convertFromEscaperResult(escaperCreate(
            email, ELECTRICAL_ENGINEERING, 1, &escaper));
    if(result != ESCAPE_TECHNION_SUCCESS){
        return result;
    }
    if(!setIsIn(system->escapers,escaper)){
        escaperDestroy(escaper);
        return ESCAPE_TECHNION_CLIENT_EMAIL_DOES_NOT_EXIST;
    }
    removeClientOrders(system,escaper);
    setRemove(system->escapers,escaper);
    escaperDestroy(escaper);
    return ESCAPE_TECHNION_SUCCESS;
}

EscapeTechnionResult escapeTechnionAddOrder(EscapeTechnion system, char* email,
                                            TechnionFaculty faculty, long id,
                                            int day, int hour, int num_ppl) {
    NULL_ARGUMENT_CHECK(system && email);
    Order order;
    Room room;
    Escaper escaper;
    EscapeTechnionResult result = convertFromEscaperResult(
            escaperCreate(email,faculty,1,&escaper));
    if(result != ESCAPE_TECHNION_SUCCESS){
        return result;
    }
    EscapeTechnionResult result1 = convertFromRoomResult(
            roomCreate(id,4,1,0,24,1,NULL,&room));
    if(result1 != ESCAPE_TECHNION_SUCCESS){
        escaperDestroy(escaper);
        return result;
    }
    EscapeTechnionResult result2 = convertFromOrderResult(
            orderCreate(faculty, num_ppl,hour,day,room,escaper,&order));
}

/**------------------------Escape Technion Get Faculty Profit-----------------*/
EscapeTechnionResult escapeTechnionGetFacultyProfit(EscapeTechnion system,
                                                    TechnionFaculty faculty,
                                                    long* profit){
    NULL_ARGUMENT_CHECK(system && profit);
    if(!facultyCheck(faculty)){
        return ESCAPE_TECHNION_INVALID_PARAMETER;
    }
    *profit = system->faculty_profit[faculty];
    return ESCAPE_TECHNION_SUCCESS;
}

/**-----------------------Escape Technion Report Day------------------------------------------*/
EscapeTechnionResult  escapeTechnionReportDay(EscapeTechnion system){
    NULL_ARGUMENT_CHECK(system);
    //mtmPrintDayFooter(,system->day);
    for(int i = 0 ; i < FACULTY_NUM ; i++) {
        LIST_FOREACH(Order, current_order,
                     listFilter(system->orders,isOrderFacultyMatch,&i)){
            //orderPrint(current_order);
            system->faculty_profit[i] += orderCalculatePrice(current_order);
            orderDestroy(current_order);
        }
    }
    //mtmPrintDayFooter(,system->day);
    system->day++;
    return ESCAPE_TECHNION_SUCCESS;
}

EscapeTechnionResult escapeTechnionBestFaculties(EscapeTechnion system){
    NULL_ARGUMENT_CHECK(system);
    long totalprofit = calculate_total_profit(system);
    //mtmPrintFacultiesHeader(,FACULTY_NUM,system->day,totalprofit);
    TechnionFaculty faculty[BEST_FACULTIES_NUM];
    findBestFaculties(system,faculty);
    for(int i = 0 ; i < BEST_FACULTIES_NUM ; i++){
        //mtmPrintFaculty(,faculty[i],system->faculty_profit[(int)faculty[i]]);
    }
    //mtmPrintFacultiesFooter();
    return ESCAPE_TECHNION_SUCCESS;
}



/**===================Static functions implementation=========================*/
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

static bool isOrderFacultyMatch(ListElement order, ListFilterKey faculty){
    TechnionFaculty faculty1;
    orderGetFaculty((Order)order,&faculty1);
    return (faculty1 == *(TechnionFaculty*)faculty);
}

static bool facultyCheck(TechnionFaculty faculty){
    return (faculty >= 0) && (faculty < FACULTY_NUM);
}

static bool isEmailAlreadyExist(EscapeTechnion system, char* email){
    assert((system != NULL) && (email != NULL));
    SET_FOREACH(Company , current_company, system->companies) {
        if (strcmp(companyGetEmail(current_company),email) == 0 ) {
            return true;
        }
    }
    SET_FOREACH(Escaper, current_escaper, system->escapers){
        if(strcmp(escaperGetEmail(current_escaper),email) == 0 ){
            return true;
        }
    }
    return false;
}

static bool isCompanyHasReservation(EscapeTechnion system, Company company){
    assert(system && company);
    LIST_FOREACH(Order, current_order, system->orders){
        assert(orderGetCompany(current_order));
        if(companyCompare(orderGetCompany(current_order),company) == 0 ){
            return true;
        }
    }
    return false;
}

static bool isRoomHasReservation(EscapeTechnion system, Room room){
    LIST_FOREACH(Order, currrent_order, system->orders){
        if(roomCompare(orderGetRoom(currrent_order),room) == 0){
            return true;
        }
    }
    return false;
}

static Company getCompany(EscapeTechnion system, char *email){
    assert(system && email);
    SET_FOREACH(Company,current_company,system->companies){
        if(strcmp(companyGetEmail(current_company),email) == 0){
            return current_company;
        }
    }
    return NULL;
}

static bool isIdAlreadyExistInFaculty(EscapeTechnion system, Room room){
    assert(system && room;
    SET_FOREACH(Room , current_room, system->rooms){
        if(roomCompare(current_room,room)==0){
            return true;
        }
    }
    return false;
}

static  EscapeTechnionResult convertFromCompanyResult(CompanyResult result){
    switch(result){
        case COMPANY_SUCCESS:
            return ESCAPE_TECHNION_SUCCESS;
        case COMPANY_ROOM_ALREADY_EXISTS:
            return ESCAPE_TECHNION_ID_ALREADY_EXIST;
        case COMPANY_ROOM_DOES_NOT_EXIST:
            return ESCAPE_TECHNION_ID_DOES_NOT_EXIST;
        case COMPANY_NULL_ARGUMENT:
            return ESCAPE_TECHNION_NULL_PARAMETER;
        case COMPANY_OUT_OF_MEMORY:
            return ESCAPE_TECHNION_OUT_OF_MEMORY;
        case COMPANY_INVALID_PARAMETER:
            return ESCAPE_TECHNION_INVALID_PARAMETER;
    }
}

static EscapeTechnionResult convertFromRoomResult(RoomResult result){
    switch(result){
        case ROOM_SUCCESS:
            return ESCAPE_TECHNION_SUCCESS;
        case ROOM_NULL_ARGUMENT:
            return ESCAPE_TECHNION_NULL_PARAMETER;
        case ROOM_OUT_OF_MEMORY:
            return ESCAPE_TECHNION_OUT_OF_MEMORY;
        case ROOM_INVALID_PARAMETER:
            return ESCAPE_TECHNION_INVALID_PARAMETER;
    }
}

static EscapeTechnionResult convertFromEscaperResult(EscaperResult result){
    switch (result){
        case ESCAPER_INVALID_PARAMETER:
            return ESCAPE_TECHNION_INVALID_PARAMETER;
        case ESCAPER_NULL_ARGUMENT:
            return ESCAPE_TECHNION_NULL_PARAMETER;
        case ESCAPER_OUT_OF_MEMORY:
            return ESCAPE_TECHNION_OUT_OF_MEMORY;
        case ESCAPER_SUCCESS:
            return ESCAPE_TECHNION_SUCCESS;
    }
}

static long calculate_total_profit(EscapeTechnion system){
    assert(system);
    long profit = 0;
    for(int i = 0 ; i < FACULTY_NUM ; i++){
        profit += system->faculty_profit[i];
    }
    return profit;
}

static void findBestFaculties(EscapeTechnion system, TechnionFaculty* faculty){
    assert(system && faculty);
    long best_profit[BEST_FACULTIES_NUM];
    for(int i = 0 ; i < BEST_FACULTIES_NUM ; i++){
        best_profit[i] = -1;
    }
    for(int i = 0 ; i < FACULTY_NUM ; i++){
        for(int j = 0 ; j < BEST_FACULTIES_NUM ; j++) {
            if (system->faculty_profit[i] > best_profit[i]) {
                for(int h = BEST_FACULTIES_NUM - 1 ; h >= j+1 ; h--){
                    *(faculty+h) = *(faculty+h-1);
                }
                *(faculty+j) = (TechnionFaculty)i;
            }
        }
    }
    return;
}

static void removeClientOrders(EscapeTechnion system, Escaper escaper){
    if(!system || !escaper){
        return;
    }
    LIST_FOREACH(Order, current_order,system->orders){
        if(EscaperCompare(orderGetEscaper(current_order),escaper) == 0){
            listRemoveCurrent(system->orders);
        }
    }
    return;
}



