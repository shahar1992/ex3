

#include "Order.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "list.h"
/**================================================================*/

/**====================Main entity decleration================================*/
struct Order_t{
    TechnionFaculty faculty;
    int day;
    int hour;
    int num_ppl;
    Room* room;
    Escaper* escaper;
};
/**====================end of entity decleration==============================*/

/**======================Macros===================================*/

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
static bool checkInput(TechnionFaculty faculty,int day, int hour,
                       int num_of_ppl,Room room,Escaper escaper);
static bool TimeCheck(long time);

/**===================End of static function declarations.====================*/

/**===================Order ADT functions implementation====================*/


/**============OrderCreate===========================*/

OrderResult orderCreate(TechnionFaculty faculty, int num_of_ppl, int hour,
                        int day, Room room, Escaper escaper, Order* order){
    if(!order){
        return ORDER_NULL_ARGUMENT;
    }
    if(!checkInput(faculty, time, num_of_ppl,room,escaper)){
        return ORDER_INVALID_PARAMETER;
    }
    *order = malloc(sizeof(*order));
    MEMORY_CHECK(*order, *order);
    (*order)->faculty=faculty;
    (*order)->day=day;
    (*order)->hour=hour;
    (*order)->num_ppl=num_ppl;
    (*order)->escaper = escaper;
    (*order)->room = room;
    return ORDER_SUCCESS;
}

/**============OrderDestroy===========================*/

void orderDestroy(ListElement  order){
    if(order != NULL) {
        free(((Order)order)->email);
        free(order);
    }
    return;
}

/**============OrderCopy===========================*/
Order OrderCopy(Order order){ //change this function and use create.
    assert(order!=NULL);
    Order new_order=malloc(sizeof(*new_order));
    if(new_order==NULL){
        orderDestroy(new_order);
        return NULL;
    }
    new_order->email=malloc(sizeof(char)*(strlen(order->email)+1));
    if(new_order->email==NULL){
        orderDestroy(new_order);
        return NULL;
    }
    new_order->days_left=order->days_left;
    new_order->start_hour=order->start_hour;
    new_order->ID=order->ID;
    new_order->faculty=order->faculty;
    new_order->num_ppl=order->num_ppl;
    return new_order;
}
/**============OrdeGetFaculty===========================*/

TechnionFaculty OrderGetFaculty(Order order){
    assert(order!=NULL);
    return  order->faculty;
}

/**============OrderCmpByTime===========================*/

int OrderCmpByTime(Order order1,Order order2){
    assert(order1!=NULL&&order2!=NULL);
    if(order1->start_hour!=order2->start_hour) return 1;
    if(order1->days_left!=order2->days_left) return 1;
    return 0;
}

/**============OrderCmpByTime===========================*/

int OrderCmpByTime_Faculty_ID(Order order1,Order order2){
    assert(order1!=NULL&&order2!=NULL);
    if(order1->start_hour!=order2->start_hour) return 1;
    if(order1->days_left!=order2->days_left) return 1;
    if(order1->faculty!=order2->faculty) return  1;
    if(order1->ID!=order2->ID) return 1;
    return 0;

}

OrderResult orderGetFaculty(Order order, TechnionFaculty* faculty){
    if(!order || !faculty){
        return Order_NULL_ARGUMENT;
    }
    *faculty = order->faculty;
    return Order_SUCCESS;
}

/**============OrderPrint===========================*/
void OrderPrint(Order order){
    assert(order!=NULL);
    printf("----------Printing order---------------\n");
    printf("Order's order email: %s\n",order->email);
    printf("Order's room id : %d\n",order->ID);
    printf("Order's room faculty : %d\n",order->faculty);
    printf("Days until order: %ld\n",order->days_left);
    printf("Order's wanted hour: %ld\n",order->start_hour);
    printf("Number of peoples in order: %d\n",order->num_ppl);
}

/** ===============Static functions implementaion==========================*/

static OrderResult GetTime(char *time_format,long* hour,long* days_left){
    assert(time_format&&hour&&days_left);
    char *ptr;
    long ret;
    ret = strtol(time_format, &ptr, 10);
    *days_left=ret;
    ret=strtol(ptr+1,&ptr,10);
    *hour=ret;
    return  Order_SUCCESS;
}

static bool MailCheck(char* mail){
    char* ptr=mail;
    int counter =0;
    while(*ptr!=0){
        if(*ptr=='@') counter++;
        ptr++;
    }
    return (counter==1)?true:false;//return true if only 1 @;

}
static bool TimeCheck(long time){
    if( time<0 || time> 23) return false;
    return true;
}

Company orderGetCompany(Order order){
    if(!order){
        return NULL;
    }
    return roomGetCompany(order->room);
}

Room orderGetRoom(Order order){
    if(!order){
        return NULL;
    }
    return order->room;
}

long orderCalculatePrice(Order order){
    if(!order){
        return -1;
    }
    return (order->num_ppl * roomGetPrice(order->room));
}