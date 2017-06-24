#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include "EscapeTechnion.h"
#include <limits.h>

/**===============================Macros======================================*/
#define FACULTY_NUM ((int)UNKNOWN)

#define MEMORY_CHECK(condition,system_ptr_to_destroy) \
    if(!(condition)) { \
        escapeTechnionDestroy(system_ptr_to_destroy); \
        return ESCAPE_TECHNION_OUT_OF_MEMORY; \
    } \

#define NULL_ARGUMENT_CHECK(condition) \
    if(!(condition)) { \
        return ESCAPE_TECHNION_NULL_PARAMETER; \
    } \

/**====================Main entity decleration================================*/

struct EscapeTechnion_t{
    CompanySet companies;
    OrdersList orders;
    EscaperSet escapers;
    int day;
    long faculty_profit[FACULTY_NUM];
};
/**=====================Static functions declarations=========================*/

static bool facultyCheck(TechnionFaculty faculty);
static bool isEmailAlreadyExist(EscapeTechnion system, char* email);
static EscapeTechnionResult isCompanyHasReservation(EscapeTechnion system,
                                                    Company company);
static bool isIdAlreadyExistInFaculty(EscapeTechnion system,
                                      TechnionFaculty faculty, long id);
static bool isRoomHasReservations(EscapeTechnion sys, Room room,
                                  TechnionFaculty rooms_faculty);
static EscapeTechnionResult convertFromCompanyResult( CompanyResult result);
static EscapeTechnionResult convertFromEscaperResult(EscaperResult result);
static EscapeTechnionResult convertFromRoomResult(RoomResult result);
static EscapeTechnionResult removeClientOrders(EscapeTechnion system,
                                               Escaper escaper);
static EscapeTechnionResult getEscaper(EscapeTechnion system, char *email,
                                       Escaper* escaper);
static EscapeTechnionResult getRoom(
        EscapeTechnion system, TechnionFaculty faculty, long id, Room* room);
static EscapeTechnionResult convertFromOrderResult(OrderResult result);
static bool isRoomAvailable(EscapeTechnion system,long system_day,
                            long hour,long id,Room room,
                            TechnionFaculty room_faculty);
static bool isClientAvailable(EscapeTechnion system,long day,
                              long hour,Escaper client);
static long CalculateRecommendedFormula(long P_r,long P_e,
                                          long difficulty,long skill_level);
static void GetRoomNextAvailabilty(EscapeTechnion system, Room room,
                                   long *next_available_hour,
                                   long* next_avialable_day,
                                   TechnionFaculty faculty);
static bool isOrderForDay(ListElement order, ListFilterKey key);
static bool isOrderNotForDay(ListElement order, ListFilterKey key);
static bool isEmailLegal(char* email);
static bool isBetterMatch(TechnionFaculty checked_faculty,
                          TechnionFaculty recommended_faculty,
                          TechnionFaculty escaper_faculty,long checked_room_id,
                          long rec_room_id);
static EscapeTechnionResult getCompany(EscapeTechnion system, char *email,
                                Company *company);

/**===================System ADT functions implementation=====================*/

/**-------------------------System Create-------------------------------------*/
EscapeTechnionResult escapeTechnionCreate(EscapeTechnion *system){
    assert(system);
    *system = malloc(sizeof(**system));
    if(!*system){
        return ESCAPE_TECHNION_OUT_OF_MEMORY;
    }
    (*system)->companies = setCreate(companyCopy,companyDestroy,companyCompare);
    MEMORY_CHECK((*system)->companies, *system);
    (*system)->escapers = setCreate(escaperCopy,escaperDestroy,escaperCompare);
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
    assert(system);
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
    assert(system);
    if(!email){
        return ESCAPE_TECHNION_NULL_PARAMETER;
    }
    if(!isEmailLegal(email)){
        return ESCAPE_TECHNION_INVALID_PARAMETER;
    }
    SET_FOREACH(Company,company,system->companies){//for each company
        char* company_email = NULL;
        convertFromCompanyResult(companyGetEmail(company,&company_email));
        //get mail
        if(!company_email){
            return ESCAPE_TECHNION_OUT_OF_MEMORY;
        }
        if(strcmp(company_email,email) == 0){//if we found the company
            free(company_email);//
            EscapeTechnionResult result =
                    isCompanyHasReservation(system,company);
            if(result != ESCAPE_TECHNION_SUCCESS) {
                return result;
            }
            setRemove(system->companies,company);
            return ESCAPE_TECHNION_SUCCESS;
        }
        free(company_email);
    }
    return ESCAPE_TECHNION_COMPANY_EMAIL_DOES_NOT_EXIST;
}

