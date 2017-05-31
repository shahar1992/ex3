//
// Created by Amir on 31/05/2017.
//

/**====================Include files==============================*/
#include "Order.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>
/**================================================================*/

/**====================Main entity decleration================================*/
struct Order_t{
    char * Email;
    TechnionFaculty faculty;
    int ID;
    long start_hour;
    long days_left;
    int num_ppl;
};
/**====================end of entity decleration==============================*/

/**======================Macros===================================*/

#define MEMORY_CHECK_NULL(ptr) \
    if ((ptr) == NULL) { \
        return Order_OUT_OF_MEMORY; \
    }
/**====================End of Macros==============================*/

/**=====================Static functions declarations=========================*/
/** List of static functions and its usage
 * GetTime          -gets time in time format and returns both wanted hour and
 *                   number of days left in pointers;
 */
static OrderResult GetTime(char *time_format,long* hour,long* days_left);

/**===================End of static function declarations.====================*/

/**===================Order ADT functions implementation====================*/


/**============OrderCreate===========================*/

Order OrderCreate(char* email, TechnionFaculty faculty, int id,char* time,int num_of_ppl){
    assert(mail!=NULL && time!=NULL);
    Order order=malloc(sizeof(*order));
    if(order==NULL) {
        return NULL;
    }
    order->Email=malloc(sizeof(char)*(strlen(email)+1));
    if(order->Email==NULL) {
        OrderDestroy(order);
        return NULL;
    }
    strcpy(order->Email,email);
    order->faculty=faculty;
    order->ID=id;
    GetTime(time,&order->start_hour,&order->days_left);
    return order;
}

/**============OrderDestroy===========================*/

OrderResult OrderDestroy(Order order){
    assert(order!=NULL);
    free(order->Email);
    return Order_SUCCESS;

}
/**============OrderCopy===========================*/
Order OrderCopy(Order order){
    assert(order!=NULL);
    Order new_order=malloc(sizeof(*new_order));
    if(new_order==NULL){
        OrderDestroy(new_order);
        return NULL;
    }
    new_order->Email=malloc(sizeof(char)*(strlen(order->Email)+1));
    if(new_order->Email==NULL){
        OrderDestroy(new_order);
        return NULL;
    }
    assert(new_order->Email!=NULL);
    strcpy(new_order->Email,order->Email);
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
/**============OrderPrint===========================*/
void OrderPrint(Order order){
    assert(order!=NULL);
    printf("----------Printing order---------------/n");
    printf("Order's order email: %s/n",order->Email);
    printf("Order's room id : %d/n",order->ID);
    printf("Order's room faculty : %d/n",order->faculty);
    printf("Days until order: %ld/n",order->days_left);
    printf("Order's wanted hour: %ld/n",order->start_hour);
    printf("Number of peoples in order: %d/n",order->num_ppl);
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