#include <assert.h>
#include <string.h>
#include <stdlib.h>

#include "EscapeTechnion.h"

/**===============================Macros======================================*/
#define FACULTY_NUM ((int)UNKNOWN)
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
static bool isCompanyHasReservation(EscapeTechnion system, Company company);
static bool isIdAlreadyExistInFaculty(EscapeTechnion system,
                                      TechnionFaculty faculty, long id);
static bool doesRoomHaveOrders(EscapeTechnion sys, Room room,
                               TechnionFaculty rooms_fac);
/*static EscapeTechnionResult checkRoomReservations(EscapeTechnion system,
                                                  Company company, long id);*/
static EscapeTechnionResult convertFromCompanyResult( CompanyResult result);
static EscapeTechnionResult convertFromEscaperResult(EscaperResult result);
static EscapeTechnionResult convertFromRoomResult(RoomResult result);
static long calculate_total_profit(EscapeTechnion system);
static void findBestFaculties(EscapeTechnion system, TechnionFaculty* faculty);
static void removeClientOrders(EscapeTechnion system, Escaper escaper);
static Escaper getEscaper(EscapeTechnion system, char *email);
static Room getRoom(EscapeTechnion system, TechnionFaculty faculty,long id);
static EscapeTechnionResult convertFromOrderResult(OrderResult result);
static bool isRoomAvailable(EscapeTechnion system,long day,
                            long hour,long id,Room room);
inline static bool checkAddOrderInput(int day, int hour, int num_of_ppl,
                                      TechnionFaculty faculty,char* mail,long id);
static bool isClientAvailable(EscapeTechnion system,long day,
                              long hour,Escaper client);
static long CalculateRecommendedFormula(long P_r,long P_e,
                                          long difficulty,long skill_level);
static void GetRoomNextAvailabilty(EscapeTechnion system,Room room,
                                   long *next_available_hour,long* next_avialable_day);
static bool isOrderForDay(ListElement order, ListFilterKey key);
static bool isEmailLegal(char* email);
static bool isFacultyNearer(TechnionFaculty checked_faculty,
                            TechnionFaculty recommended_faculty,
                            TechnionFaculty escaper_faculty);
static EscapeTechnionResult getCompany(EscapeTechnion system, char *email,
                                Company *company);

/**===================System ADT functions implementation=====================*/

