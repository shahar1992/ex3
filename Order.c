#include <stdlib.h>
#include <assert.h>
#include "Order.h"
/**================================================================*/

/**====================Main entity decleration================================*/
struct Order_t{
    TechnionFaculty faculty;
    int day;
    int hour;
    int num_ppl;
    Room room;
    Escaper escaper;
};

/**====================end of entity decleration==============================*/

/**======================Macros===================================*/



#define NULL_CHECK(condition) \
    if (!((condition))) { \
        return ORDER_NULL_ARGUMENT; \
    }

#define MEMORY_CHECK(condition,ptr_to_destroy) \
if(!(condition)){ \
    orderDestroy(ptr_to_destroy); \
    return ORDER_OUT_OF_MEMORY; \
}
/**====================End of Macros==============================*/

/**=====================Static functions declarations=========================*/
/** List of static functions and its usage
 * GetTime          -gets time in time format and returns both wanted hour and
 *                   number of days left in pointers;
 *  MailCheck        -checks if a mail is valid (only 1 @)
 *  TimeCheck         - checks if time is valid (betwenn 0 and 23 including)
 */
static bool checkInput(int day, int hour, int num_of_ppl,TechnionFaculty faculty);

/**===================End of static function declarations.====================*/

/**===================Order ADT functions implementation====================*/


/**============OrderCreate===========================*/

OrderResult orderCreate(int num_of_ppl, int hour, int day,
                        TechnionFaculty faculty, Room room, Escaper escaper,
                        Order* order){
    NULL_CHECK(order && room && escaper);
    if(!checkInput(day, hour, num_of_ppl,faculty)){
        return ORDER_INVALID_PARAMETER;
    }
    *order = malloc(sizeof(**order));
    MEMORY_CHECK(*order, *order);
    (*order)->day = day;
    (*order)->hour = hour;
    (*order)->num_ppl = num_of_ppl;
    (*order)->room = room;
    (*order)->escaper = escaper;
    (*order)->faculty = faculty;
    return ORDER_SUCCESS;

}

/**============OrderDestroy===========================*/

void orderDestroy(void* order){
    if(order != NULL) {
        free(order);
    }
    return;

}

/**============OrderCopy===========================*/
void* orderCopy(void* order){
    if(!order){
        return NULL;
    }
    Order new_order;
    OrderResult result = orderCreate(((Order)order)->num_ppl,
                                     ((Order)order)->hour,((Order)order)->day,
                                     ((Order)order)->faculty,
                                     ((Order)order)->room,
                                     ((Order)order)->escaper,&(new_order));
    return (result != ORDER_SUCCESS) ? NULL : new_order;
}

/**============OrderCmp===========================*/
int orderCompare(void* order1, void* order2){
    assert(((Order)order1 != NULL) && ((Order)order2 != NULL));
    return (((Order)order1)->day != ((Order)order2)->day) ?
           (((Order)order1)->day - ((Order)order2)->day) :
           ((Order)order1)->hour - ((Order)order2)->hour;
}

/**============OrderGetFaculty===========================================*/
OrderResult orderGetFaculty(Order order, TechnionFaculty* faculty){
    if(!order || !faculty){
        return ORDER_NULL_ARGUMENT;
    }
    *faculty = order->faculty;
    return ORDER_SUCCESS;
}

/**============OrderGetRoom===========================================*/
Room orderGetRoom(Order order){
    if(!order){
        return NULL;
    }
    return order->room;
}
/**============OrderGetEscaper===========================================*/
Escaper orderGetEscaper(Order order){
    if(!order){
        return NULL;
    }
    return order->escaper;
}
/**============OrderCalculateprice===========================================*/
long orderCalculatePrice(Order order){
    if(!order){
        return 0;
    }
    long total_price = order->num_ppl * roomGetPrice(order->room);
    TechnionFaculty escaper_faculty;
    escaperGetFaculty(order->escaper,&escaper_faculty);
    return (order->faculty == escaper_faculty) ?
           (total_price * DISCOUNT_FOT_FACULTY_MEMBERS) : total_price;
}

/**=====================OrderGetNumOFPeople===================================*/
int orderGetNumOfPeople(Order order){
    if(!order){
        return 0;
    }
    return order->num_ppl;
}

/**----------------------Order Get Day----------------------------------------*/
int orderGetDay(Order order){
    assert(order);
    return order->day;
}

/**---------------------Order Get Hour----------------------------------------*/
int orderGetHour(Order order){
    assert(order);
    return order->hour;
}

/**============OrderGetRoomId===========================================*/
OrderResult orderGetRoomId(Order order,long* id){
    if(!order||!id){
        return ORDER_NULL_ARGUMENT;
    }
    *id=roomGetId(orderGetRoom(order));
    return ORDER_SUCCESS;
}


/** ===============Static functions implementation==========================*/
/** ===============Static functions implementation============================*/

static bool checkInput(int day, int hour, int num_of_ppl,TechnionFaculty faculty){
    return ( (day >= 0) && (hour >= 0) && (hour < MAX_HOUR) && (num_of_ppl > 0)
    && (faculty<FACULTY_NUM)&&(faculty>=0) );
}
