

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
static bool checkInput(int day, int hour, int num_of_ppl);

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
    return (result != ORDER_SUCCESS) ? NULL : new_order;
}

/**============OrderCmpByTime===========================*/
int orderCompare(void* order1, void* order2){
    assert((order1 != NULL) && (order2 != NULL));
    if(((Order)order1)->day != ((Order)order2)->day){
        return ((Order)order1)->day - ((Order)order2)->day;
    }
    return  (((Order)order1)->hour - ((Order)order2)->hour);
}

OrderResult orderGetFaculty(Order order, TechnionFaculty* faculty){
    if(!order || !faculty){
        return ORDER_NULL_ARGUMENT;
    }
    *faculty = order->faculty;
    return ORDER_SUCCESS;
}

Room orderGetRoom(Order order){
    return (order == NULL) ? NULL : order->room;
}

int orderGetDay(Order order){
    return (order == NULL) ? 0 : order->day;
}

Escaper orderGetEscaper(Order order){
    return (order == NULL) ? NULL : order->escaper;
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
    return (order == NULL) ? 0 : order->num_ppl;
}

bool orderIsFacultyAndIdMatch(void* order, void* faculty_and_id_array){
    TechnionFaculty faculty;
    orderGetFaculty((Order)order,&faculty);
    return (faculty == (*(TechnionFaculty*)faculty_and_id_array) &&
            (roomGetId(((Order)order)->room) ==
                    *(long*)(faculty_and_id_array+1)));
}

bool orderFilterByDayAndHour(void* order, void* time_array){
    return (((Order)order)->day == (*(int*)time_array) &&
            ((Order)order)->hour == (*(int*)(time_array+1)));
}

/** ===============Static functions implementaion==========================*/

static bool checkInput(int day, int hour, int num_of_ppl){
    return ((day > 0) && (hour >= 0) && (hour <= 24) && (num_of_ppl > 0));
}
