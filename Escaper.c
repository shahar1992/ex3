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
    char* Email;
    TechnionFaculty faculty;
    int skill_level;

};
/**====================end of entity decleration==============================*/



/**======================Macros===================================*/
#define FACULTY_NUM 19

/**====================End of Macros==============================*/




/**=====================Static functions declarations=========================*/
/** List of static functions and its usage
 * MailCheck          -checks if a given mail is valid;
 * Facultycheck       -checks if a given faculty is valid;
 */
static bool mailCheck(char *mail);
static bool FacultyCheck(TechnionFaculty faculty);
static EscaperResult getTime(char *time_format, long *hour, long *days_left);

/**===================End of static function declarations.====================*/

/**===================Escaper ADT functions implementation====================*/


/**============EscaperCreate===========================*/
EscaperResult escaperCreate(char *email, TechnionFaculty faculty,
                            int skill_level, Escaper* escaper){
    if( !MailCheck(email) || !FacultyCheck(faculty) ){
        return ESCAPER_INVALID_PARAMETER;
    }
    *escaper = malloc(sizeof(**escaper));
    if(!*escaper){
        return ESCAPER_OUT_OF_MEMORY;
    }
    (*escaper)->Email = malloc(sizeof(char)*(strlen(email)+1));
    if(!(*escaper)->Email) {
        EscaperDestroy(escaper);
        return ESCAPER_OUT_OF_MEMORY;
    }
    strcpy(escaper->Email,email);
    escaper->faculty = faculty;
    escaper->skill_level = skill_level;
    return ESCAPER_SUCCESS;
}

/**=====EscaperDestroy==================================*/
void escaperDestroy(Escaper escaper){
    if(escaper != NULL) {
        if(escaper->Email != NULL) {
            free(escaper->Email);
        }
        free(escaper);
    }
    return;
}

/**=====EscaperCopy==================================*/
Escaper escaperCopy(Escaper escaper){
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
bool escaperCmp(Escaper escaper1, Escaper escaper2){
    assert(escaper1&&escaper2);
    return (strcmp(escaper1->Email,escaper2->Email));
    /**strcmp returns 0 if both string are equal which differs
     * escapers*/
}

/**=====EscaperGetEmail==================================*/
char* escaperGetEmail(Escaper escaper){
    assert(escaper);
    return escaper->Email;
}

/**=====EscaperGetFaculty=========================*/
TechnionFaculty escaperGetFaculty(Escaper escaper){
    assert(escaper!=NULL);
    return escaper->faculty;
}

/** ===============Static functions implementaion==========================*/
static bool mailCheck(char *mail){
    char* ptr=mail;
    int counter =0;
    while(*ptr!=0){
        if(*ptr=='@') counter++;
    }
    return (counter == 1);
}

static bool FacultyCheck(TechnionFaculty faculty){
    return (((int)faculty >= 0) &&((int)faculty < FACULTY_NUM));
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