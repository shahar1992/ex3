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



/**===================End of static function declarations.====================*/




/**===================Order ADT functions implementation====================*/


/**-----------------------Order Create----------------------------------------*/

OrderResult orderCreate(int num_of_ppl, int hour, int day,
                        TechnionFaculty faculty, Room room, Escaper escaper,
                        Order* order){
    assert(order);
    if(!ordercheckInput(day, hour, num_of_ppl,faculty)){
        return ORDER_INVALID_PARAMETER;
    }
    if(!escaper){
        return ORDER_ESCAPER_IS_NULL;
    }
    if(!room){
        return ORDER_ROOM_IS_NULL;
    }
    *order = malloc(sizeof(**order));
    if(!*order){
        return ORDER_OUT_OF_MEMORY;
    }
    (*order)->room = roomCopy(room);
    (*order)->escaper = escaperCopy(escaper);
    if(!(*order)->room || !(*order)->escaper){
        orderDestroy(*order);
        return ORDER_OUT_OF_MEMORY;
    }
    (*order)->day = day;
    (*order)->hour = hour;
    (*order)->num_ppl = num_of_ppl;
    (*order)->faculty = faculty;
    return ORDER_SUCCESS;

}

/**============OrderDestroy===========================*/

void orderDestroy(void* order){
    if(order != NULL) {
        escaperDestroy(((Order)order)->escaper);
        roomDestroy(((Order)order)->room);
        free(order);
    }
    return;
}

/**============OrderCopy===========================*/
void* orderCopy(void* order){
    assert(order);
    Order new_order;
#ifndef NDEBUG
    OrderResult result =
#endif
    orderCreate(((Order)order)->num_ppl, ((Order)order)->hour,
                ((Order)order)->day, ((Order)order)->faculty,
                ((Order)order)->room, ((Order)order)->escaper, &(new_order));
    assert(result != ROOM_INVALID_PARAMETER);
    return (!new_order) ? NULL : new_order;
}

/**============OrderCmp===========================*/
int orderCompare(void* order1, void* order2){
    assert(((Order)order1 != NULL) && ((Order)order2 != NULL));
    return (((Order)order1)->day != ((Order)order2)->day) ?
           (((Order)order1)->day - ((Order)order2)->day) :
           ((Order)order1)->hour - ((Order)order2)->hour;
}

/**============OrderGetFaculty===========================================*/
TechnionFaculty orderGetFaculty(Order order){
    assert(order);
    return order->faculty;
}

/**============OrderGetRoom===========================================*/
Room orderGetRoom(Order order){
    assert(order);
    assert(order->room);
    return order->room;
}
/**============OrderGetEscaper===========================================*/
Escaper orderGetEscaper(Order order){
    assert(order);
    assert(order->escaper);
    return order->escaper;
}
/**============OrderCalculateprice===========================================*/
long orderCalculatePrice(Order order){
    assert(order);
    long total_price = order->num_ppl * roomGetPrice(order->room);
    TechnionFaculty escaper_faculty = escaperGetFaculty(order->escaper);
    return (order->faculty == escaper_faculty) ?
           (total_price * DISCOUNT_FOT_FACULTY_MEMBERS) : total_price;
}

/**=====================OrderGetNumOFPeople===================================*/
int orderGetNumOfPeople(Order order){
    assert(order);
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
long orderGetRoomId(Order order){
    assert(order);
    return (order->room == NULL) ? 0 : roomGetId(order->room);
}

/**--------------------------Order Get Difficulty-----------------------------*/
long orderGetDifficulty(Order order){
    assert(order);
    return roomGetDiffuclty(order->room);
}

bool orderIsSameFaculty(void* order, void* compared_faculty){
    assert(order);
    assert(compared_faculty);
    return (((Order)order)->faculty == *(TechnionFaculty*)compared_faculty);
}

bool orderIsSameId(void* order, void* id_to_compare){
    assert(order);
    assert(id_to_compare);
    return (roomGetId(((Order)order)->room) == *(long*)id_to_compare);
}

/** ===============Static functions implementation==========================*/

 bool ordercheckInput(int day, int hour, int num_of_ppl,
                       TechnionFaculty faculty){
    return ( (day >= 0) && (hour >= 0) && (hour < MAX_HOUR) && (num_of_ppl > 0)
    && (faculty < FACULTY_NUM) && (faculty >= 0));
}

int orderCompareByFaculty(void* order1, void* order2){
    assert(((Order)order1 != NULL) && ((Order)order2 != NULL));
    return (((Order)order2)->faculty < ((Order)order1)->faculty);
}

int orderCompareByRoomId(void* order1, void* order2){
    assert(((Order)order1 != NULL) && ((Order)order2 != NULL));
    return orderGetRoomId(order2)-orderGetRoomId(order1);
}
int orderCompareByHour(void* order1, void* order2){
    assert(((Order)order1 != NULL) && ((Order)order2 != NULL));
    return orderGetHour(order2)-orderGetHour(order1);
}
int orderCompareByCritiria(void* order1, void* order2){
    assert(((Order)order1 != NULL) && ((Order)order2 != NULL));
    int result=orderCompareByHour(order2,order1);
    if(result==0){//same time
        result=orderCompareByFaculty(order1,order2);
        if(result==0){//same faculty
            result=orderCompareByRoomId(order1,order2);
            return result*-1;
        }
        return result;
    }
    else return result;
}

bool orderNotBelongToClient(void* order , void* escaper){
    Escaper order_escaper = ((Order)order)->escaper;
    return (escaperCompare(order_escaper,escaper) != 0);
}