/**----------------------System Add Room--------------------------------------*/
EscapeTechnionResult escapeTechnionAddRoom(
        EscapeTechnion system, char *email, long id, long price,int num_ppl,
        int open_hour, int close_hour, int difficulty){
    assert(system);
    if(!email){
        return ESCAPE_TECHNION_NULL_PARAMETER;
    }
    Room room;
    EscapeTechnionResult result = convertFromRoomResult(
            roomCreate(id,price,num_ppl,open_hour,close_hour,difficulty,&room));
    if(result != ESCAPE_TECHNION_SUCCESS){
        return result;
    }
    Company company = NULL;
    result = getCompany(system, email, &company);
    if(result != ESCAPE_TECHNION_SUCCESS){
        roomDestroy(room);
        return result;
    }
    TechnionFaculty faculty = companyGetFaculty(company);//get company's faculty
    if(isIdAlreadyExistInFaculty(system, faculty, id)){
        roomDestroy(room);
        return ESCAPE_TECHNION_ID_ALREADY_EXIST;
    }
    result = convertFromCompanyResult(companyAddRoom(company,room));
    roomDestroy(room);
    return result;
}

/**----------------------System Remove Room-----------------------------------*/
EscapeTechnionResult escapeTechnionRemoveRoom(EscapeTechnion system,
                                              TechnionFaculty faculty, long id){
    assert(system);
    if((faculty >= FACULTY_NUM) || (faculty < 0) || (id < 1)){
        return ESCAPE_TECHNION_INVALID_PARAMETER;
    }
    SET_FOREACH(Company,current_company,system->companies){
        TechnionFaculty company_faculty = companyGetFaculty(current_company);
        if(company_faculty == faculty) {
            Room room;
            CompanyResult company_result = companyFindRoom(
                    current_company,id,&room);
            if(company_result == COMPANY_SUCCESS){
                if(isRoomHasReservations(system, room, faculty)==true){
                    roomDestroy(room);
                    return ESCAPE_TECHNION_RESERVATION_EXISTS;
                }
                else{
                    company_result = companyRemoveRoom(current_company,room);
                    roomDestroy(room);
                    return convertFromCompanyResult(company_result);
                }
            }
        }
    }
    return ESCAPE_TECHNION_ID_DOES_NOT_EXIST;
}

/**---------------------Escape Technion Add Client----------------------------*/
EscapeTechnionResult escapeTechnionAddClient(EscapeTechnion system, char* email,
                                             TechnionFaculty faculty,
                                             int skill_level){
    assert(system);
    if(!email){
        return ESCAPE_TECHNION_NULL_PARAMETER;
    }
    Escaper escaper;
    EscapeTechnionResult result = convertFromEscaperResult(
            escaperCreate(email, faculty, skill_level, &escaper));
    if(result != ESCAPE_TECHNION_SUCCESS){
        return result;
    }
    if(isEmailAlreadyExist(system,email)){
        escaperDestroy(escaper);
        return ESCAPE_TECHNION_EMAIL_ALREADY_EXISTS;
    }
    SetResult result1 = setAdd(system->escapers,escaper);
    escaperDestroy(escaper);
    return  (result1 == SET_OUT_OF_MEMORY) ? ESCAPE_TECHNION_OUT_OF_MEMORY :
                                                ESCAPE_TECHNION_SUCCESS;
}