/**-------------------------System Create-------------------------------------*/
EscapeTechnionResult escapeTechnionCreate(EscapeTechnion *system){
    NULL_ARGUMENT_CHECK(system);
    *system = malloc(sizeof(**system));
    MEMORY_CHECK(*system, *system);
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
    if(result!=ESCAPE_TECHNION_SUCCESS){
       // companyDestroy(company);
        return result;
    }
    if(!setIsIn(system->companies,company)){
        companyDestroy(company);
        return ESCAPE_TECHNION_COMPANY_EMAIL_DOES_NOT_EXIST;
    }
    if(isCompanyHasReservation(system, company)==true){
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
    NULL_ARGUMENT_CHECK(system && email);
    Room room;
    EscapeTechnionResult result = convertFromRoomResult(
            roomCreate(id,price,num_ppl,open_hour,close_hour,difficulty,&room));
    if(result != ESCAPE_TECHNION_SUCCESS){
        return result;
    }
    Company company;
    result = getCompany(system, email, &company);
    if(result != ESCAPE_TECHNION_SUCCESS){
        roomDestroy(room);
        return result;
    }
    TechnionFaculty faculty;
    companyGetFaculty(company,&faculty);//get company's faculty
    if(isIdAlreadyExistInFaculty(system, faculty, id)==true){
        return ESCAPE_TECHNION_ID_ALREADY_EXIST;
    }
    result = convertFromCompanyResult(companyAddRoom(company,room));
    if(result!=ESCAPE_TECHNION_SUCCESS) {
        roomDestroy(room);
        return result;
    }
    roomDestroy(room);
    return ESCAPE_TECHNION_SUCCESS;
}

/**----------------------System Remove Room-----------------------------------*/

EscapeTechnionResult escapeTechnionRemoveRoom(EscapeTechnion system,
                                              TechnionFaculty faculty, long id){
    if(system==NULL) return ESCAPE_TECHNION_NULL_PARAMETER;
    if((faculty >= FACULTY_NUM) || (faculty < 0) || (id < 1)){//input check
        return ESCAPE_TECHNION_INVALID_PARAMETER;
    }
    SET_FOREACH(Company,current_company,system->companies){//for each company
        TechnionFaculty current_company_faculty;
        companyGetFaculty(current_company,&current_company_faculty);//get faculty
        if(current_company_faculty == faculty) {//if its the same faculty
            Room room;
            CompanyResult companyResult;
            companyResult=companyFindRoom((Company)current_company,id,&room);//search company for room id
            if(companyResult == COMPANY_SUCCESS){//found the room
                if(doesRoomHaveOrders(system, room, faculty)==true){//if room has orders
                    return ESCAPE_TECHNION_RESERVATION_EXISTS;
                }
                else{//no order exists. its ok to delete room
                    companyRemoveRoom((Company)current_company,id);
                    return ESCAPE_TECHNION_SUCCESS;
                }

            }
        }
    }
    return ESCAPE_TECHNION_ID_DOES_NOT_EXIST;
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
    if(isEmailAlreadyExist(system,email)){
        escaperDestroy(escaper);
        return ESCAPE_TECHNION_EMAIL_ALREADY_EXISTS;
    }
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
            email, ELECTRICAL_ENGINEERING,1, &escaper));//random input
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
    if(!checkAddOrderInput(day,hour,num_ppl,faculty,email,id)){
        return ESCAPE_TECHNION_INVALID_PARAMETER;
    }
    day=day+escapeTechnionGetDay(system);
    Order order;
    Escaper escaper = getEscaper(system, email);//get the order's client
    Room room = getRoom(system,faculty,id);//get the order's room
    EscapeTechnionResult result = convertFromOrderResult(
            orderCreate(num_ppl,hour,day,faculty,room,escaper,&order));
    if(result == ESCAPE_TECHNION_NULL_PARAMETER){//cannot create order
        assert(!escaper || !room);
        return (escaper == NULL) ? ESCAPE_TECHNION_CLIENT_EMAIL_DOES_NOT_EXIST
                                 : ESCAPE_TECHNION_ID_DOES_NOT_EXIST;
        //is it an escaper problem? or a room problem?
    }
    if(result != ESCAPE_TECHNION_SUCCESS){
        return result;
    }
    //order was created;
    if(!isClientAvailable(system,day,hour,escaper)){//client unabailable
        orderDestroy(order);
        return ESCAPE_TECHNION_CLIENT_IN_ROOM;
    }
    if(!isRoomAvailable(system,day,hour,id,room)) {//room unavailable
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
    Order rec_order =NULL;
    long best_barometer = LONG_MAX;
    NULL_ARGUMENT_CHECK(system && mail);//not null//
    Escaper client = getEscaper(system, mail);//find escaper
    if(client == NULL){
        return ESCAPE_TECHNION_CLIENT_EMAIL_DOES_NOT_EXIST;
    }
    long skill_level = escaperGetSkillLevel(client);
    SET_FOREACH(Company, cur_company, system->companies) {//for each company
        RoomSet roomSet = companyGetRoomsSet((Company) cur_company);
        TechnionFaculty cur_company_faculty;
        companyGetFaculty((Company) cur_company, &cur_company_faculty);
        SET_FOREACH(Room,room,roomSet) {//for each room in cur_company
            long cur_room_dif = roomGetDiffuclty(room);
            long cur_room_recommended = roomGetRecommendedNumOfPeople(room);
            long barometer = CalculateRecommendedFormula(cur_room_recommended,
                                                         num_ppl, cur_room_dif,
                                                         skill_level);
            TechnionFaculty checked_faculty,recommended_faculty,escaper_faculty;
            orderGetFaculty(rec_order,&recommended_faculty);
            companyGetFaculty(cur_company,&checked_faculty);
            escaperGetFaculty(client,&escaper_faculty);
            //calculate barometer for current room
            if ((barometer < best_barometer)||
                    ((barometer==best_barometer)&&isFacultyNearer(checked_faculty,recommended_faculty,escaper_faculty))){//if it is better
                best_barometer = barometer;//update best barometer
                long available_hour, available_day;
                GetRoomNextAvailabilty(system,room,&available_hour,
                                       &available_day);
                //get avilabilty
                orderDestroy((void *) rec_order);//destroy previous order
                orderCreate(num_ppl, available_hour, available_day-escapeTechnionGetDay(system),
                            cur_company_faculty,room,client,&rec_order);
            }
        }
    }
    if(rec_order!=NULL){
        TechnionFaculty faculty;
        orderGetFaculty(rec_order,&faculty);
        long id = orderGetRoomId(rec_order);
        EscapeTechnionResult result;
        result=escapeTechnionAddOrder(system,mail,faculty,id,orderGetDay(rec_order),
                orderGetHour(rec_order),num_ppl);
        return ESCAPE_TECHNION_SUCCESS;
    }
    else{
        return ESCAPE_TECHNION_NO_ROOMS_AVAILABLE;
    }
}


