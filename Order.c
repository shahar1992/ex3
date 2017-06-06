

#include <stdlib.h>
#include <assert.h>
#include <string.h>

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
#define DISCOUNT_FOT_FACULTY_MEMBERS 1/4


#define NULL_CHECK(condition) \
    if (!(condition)) { \
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
static OrderResult GetTime(char *time_format,long* hour,long* days_left);
static bool checkInput(int day, int hour, int num_of_ppl);
static bool TimeCheck(long time);

/**===================End of static function declarations.====================*/

/**===================Order ADT functions implementation====================*/


/**============OrderCreate===========================*/

OrderResult orderCreate(int num_of_ppl, int hour, int day,
                        TechnionFaculty faculty, Room room, Escaper escaper,
                        Order* order){
    NULL_CHECK(order && room && escaper);
    if(!checkInput(hour, day, num_of_ppl)){
        return ORDER_INVALID_PARAMETER;
    }
    *order = malloc(sizeof(*order));
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
    if(result != ORDER_SUCCESS){
        return NULL;
    }
    return new_order;
}

/**============OrderCmpByTime===========================*/
int orderCompare(Order order1, Order order2){
    assert((order1 != NULL) && (order2 != NULL));
    if(order1->day != order2->day){
        return order1->day - order2->day;
    }
    return  (order1->hour - order2->hour);
}

OrderResult orderGetFaculty(Order order, TechnionFaculty* faculty){
    if(!order || !faculty){
        return ORDER_NULL_ARGUMENT;
    }
    *faculty = order->faculty;
    return ORDER_SUCCESS;
}

Room orderGetRoom(Order order){
    if(!order){
        return NULL;
    }
    return order->room;
}

Escaper orderGetEscaper(Order order){
    if(!order){
        return NULL;
    }
    return order->escaper;
}

long orderCalculatePrice(Order order){
    if(!order){
        return 0;
    }
    long total_price = order->num_ppl * roomGetPrice(order->room);
    TechnionFaculty faculty;
    escaperGetFaculty(order->escaper,&faculty);
    return (order->faculty == faculty) ?
           (total_price * DISCOUNT_FOT_FACULTY_MEMBERS) : total_price;
}

int orderGetNumOfPeople(Order order){
    if(!order){
        return 0;
    }
    return order->num_ppl;
}

/** ===============Static functions implementaion==========================*/

static bool checkInput(int day, int hour, int num_of_ppl){
    return ((day > 0) && (hour >= 0) && (hour <= 24) && (num_of_ppl > 0));
}