/**---------------------Escape Technion Remove Client-------------------------*/
EscapeTechnionResult escapeTechnionRemoveClient(EscapeTechnion system,
                                                char* email){
    assert(system);
    if(!email){
        return ESCAPE_TECHNION_NULL_PARAMETER;
    }
    Escaper escaper;
    EscapeTechnionResult result = getEscaper(system,email,&escaper);
    if(result != ESCAPE_TECHNION_SUCCESS){
        return result;
    }
    result = removeClientOrders(system,escaper);
    if(result != ESCAPE_TECHNION_SUCCESS){
        escaperDestroy(escaper);
        return result;
    }
    assert(setIsIn(system->escapers,escaper));
    setRemove(system->escapers,escaper);
    escaperDestroy(escaper);
    return ESCAPE_TECHNION_SUCCESS;
}

/**-----------------------Escape Technion Add Order---------------------------*/
EscapeTechnionResult escapeTechnionAddOrder(EscapeTechnion system, char* email,
                                            TechnionFaculty faculty, long id,
                                            int day_left,int hour,int num_ppl){
    assert(system);
    if(!email){
        return ESCAPE_TECHNION_NULL_PARAMETER;
    }
    if(!ordercheckInput(day_left,hour,num_ppl,faculty)||id<=0){
        return ESCAPE_TECHNION_INVALID_PARAMETER;
    }
    Escaper escaper;
    EscapeTechnionResult result = getEscaper(system, email, &escaper);
    if(result != ESCAPE_TECHNION_SUCCESS ){
        return result;
    }
    Room room;
    result = getRoom(system,faculty,id,&room);
    if(result != ESCAPE_TECHNION_SUCCESS){
        return result;
    }
    Order order;
    result = convertFromOrderResult(
            orderCreate(num_ppl,hour,day_left+system->day,faculty,room,escaper,
                        &order));
    roomDestroy(room);
    escaperDestroy(escaper);
    if(result != ESCAPE_TECHNION_SUCCESS){
        return result;
    }
    if(!isClientAvailable(system,day_left+system->day,hour,
                          orderGetEscaper(order))){
        orderDestroy(order);
        return ESCAPE_TECHNION_CLIENT_IN_ROOM;
    }
    if(!isRoomAvailable(system,day_left+system->day,hour,id,orderGetRoom(order),
                        faculty)) {
        orderDestroy(order);
        return ESCAPE_TECHNION_ROOM_NOT_AVAILABLE;
    }
    ListResult result1 = listInsertLast(system->orders,order);
    orderDestroy(order);
    return (result1 == LIST_OUT_OF_MEMORY) ? ESCAPE_TECHNION_OUT_OF_MEMORY
                                          : ESCAPE_TECHNION_SUCCESS;
}