/***/
Company escapeTechnionFindCompanyByRoomAndFaculty(EscapeTechnion system,
                                                  Room room,
                                                  TechnionFaculty faculty){
    SET_FOREACH(Company,company,system->companies){
        TechnionFaculty company_faculty;
        companyGetFaculty(company,&company_faculty);
        if(faculty == company_faculty){
            if(companySearchRoom(company,roomGetId(room)) == COMPANY_SUCCESS){
                return company;
            }
        }
    }
    return NULL;
}
/**------------------------Escape Technion Get Faculty Profit---------------*/
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

/**-----------------------Escape Technion Report Day------------------------------------------*/
/*
EscapeTechnionResult  escapeTechnionReportDay(EscapeTechnion system, const FILE* output_c){
    NULL_ARGUMENT_CHECK(system);
    //mtmPrintDayFooter(,system->day);
    ListResult result = listSort(system->orders,orderCompare);
    if(result == LIST_OUT_OF_MEMORY){
        return ESCAPE_TECHNION_OUT_OF_MEMORY;
    }
    LIST_FOREACH(Order, order, system->orders){
        //orderPrint(current_order);
        if(orderGetDay(order) == system->day){
            TechnionFaculty faculty;
            orderGetFaculty(order,&faculty);
            system->faculty_profit[(int)faculty] += orderCalculatePrice(order);
            orderDestroy(order);
        }
    }
    //mtmPrintDayFooter(,system->day);
    system->day++;
    return ESCAPE_TECHNION_SUCCESS;
}
*/
/**--------------------Escape Technion Best Faculties-------------------------*/
/*
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
*/
/**-----------------Escape Technion Get Day-----------------------------------*/
int escapeTechnionGetDay(EscapeTechnion system){
    assert(system);
    return system->day;
}

EscapeTechnionResult escapeTechnionSortOrdersByDay(EscapeTechnion system){
    assert(system);
    ListResult result = listSort(system->orders,orderCompare);
    return result == LIST_OUT_OF_MEMORY ? ESCAPE_TECHNION_OUT_OF_MEMORY
                                        : ESCAPE_TECHNION_SUCCESS;
}

/**----------------Escape Technion Get Today Orders List----------------------*/
OrdersList escapeTechnionGetTodayOrdersList(EscapeTechnion system){
    assert(system);
    ListResult result = listSort(system->orders,orderCompare);
    if(result == LIST_OUT_OF_MEMORY){
        return NULL;
    }
    OrdersList list = listFilter(system->orders,isOrderForDay,&system->day);
    listSort(list,orderCompareByFaculty);
    listSort(list,orderCompare);
    if(!list){
        return NULL;
    }
    LIST_FOREACH(Order,order,system->orders){
        if(orderGetDay(order) > system->day){
            break;
        }
        listRemoveCurrent(system->orders);
    }
    return list;
}

long escapeTechnionCalculateTotalRevenue(EscapeTechnion system){
    assert(system);
    long total_revenue = 0;
    for(int i=0 ; i < FACULTY_NUM ; i++){
        total_revenue += escapeTechnionGetFacultyProfit(
                system,(TechnionFaculty)i);
    }
    return total_revenue;
}


