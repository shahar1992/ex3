//
// Created by Amir on 31/05/2017.
//

/**====================Include files==============================*/
#include "Order.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
/**================================================================*/

/**====================Main entity decleration==============================*/
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
static OrderResult GetTime(char *time_format,int* hour,int* days_left);


/**===================End of static function declarations.====================*/

/**===================Order ADT functions implementation====================*/


/**============OrderCreate===========================*/

Order OrderCreate(char* email, TechnionFaculty faculty, int id,char* time,int num_of_ppl){
    assert(mail!=NULL && time!=NULL);
    Order order=malloc(sizeof(*order));
    MEMORY_CHECK_NULL(order);
    order->Email=malloc(sizeof(char)*(strlen(email)+1));
    if(order->Email==NULL){

    }

}


/** ===============Static functions implementaion==========================*/

static OrderResult GetTime(char *time_format,long* hour,long* days_left){
    assert(time_format&&hour&&days_left);
    char *ptr;
    long ret;
    ret = strtol(time_format, &ptr, 10);
    //printf("number of days is %ld\n", ret);
    *days_left=ret;
    ret=strtol(ptr+1,&ptr,10);
    *hour=ret;
    //printf("hour is %ld\n", ret);
}