/**--------------Escape Technion  Recommended Room order---------------*/
EscapeTechnionResult escapeTechnionRecommendedRoomOrder(EscapeTechnion system,
                                                        char* mail,
                                                        long num_ppl) {
    Order recommended_order =NULL;
    if(num_ppl<=0){
        return ESCAPE_TECHNION_INVALID_PARAMETER;
    }
    long best_barometer = LONG_MAX;
    NULL_ARGUMENT_CHECK(system && mail);//not null//
    Escaper client ;//find escaper
    EscapeTechnionResult res=getEscaper(system,mail,&client);
    if(res!=ESCAPE_TECHNION_SUCCESS){
        return res;
    }
    long skill_level = escaperGetSkillLevel(client);
    SET_FOREACH(Company, company, system->companies) {//for each company
        RoomSet roomSet = companyGetRoomsSet(company);
        TechnionFaculty current_company_faculty = companyGetFaculty(company);
        SET_FOREACH(Room,room,roomSet) {//for each room in company
            long cur_room_dif = roomGetDiffuclty(room);
            long cur_room_recommended = roomGetRecommendedNumOfPeople(room);
            long barometer = CalculateRecommendedFormula(cur_room_recommended,
                                                         num_ppl, cur_room_dif,
                                                         skill_level);
            TechnionFaculty checked_faculty,recommended_faculty,escaper_faculty;
            long order_room_id=0;
            if(recommended_order!=NULL){
                recommended_faculty = orderGetFaculty(recommended_order);
                order_room_id=orderGetRoomId(recommended_order);
            }
            checked_faculty = companyGetFaculty(company);
            escaper_faculty = escaperGetFaculty(client);
            //calculate barometer for current room
            if ((barometer < best_barometer)||
                    ((barometer==best_barometer)&& isBetterMatch(checked_faculty,
             recommended_faculty,escaper_faculty,
                            roomGetId(room),order_room_id))){//if it is better
                best_barometer = barometer;//update best barometer
                long available_hour, available_day;
                GetRoomNextAvailabilty(system,room,&available_hour,
                                       &available_day,checked_faculty);
                //get avilabilty
                orderDestroy(recommended_order);//destroy previous order
                orderCreate(num_ppl, available_hour,
                            available_day-escapeTechnionGetDay(system),
                            current_company_faculty,room,
                            client,&recommended_order);
            }
        }
        setDestroy(roomSet);
    }

    if(recommended_order!=NULL){
        TechnionFaculty faculty = orderGetFaculty(recommended_order);
        long id = orderGetRoomId(recommended_order);
        escapeTechnionAddOrder(system,mail,faculty,id,
                               orderGetDay(recommended_order),
                orderGetHour(recommended_order),num_ppl);
        escaperDestroy(client);
        orderDestroy(recommended_order);
        return ESCAPE_TECHNION_SUCCESS;
    }
    else{
        escaperDestroy(client);
        orderDestroy(recommended_order);
        return ESCAPE_TECHNION_NO_ROOMS_AVAILABLE;
    }
}

/**--------------Escape Technion Get Company By Room And Faculty--------------*/
Company escapeTechnionFindCompanyByRoomAndFaculty(EscapeTechnion system,
                                                  Room room,
                                                  TechnionFaculty faculty){
    SET_FOREACH(Company,company,system->companies){
        TechnionFaculty company_faculty = companyGetFaculty(company);
        if(faculty == company_faculty){
            if(companySearchRoom(company,roomGetId(room)) == COMPANY_SUCCESS){
                return company;
            }
        }
    }
    return NULL;
}

/**------------------------Escape Technion Get Faculty Profit-----------------*/
long escapeTechnionGetFacultyProfit(EscapeTechnion system,
                                    TechnionFaculty faculty){
    assert(system);
    if(!facultyCheck(faculty)) {
        return -1;
    }
    return system->faculty_profit[(int)faculty];
}

/**------------------------Escape Technion Increase Day-----------------------*/
void escapeTechnionIncreaseDay(EscapeTechnion system){
    assert(system);
    system->day++;
    return;
}

/**-----------------Escape Technion Get Day-----------------------------------*/
int escapeTechnionGetDay(EscapeTechnion system){
    assert(system);
    return system->day;
}