void escapeTechnionIncreaseFacultyProfit(EscapeTechnion system,long profit,
                                         TechnionFaculty faculty){
    assert(system);
    if(facultyCheck(faculty)){
        system->faculty_profit[(int)faculty]+=profit;
    }
    return;
}

/**===================Static functions implementation=========================*/
static bool isCompanyHasReservation(EscapeTechnion system, Company company){
    LIST_FOREACH(Order,cur_order,system->orders){//for each order in sys
        TechnionFaculty orders_faculty;
        OrderResult ord_result = orderGetFaculty(cur_order,&orders_faculty);
        if(ord_result!=ORDER_SUCCESS){
            return convertFromOrderResult(ord_result);
        }
        SET_FOREACH(Company,cur_company,system->companies) {//for each company
            TechnionFaculty companys_faculty;
            CompanyResult com_result = companyGetFaculty(
                    cur_company,&companys_faculty);
            if (com_result!=COMPANY_SUCCESS) {
                 return convertFromCompanyResult(com_result);
             }
             if (orders_faculty == companys_faculty) {//if same faculty
                 Room orders_room=orderGetRoom(cur_order);
                 long orders_room_id=roomGetId(orders_room);
                 com_result=companySearchRoom(cur_company,orders_room_id);
                 if(com_result==COMPANY_SUCCESS){//found a match
                     return true;
                 }
             }
         }
    }
    return false;
}

static bool doesRoomHaveOrders(EscapeTechnion sys, Room room,
                               TechnionFaculty rooms_fac){
    LIST_FOREACH(OrdersList,cur_order,sys->orders){
        TechnionFaculty cur_order_fac;
        orderGetFaculty((Order)cur_order,&cur_order_fac);//get orders faculty
        if(cur_order_fac==rooms_fac){//if its the same faculty
            Room  orders_room;
            orders_room=orderGetRoom((Order)cur_order);//get orders room
            long orders_room_id=roomGetId(orders_room);//get orders room id
            if(orders_room_id==roomGetId(room)){//if its same faculty and same id
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
            return true;
        }
    }
    SET_FOREACH(Escaper, escaper, system->escapers){
        char* escaper_email;
        escaperGetEmail(escaper,&escaper_email);
        if(strcmp(escaper_email,email) == 0 ){
            return true;
        }
    }
    return false;
}
/*
static EscapeTechnionResult checkCompanyHasReservation(EscapeTechnion system,
                                                       Company company){
    assert(system && company);
    Set rooms_set = companyGetRoomsSet(company);
    SET_FOREACH(Room,room,rooms_set){
        EscapeTechnionResult result = checkRoomReservations(
                system,company,roomGetId(room));
        if(result != ESCAPE_TECHNION_SUCCESS){
            setDestroy(rooms_set);
            return result;
        }
    }
    return ESCAPE_TECHNION_SUCCESS;
}
 */
/*
static EscapeTechnionResult checkRoomReservations(EscapeTechnion system,
                                                  Company company, long id){
    assert(system && company);
    TechnionFaculty faculty;
    companyGetFaculty(company,&faculty);
    void* input_array[2] = {&faculty,&id};
    OrdersList orders_list = listFilter(system->orders,
                                        orderFilterByCompanyAndId,input_array);
    if(!orders_list){
        return ESCAPE_TECHNION_OUT_OF_MEMORY;
    }
    if(listGetSize(orders_list) > 0)
    {
        return ESCAPE_TECHNION_RESERVATION_EXISTS;
    }
    return ESCAPE_TECHNION_SUCCESS;
}
*/
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
        char *cur_company_mail;
        companyGetEmail((Company)current_company,&cur_company_mail);
        if(strcmp(cur_company_mail,email) == 0){
            *company = current_company;
            return ESCAPE_TECHNION_SUCCESS;
        }
    }
    return ESCAPE_TECHNION_COMPANY_EMAIL_DOES_NOT_EXIST;
}

