//
// Created by Amir on 31/05/2017.
//

/**====================Include files==============================*/
#include "Escaper.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>
/**================================================================*/

/**====================Main entity decleration==============================*/
struct Escaper_t{
    char * Email;
    TechnionFaculty faculty;
    int skill_level;
    Order* Orders;
};
/**====================end of entity decleration==============================*/



/**======================Macros===================================*/

#define MEMORY_CHECK_NULL(ptr) \
    if ((ptr) == NULL) { \
        return Escaper_OUT_OF_MEMORY; \
    }
/**====================End of Macros==============================*/




/**=====================Static functions declarations=========================*/
/** List of static functions and its usage
 * MailCheck          -checks if a given mail is valid;
 * Facultycheck       -checks if a given faculty is valid;
 */
static bool MailCheck(char *mail);
static bool FacultyCheck(TechnionFaculty faculty);

/**===================End of static function declarations.====================*/

/**===================Escaper ADT functions implementation====================*/


/**============EscaperCreate===========================*/
Escaper EscaperCreate(char* email, TechnionFaculty faculty, int skill_level){
    if( !MailCheck(email) || !FacultyCheck(faculty) ){
        return NULL;
    }
    Escaper escaper=malloc(sizeof(*escaper));
    MEMORY_CHECK_NULL(escaper);
    escaper->Email= malloc(sizeof(char)*(strlen(email)+1));
    if(escaper->Email==NULL){

    }
    strcpy(escaper->Email,email);
    escaper->faculty=faculty;
    escaper->skill_level=skill_level;
return escaper;
}

/**=====EscaperDestroy==================================*/
void EscaperDestroy(Escaper escaper){
    assert(escaper);
    free(escaper->Email);
    free(escaper);
    return;
}

/**=====EscaperCopy==================================*/
Escaper EscaperCopy(Escaper escaper){
    assert(escaper);
    Escaper  new_escaper=malloc(sizeof(*new_escaper));
    Escaper_OUT_OF_MEMORY(new_escaper);
    new_escaper->Email=malloc(sizeof(char)*(strlen(escaper->Email)+1) );
    Escaper_OUT_OF_MEMORY(new_escaper);
    strcpy(new_escaper->Email,escaper->Email);
    new_escaper->skill_level=escaper->skill_level;
    new_escaper->faculty=new_escaper->faculty;
    return new_escaper;
}

/**=====EscaperCopy==================================*/
bool EscaperCmp(Escaper escaper1,Escaper escaper2){
    assert(escaper1&&escaper2);
    return (strcmp(escaper1->Email,escaper2->Email));
    /**strcmp returns 0 if both string are equal which differs
     * escapers*/
}
/**=====EscaperGetEmail==================================*/
EscaperResult EscaperGetEmail(Escaper escaper,char* mail){
    assert(escaper&&mail);
    mail=malloc(sizeof(char)*(strlen(escaper->Email)+1) );
    MEMORY_CHECK_NULL(mail);
    strcpy(escaper->Email,mail);
    return Escaper_SUCCESS;
}

/**
 *
 * Implement order ADT AND CONTINUE
 *
 *
 */
/**=====EscaperGetOrder=========================*/
EscaperResult EscaperGetOrder(Escaper escaper,Order order){

}

/** ===============Static functions implementaion==========================*/
static bool MailCheck(char* mail){
    char* ptr=mail;
    int counter =0;
    while(*ptr!=0){
        if(*ptr=='@') counter++;
    }
    return (counter==1)?TRUE:FALSE;//return true if only 1 @;

}
static bool FacultyCheck(TechnionFaculty faculty){

}