/**----------------Escape Technion Get Today Orders List----------------------*/
OrdersList escapeTechnionGetTodayOrdersList(EscapeTechnion system){
    assert(system);
    OrdersList filtered_list = listFilter(system->orders,isOrderForDay,
                                          &system->day);
    if(!filtered_list){
        return NULL;
    }/*
    if(listSort(filtered_list,orderCompareByRoomId) != LIST_SUCCESS ||
            listSort(filtered_list,orderCompareByFaculty) != LIST_SUCCESS ||
            listSort(filtered_list,orderCompare) != LIST_SUCCESS){
        listDestroy(filtered_list);
        return NULL;
    }*/
    listSort(filtered_list,orderCompareByCritiria);
    OrdersList new_system_list = listFilter(
            system->orders,isOrderNotForDay,&system->day);
    if(!new_system_list){
        listDestroy(filtered_list);
        return NULL;
    }
    listDestroy(system->orders);
    system->orders = new_system_list;
    return filtered_list;
}
/**-------------Escape Technion Calculate Total Revenue-----------------------*/
long escapeTechnionCalculateTotalRevenue(EscapeTechnion system){
    assert(system);
    long total_revenue = 0;
    for(int i=0 ; i < FACULTY_NUM ; i++){
        total_revenue += escapeTechnionGetFacultyProfit(
                system,(TechnionFaculty)i);
    }
    return total_revenue;
}

/**--------------------Escape Technion Add Profit To Faculty------------------*/
void escapeTechnionAddProfitToFaculty(EscapeTechnion system, long profit,
                                      TechnionFaculty faculty){
    assert(system);
    if(facultyCheck(faculty)){
        system->faculty_profit[(int)faculty]+=profit;
    }
    return;
}

/**===================Static functions implementation=========================*/
static EscapeTechnionResult isCompanyHasReservation(EscapeTechnion system,
                                                    Company company){
    TechnionFaculty company_faculty = companyGetFaculty(company);
    OrdersList list = listFilter(system->orders,orderIsSameFaculty,
                                 &company_faculty);
    //filter orders list by faculty
    if(!list){//memory problem
        return ESCAPE_TECHNION_OUT_OF_MEMORY;
    }
    long list_size=listGetSize(list);
    if(list_size== 0){//no orders for this faculty
        listDestroy(list);
        return ESCAPE_TECHNION_SUCCESS;
    }
    /*else
        return ESCAPE_TECHNION_RESERVATION_EXISTS;*/

    RoomSet rooms_set = companyGetRoomsSet(company);
    if(!rooms_set){
        listDestroy(list);
        return ESCAPE_TECHNION_OUT_OF_MEMORY;
    }
    SET_FOREACH(Room,room,rooms_set){
        long id = roomGetId(room);
        OrdersList filtered_list = listFilter(list,orderIsSameId,&id);
        if(!filtered_list){
            listDestroy(list);
            setDestroy(rooms_set);
            return ESCAPE_TECHNION_OUT_OF_MEMORY;
        }
        if(listGetSize(filtered_list) > 0 ){
            setDestroy(rooms_set);
            listDestroy(filtered_list);
            listDestroy(list);
            return ESCAPE_TECHNION_RESERVATION_EXISTS;
        }
        listDestroy(filtered_list);
    }
    listDestroy(list);
    setDestroy(rooms_set);
    return ESCAPE_TECHNION_SUCCESS;

}

static bool isRoomHasReservations(EscapeTechnion sys, Room room,
                                  TechnionFaculty rooms_faculty){
    LIST_FOREACH(Order,order,sys->orders){ //get orders faculty
        if(orderGetFaculty(order) == rooms_faculty){ //if its the same faculty
            Room orders_room = orderGetRoom(order); //get orders room
            long orders_room_id = roomGetId(orders_room); //get orders room id
            if(orders_room_id == roomGetId(room)){//if same id
                return true;
            }
        }
    }
    return  false;
}

static bool facultyCheck(TechnionFaculty faculty){
    return (faculty >= 0) && (faculty < FACULTY_NUM);
}

static bool isEmailAlreadyExist(EscapeTechnion system, char* email){
    assert((system != NULL) && (email != NULL));
    SET_FOREACH(Company , company, system->companies) {
        char* company_email;
        companyGetEmail(company,&company_email);
        if(strcmp(company_email,email) == 0 ){
            free(company_email);
            return true;
        }
        free(company_email);
    }
    SET_FOREACH(Escaper, escaper, system->escapers){
        char* escaper_email;
        escaperGetEmail(escaper,&escaper_email);
        if(strcmp(escaper_email,email) == 0 ){
            free(escaper_email);
            return true;
        }
        free(escaper_email);
    }
    return false;
}