static bool isIdAlreadyExistInFaculty(EscapeTechnion system, TechnionFaculty faculty, long id){
    assert(system);
    CompanyResult result;
    SET_FOREACH(CompanySet,current_company,system->companies){//for each company
        TechnionFaculty current_faculty;
        companyGetFaculty((Company)current_company,&current_faculty);//get faculty
        if(faculty == current_faculty){//if same faculty
            result = companySearchRoom((Company)current_company, id);//
            if(result == COMPANY_SUCCESS){//and same id
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
        case ROOM_NULL_ARGUMENT:
            return ESCAPE_TECHNION_NULL_PARAMETER;
        case ROOM_OUT_OF_MEMORY:
            return ESCAPE_TECHNION_OUT_OF_MEMORY;
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

static void removeClientOrders(EscapeTechnion system, Escaper escaper){
    assert(system && escaper);
    LIST_FOREACH(Order,order,system->orders){
        if(escaperCompare(orderGetEscaper(order),escaper) == 0){
            listRemoveCurrent(system->orders);
            listGetFirst(system->orders);
        }
    }
    return ;
}

static Escaper getEscaper(EscapeTechnion system, char *email){
    assert(system);
    SET_FOREACH(Escaper,escaper,system->escapers){
        char* escaper_email;
        escaperGetEmail(escaper,&escaper_email);
        if(strcmp(email,escaper_email) == 0){
            free(escaper_email);
            return escaper;
        }
    }
    return NULL;
}

static Room getRoom(EscapeTechnion system, TechnionFaculty faculty, long id){
    assert(system);
    assert(faculty >= 0 && faculty < FACULTY_NUM && id > 0);
    Room room;
    SET_FOREACH(Company,company,system->companies){
        TechnionFaculty company_faculty;
        companyGetFaculty(company,&company_faculty);
        if(company_faculty == faculty){
            companyFindRoom(company,id,&room);
            if(room != NULL){
                return room;
            }
        }
    }
    return NULL;
}

static bool isRoomAvailable(EscapeTechnion system,long day,
                                            long hour,long id,Room room){
    assert(system);
    long open_hour,close_hour;
    roomGetOpenAndCloseHour(room,&open_hour,&close_hour);
    if(hour<open_hour||hour>=close_hour){
        return false;
    }
    List filtered_list,temp_list;
    LIST_FOREACH(Order,order,system->orders){
        Room current_room = orderGetRoom(order);
        if (roomCompare(current_room,room)==0){
            if(orderGetDay(order) == day && orderGetHour(order) == hour){
                return false;
            }
        }
    }
    return true;
    /*
    void* room_array[2] = {&faculty,&id};
    void* time_array[2] = {&day,&hour};
    filtered_list = listFilter(system->orders,orderFilterByFacultiesAndId,
                               room_array);
    if(!filtered_list){
        return ESCAPE_TECHNION_OUT_OF_MEMORY;
    }
    temp_list = filtered_list;
    filtered_list = listFilter(temp_list,orderFilterByDayAndHour,time_array);
    listDestroy(temp_list);
    if(!filtered_list){
        return ESCAPE_TECHNION_OUT_OF_MEMORY;
    }
    if(listGetSize(filtered_list) > 0){
        return ESCAPE_TECHNION_ROOM_NOT_AVAILABLE;
    }
    listDestroy(filtered_list);
    return ESCAPE_TECHNION_SUCCESS;
     */
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
                            long *next_available_hour,long* next_avialable_day) {
    long wanted_hour, close_hour, day = escapeTechnionGetDay(system), open_hour, id;
    roomGetOpenAndCloseHour((Room) room, &open_hour, &close_hour);
    id = roomGetId(room);
    wanted_hour = open_hour;
    while (!isRoomAvailable(system, day, wanted_hour, id,
                            room)) {//find next avilability
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

static bool isFacultyNearer(TechnionFaculty checked_faculty,
                            TechnionFaculty recommended_faculty,
                            TechnionFaculty escaper_faculty){
    int checked_faculty_distance=abs(escaper_faculty-checked_faculty);
    int recommended_faculty_distance=abs(recommended_faculty-escaper_faculty);
    if(checked_faculty_distance==recommended_faculty_distance){
        return (checked_faculty<recommended_faculty) ? true : false;
    }
    if(checked_faculty_distance<recommended_faculty_distance)return true;
    return false;


}
