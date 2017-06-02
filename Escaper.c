//
// Created by Amir on 31/05/2017.
//

/**====================Include files==============================*/
#include "Escaper.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "set.h"
#include "list.h"
/**================================================================*/

/**====================Main entity decleration==============================*/
struct Escaper_t{
    char * Email;
    TechnionFaculty faculty;
    int skill_level;
    Set Orders;

};
/**====================end of entity decleration==============================*/



/**======================Macros===================================*/

/**====================End of Macros==============================*/




/**=====================Static functions declarations=========================*/
/** List of static functions and its usage
 * MailCheck          -checks if a given mail is valid;
 * Facultycheck       -checks if a given faculty is valid;
 */
static bool MailCheck(char *mail);
static bool FacultyCheck(TechnionFaculty faculty);
static EscaperResult GetTime(char *time_format,long* hour,long* days_left);

/**===================End of static function declarations.====================*/

/**===================Escaper ADT functions implementation====================*/


/**============EscaperCreate===========================*/
Escaper EscaperCreate(char* email, TechnionFaculty faculty, int skill_level){
    if( !MailCheck(email) || !FacultyCheck(faculty) ){
        return NULL;
    }
    Escaper escaper=malloc(sizeof(*escaper));
    if(escaper==NULL){
        return  NULL;
    }
    escaper->Email= malloc(sizeof(char)*(strlen(email)+1));
    if(escaper->Email == NULL) {

    }
    strcpy(escaper->Email,email);
    escaper->Orders=setCreate(OrderCopy,OrderDestroy,OrderCmpByTime);
    escaper->faculty=faculty;
    escaper->skill_level=skill_level;
return escaper;
}

/**=====EscaperDestroy==================================*/
void EscaperDestroy(Escaper escaper){
    assert(escaper);
    free(escaper->Email);
    setDestroy(escaper->Orders);
    free(escaper);
    return;
}

/**=====EscaperCopy==================================*/
Escaper EscaperCopy(Escaper escaper){
    assert(escaper);
    Escaper  new_escaper=malloc(sizeof(*new_escaper));
    if(new_escaper==NULL){
        return  NULL;
    }
    new_escaper->Email=malloc(sizeof(char)*(strlen(escaper->Email)+1) );
    if(new_escaper->Email==NULL){
        EscaperDestroy(new_escaper);
        return  NULL;
    }
    strcpy(new_escaper->Email,escaper->Email);
    new_escaper->Orders=setCopy(escaper->Orders);
    if(new_escaper->Orders==NULL){
        EscaperDestroy(new_escaper);
        return NULL;
    }
    new_escaper->skill_level=escaper->skill_level;
    new_escaper->faculty=new_escaper->faculty;
    return new_escaper;
}

/**=====EscaperCmp==================================*/
bool EscaperCmp(Escaper escaper1,Escaper escaper2){
    assert(escaper1&&escaper2);
    return (strcmp(escaper1->Email,escaper2->Email));
    /**strcmp returns 0 if both string are equal which differs
     * escapers*/
}

/**=====EscaperGetEmail==================================*/
char* EscaperGetEmail(Escaper escaper){
    assert(escaper);
    return escaper->Email;
}

/**
 *
 * Implement order ADT AND CONTINUE
 *
 *
 */
/**=====EscaperAddOrder=========================*/
SetResult EscaperAddOrder(Escaper escaper,Order order){
    assert(escaper!=NULL&&order!=NULL);
    return  setAdd(escaper->Orders,order);
}

/**=====EscaperRemoveOrder=========================*/
SetResult EscaperRemoveOrder(Escaper escaper,Order order){
    assert(escaper!=NULL&&order!=NULL);
    return setRemove(escaper->Orders,order);
}

/**=====EscaperRemoveAllOrders=========================*/
void EscaperRemoveAllOrders(Escaper escaper){
    assert(escaper!=NULL);
    setDestroy(escaper->Orders);
    return;
}

/**=====EscaperGetFaculty=========================*/
TechnionFaculty EscaperGetFaculty(Escaper escaper){
    assert(escaper!=NULL);
    return escaper->faculty;
}

/**=====EscaperCheckAvailabilty=========================*/
SetResult EscaperCheckAvailabilty(Escaper escaper,char* time){
    long hour,days_left;
    GetTime(time,&hour,&days_left);
    Order dummy_order=OrderCreate('Dummy order',UNKNOWN,0,time,0);
    //create a dummy order to check availabilty by time
    SetResult result=setIsIn(escaper->Orders,dummy_order);// returns true if
    //element is in the set
    OrderDestroy(dummy_order);
    return  result;

}




/** ===============Static functions implementaion==========================*/
static bool MailCheck(char* mail){
    char* ptr=mail;
    int counter =0;
    while(*ptr!=0){
        if(*ptr=='@') counter++;
    }
    return (counter==1)?true:false;//return true if only 1 @;

}
static bool FacultyCheck(TechnionFaculty faculty){

}

static EscaperResult GetTime(char *time_format,long* hour,long* days_left){
    assert(time_format&&hour&&days_left);
    char *ptr;
    long ret;
    ret = strtol(time_format, &ptr, 10);
    *days_left=ret;
    ret=strtol(ptr+1,&ptr,10);
    *hour=ret;
    return  Escaper_SUCCESS;
}