static EscapeTechnionResult getCompany(EscapeTechnion system, char *email,
                                Company *company){
    assert(system);
    if(!email || !company){
        return ESCAPE_TECHNION_NULL_PARAMETER;
    }
    if(!isEmailLegal(email)){
        return ESCAPE_TECHNION_INVALID_PARAMETER;
    }
    SET_FOREACH(Company,current_company,system->companies){
        char *current_company_mail;
        companyGetEmail(current_company,&current_company_mail);
        if(strcmp(current_company_mail,email) == 0){
            *company = current_company;
            free(current_company_mail);
            return ESCAPE_TECHNION_SUCCESS;
        }
        free(current_company_mail);
    }
    return ESCAPE_TECHNION_COMPANY_EMAIL_DOES_NOT_EXIST;
}

static bool isIdAlreadyExistInFaculty(EscapeTechnion system,
                                      TechnionFaculty faculty, long id){
    assert(system);
    CompanyResult result;
    SET_FOREACH(Company ,current_company,system->companies){//for each company.
        if(faculty == companyGetFaculty(current_company)){//if same faculty.
            result = companySearchRoom(current_company, id);//
            if(result == COMPANY_SUCCESS){//and same id.
                return true;
            }
        }
    }
    return false;
}

static  EscapeTechnionResult convertFromCompanyResult(CompanyResult result){
    switch(result){
        case COMPANY_SUCCESS:
            return ESCAPE_TECHNION_SUCCESS;
        case COMPANY_ROOM_DOES_NOT_EXIST:
            return ESCAPE_TECHNION_ID_DOES_NOT_EXIST;
        case COMPANY_OUT_OF_MEMORY:
            return ESCAPE_TECHNION_OUT_OF_MEMORY;
        case COMPANY_INVALID_PARAMETER:
            return ESCAPE_TECHNION_INVALID_PARAMETER;

        default:
            return ESCAPE_TECHNION_NULL_PARAMETER;
    }
}
static EscapeTechnionResult convertFromEscaperResult(EscaperResult result){
    switch (result){
        case ESCAPER_INVALID_PARAMETER:
            return ESCAPE_TECHNION_INVALID_PARAMETER;

        case ESCAPER_OUT_OF_MEMORY:
            return ESCAPE_TECHNION_OUT_OF_MEMORY;
        case ESCAPER_SUCCESS:
            return ESCAPE_TECHNION_SUCCESS;

        default:
            return ESCAPE_TECHNION_NULL_PARAMETER;
    }
}

