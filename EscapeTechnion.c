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
static Company getCompany(EscapeTechnion system, char *email);
static bool isIdAlreadyExistInFaculty(EscapeTechnion system,
                                      TechnionFaculty faculty, long id);
static bool DoesRoomHaveOrders(EscapeTechnion sys,Room room,TechnionFaculty rooms_fac);
/*static EscapeTechnionResult checkRoomReservations(EscapeTechnion system,
                                                  Company company, long id);*/
static EscapeTechnionResult convertFromCompanyResult( CompanyResult result);
static EscapeTechnionResult convertFromEscaperResult(EscaperResult result);
static EscapeTechnionResult convertFromRoomResult(EscaperResult result);
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
static bool isOrderForDay(ListElement order, ListFilterKey key);

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
        companyDestroy(company);
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
    Company company = getCompany(system, email);//get Company by mail
    if(company==NULL){//if no company exists
        return ESCAPE_TECHNION_COMPANY_EMAIL_DOES_NOT_EXIST;
    }
    TechnionFaculty faculty;
    companyGetFaculty(company,&faculty);//get company's faculty
    if(isIdAlreadyExistInFaculty(system, faculty, id)==true){
        return ESCAPE_TECHNION_ID_ALREADY_EXIST;//break if id exists in faculty
    }
    CompanyResult com_res;
    com_res=companyAddRoom(company, id, price, num_ppl, open_hour, close_hour,
                           difficulty);
    EscapeTechnionResult result = convertFromCompanyResult(com_res);
    if(result != ESCAPE_TECHNION_SUCCESS){
        return result;
    }

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
                if(DoesRoomHaveOrders(system,room,faculty)==true){//if room has orders
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
    if(!checkAddOrderInput(day,hour,num_ppl,faculty,email,id)){
        return ESCAPE_TECHNION_INVALID_PARAMETER;
    }
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

/***/
Company escapeTechnionFindCompanyByRoomAndFaculty(EscapeTechnion system,
                                                  Room room,
                                                  TechnionFaculty* faculty){
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
OrdersList escapeTechnionGetTodayOrdersLists(EscapeTechnion system){
    assert(system);
    ListResult result = listSort(system->orders,orderCompare);
    if(result == LIST_OUT_OF_MEMORY){
        return NULL;
    }
    OrdersList list = listFilter(system->orders,isOrderForDay,&system->day);
    if(!list){
        return NULL;
    }
    LIST_FOREACH(Order,order,system->orders){
        if(orderGetTimeAndDay(order) > system->day){
            break;
        }
        listRemoveCurrent(system->orders);
        listGetFirst(system->orders);
    }
    return list;
}

/**===================Static functions implementation=========================*/
static bool isCompanyHasReservation(EscapeTechnion system, Company company){
    LIST_FOREACH(OrdersList,cur_order,system->orders){//for each order in sys
        TechnionFaculty orders_faculty;
        OrderResult ord_result=orderGetFaculty((Order)cur_order,&orders_faculty);
        if(ord_result!=ORDER_SUCCESS){
            return convertFromOrderResult(ord_result);
        }
        SET_FOREACH(CompanySet,cur_company,system->companies) {//for each company
            TechnionFaculty companys_faculty;
            CompanyResult com_result=companyGetFaculty((Company)cur_company,&companys_faculty);
            if (com_result!=COMPANY_SUCCESS) {
                 return convertFromCompanyResult(com_result);
             }
             if (orders_faculty == companys_faculty) {//if same faculty
                 Room orders_room=orderGetRoom((Order)cur_order);
                 long orders_room_id=roomGetId(orders_room);
                 com_result=companySearchRoom((Company)cur_company,orders_room_id);
                 if(com_result==COMPANY_SUCCESS){//found a match
                     return true;
                 }
             }
         }
    }
    return false;
}

static bool DoesRoomHaveOrders(EscapeTechnion sys,Room room,TechnionFaculty rooms_fac){
    LIST_FOREACH(OrdersList,cur_order,sys->orders){//for each order
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
static Company getCompany(EscapeTechnion system, char *email){
    assert(system && email);
    SET_FOREACH(Company,current_company,system->companies){
        char *cur_company_mail;
        companyGetEmail((Company)current_company,&cur_company_mail);
        if(strcmp(cur_company_mail,email) == 0){
            return current_company;
        }
    }
    return NULL;
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
static EscapeTechnionResult convertFromRoomResult(EscaperResult result){
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

static void removeClientOrders(EscapeTechnion system,
                                               Escaper escaper){
    assert(system && escaper);
    LIST_FOREACH(Order,order,system->orders){
        if(escaperCompare(orderGetEscaper(order),escaper) == 0){
            listRemoveCurrent(system->orders);
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
    List filtered_list,temp_list;
    LIST_FOREACH(Order,cur_order,system->orders){
        Room orders_room=orderGetRoom((Order)cur_order);
        if (room==orders_room){//same pointer
            long cur_orders_hour,cur_orders_day;
            assert(orderGetTimeAndDay(cur_order,&cur_orders_hour,&cur_orders_day)==ORDER_SUCCESS);
            if(cur_orders_day==day&&cur_orders_hour==hour){
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
             && (faculty<FACULTY_NUM)&&(faculty>=0)&& (id>0) );
}

static bool isClientAvailable(EscapeTechnion system,long day,
                            long hour,Escaper client) {
    assert(system);
    LIST_FOREACH(Order, cur_order, system->orders) {//for each order
        Escaper orders_client = orderGetEscaper((Order) cur_order);
        if (escaperCompare(orders_client,client)==0) {//same pointer
            long cur_orders_hour, cur_orders_day;
            assert(orderGetTimeAndDay(cur_order, &cur_orders_hour,
                                      &cur_orders_day) == ORDER_SUCCESS);
            if (cur_orders_day == day && cur_orders_hour == hour) {
                return false;
            }
        }
    }
    return true;
}


static bool isOrderedForDay(ListElement order, ListFilterKey key) {
    assert(order);
    int day = orderGetDay((Order)order);
    return  day == *(int*)key;
}