static EscapeTechnionResult convertFromOrderResult(OrderResult result){
    switch (result){
        case ORDER_SUCCESS:
            return ESCAPE_TECHNION_SUCCESS;
        case ORDER_INVALID_PARAMETER:
            return ESCAPE_TECHNION_INVALID_PARAMETER;
        case ORDER_OUT_OF_MEMORY:
            return ESCAPE_TECHNION_OUT_OF_MEMORY;
        case ORDER_ESCAPER_IS_NULL:
            return ESCAPE_TECHNION_CLIENT_EMAIL_DOES_NOT_EXIST;
        case ORDER_ROOM_IS_NULL:
            return ESCAPE_TECHNION_ID_DOES_NOT_EXIST;

        default:
            return ESCAPE_TECHNION_NULL_PARAMETER;
    }
}
static EscapeTechnionResult convertFromRoomResult(RoomResult result){
    switch (result){
        case ROOM_SUCCESS:
            return ESCAPE_TECHNION_SUCCESS;
        case ROOM_INVALID_PARAMETER:
            return ESCAPE_TECHNION_INVALID_PARAMETER;
        case ROOM_OUT_OF_MEMORY:
            return ESCAPE_TECHNION_OUT_OF_MEMORY;

        default:
            return ESCAPE_TECHNION_NULL_PARAMETER;
    }
}
/*
static long calculate_total_profit(EscapeTechnion system){
    assert(system);
    long profit = 0;
    for(int i = 0 ; i < FACULTY_NUM ; i++){
        profit += system->faculty_profit[i];
    }
    return profit;
}
*/
void escapeTechnionGetBestFaculties(EscapeTechnion system,
                                                    TechnionFaculty *faculties,
                                                    int best_faculties_num){
    assert(system);
    long faculties_profit[FACULTY_NUM];
    for(int i=0; i<FACULTY_NUM ;i++){
        faculties_profit[i] = system->faculty_profit[i];
    }
    long best_profit[best_faculties_num];
    for(int i = 0 ; i < best_faculties_num ; i++){
        for(int j=0 ; j < FACULTY_NUM ; j++){
            if(j == 0){
                best_profit[i] = faculties_profit[j];
                faculties[i] = (TechnionFaculty)j;
            }
            else if(faculties_profit[j] > best_profit[i]){
                best_profit[i] = faculties_profit[j];
                faculties[i] = (TechnionFaculty)j;
            }
        }
        faculties_profit[(int)faculties[i]] = -1;
    }
    return ;
}

static EscapeTechnionResult removeClientOrders(EscapeTechnion system,
                                               Escaper escaper){
    assert(system && escaper);
    OrdersList new_list = listFilter(system->orders,orderNotBelongToClient,
                                     escaper);
    //long new_list_size=listGetSize(new_list);
    if(!new_list){
        return ESCAPE_TECHNION_OUT_OF_MEMORY;
    }
    listDestroy(system->orders);
    system->orders = new_list;
    return ESCAPE_TECHNION_SUCCESS;
}

static EscapeTechnionResult getEscaper(EscapeTechnion system, char *email,
                                       Escaper* escaper){
    assert(system);
    if(!isEmailLegal(email)){
        return ESCAPE_TECHNION_INVALID_PARAMETER;
    }
    SET_FOREACH(Escaper,current_escaper,system->escapers){
        char* escaper_email;
        if(escaperGetEmail(current_escaper,&escaper_email) ==
                ESCAPER_OUT_OF_MEMORY) {
            return ESCAPE_TECHNION_OUT_OF_MEMORY;
        }
        if(strcmp(email,escaper_email) == 0){
            free(escaper_email);
            *escaper = escaperCopy(current_escaper);
            if(!*escaper){
                return ESCAPE_TECHNION_OUT_OF_MEMORY;
            }
            return ESCAPE_TECHNION_SUCCESS;
        }
        free(escaper_email);
    }
    *escaper = NULL;
    return ESCAPE_TECHNION_CLIENT_EMAIL_DOES_NOT_EXIST;
}

static EscapeTechnionResult getRoom(
        EscapeTechnion system, TechnionFaculty faculty, long id, Room* room){
    assert(system);
    EscapeTechnionResult result;
    if((faculty < 0) || (faculty >= (UNKNOWN)) || (id <= 0)){
        *room = NULL;
        return ESCAPE_TECHNION_INVALID_PARAMETER;
    }
    SET_FOREACH(Company,company,system->companies){
        if(faculty == companyGetFaculty(company)){
            result = convertFromCompanyResult(companyFindRoom(company,id,room));
            if(result != ESCAPE_TECHNION_ID_DOES_NOT_EXIST){
                return result;
            }
        }
    }
    *room = NULL;
    return ESCAPE_TECHNION_ID_DOES_NOT_EXIST;
}

static bool isRoomAvailable(EscapeTechnion system, long system_day, long hour,
                            long id, Room room, TechnionFaculty faculty){
    assert(system);
    long open_hour = roomGetOpenHour(room),close_hour = roomGetCloseHour(room);
    if(hour < open_hour || hour >= close_hour){
        return false;
    }
    LIST_FOREACH(Order,cur_order,system->orders){
        //Room current_room = orderGetRoom(cur_order);
        TechnionFaculty orders_faculty = orderGetFaculty(cur_order);
        if ((orderGetRoomId(cur_order)==id)&&(faculty==orders_faculty)){
            if((orderGetDay(cur_order)==system_day)&&
                    (orderGetHour(cur_order)==hour)){
                return false;
            }
        }
    }//
    return true;
}

inline static bool checkAddOrderInput(int day, int hour, int num_of_ppl,
                               TechnionFaculty faculty,char* mail,long id){
    assert(mail!=NULL);
    char* ptr=mail;
    int counter=0;
    while(*ptr){
        if(*ptr=='@')counter++;
        ptr++;
    }
    return ( (counter==1)&&(day >= 0) && (hour >= 0)
             && (hour < MAX_HOUR) && (num_of_ppl > 0)
             && (faculty<FACULTY_NUM-1)&&(faculty>=0)&& (id>0) );
}

static bool isClientAvailable(EscapeTechnion system,long day,
                            long hour,Escaper client) {
    assert(system);
    LIST_FOREACH(Order, order, system->orders) {
        Escaper current_client = orderGetEscaper(order);
        if (escaperCompare(current_client,client)==0) {
            if (orderGetDay(order) == day && orderGetHour(order) == hour) {
                return false;
            }
        }

    }
    return true;
}


static bool isOrderForDay(ListElement order, ListFilterKey key) {
    assert(order);
    int day = orderGetDay((Order)order);
    return  day == *(int*)key;
}

static bool isOrderNotForDay(ListElement order, ListFilterKey key) {
    assert(order);
    int day = orderGetDay((Order)order);
    return  day != *(int*)key;
}

static long CalculateRecommendedFormula(long P_r,long P_e,
                                          long difficulty,long skill_level){
    long tmp1=0,tmp2;
    tmp1=P_r-P_e;
    tmp2=difficulty-skill_level;
    tmp1=tmp1*tmp1;
    tmp2=tmp2*tmp2;
    return tmp1+tmp2;
}

static void GetRoomNextAvailabilty(EscapeTechnion system,Room room,
                            long *next_available_hour,long* next_avialable_day,
                                   TechnionFaculty faculty) {
    long wanted_hour, day = escapeTechnionGetDay(system), id = roomGetId(room);
    long close_hour = roomGetCloseHour(room),open_hour = roomGetOpenHour(room);
    wanted_hour = open_hour;
    while (!isRoomAvailable(system, day, wanted_hour, id, room,faculty)){
        //find next avilability
        if (wanted_hour < close_hour) {
            wanted_hour++;
        } else {
            wanted_hour = open_hour;
            day++;
        }
    }
    *next_available_hour = wanted_hour;
    *next_avialable_day = day;
}

static bool isEmailLegal(char* email){
    assert(email);
    char* ptr=email;
    int counter=0;
    while(*ptr){
        if((*ptr) == '@'){
            counter++;
        }
        ptr++;
    }
    return (counter==1)?true:false;
}

static bool isBetterMatch(TechnionFaculty checked_faculty,
                          TechnionFaculty recommended_faculty,
                          TechnionFaculty escaper_faculty,long checked_room_id,
                          long rec_room_id){
    int checked_faculty_distance=abs(escaper_faculty-checked_faculty);
    int recommended_faculty_distance=abs(recommended_faculty-escaper_faculty);
    if(checked_faculty_distance==recommended_faculty_distance){
        if(checked_faculty==recommended_faculty){
            return ((checked_room_id<rec_room_id) ? true:false);
        }
        return (checked_faculty<recommended_faculty) ? true : false;
    }
    if(checked_faculty_distance<recommended_faculty_distance)return true;
    return false;


}