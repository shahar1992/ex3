//
// Created by Amir on 31/05/2017.
//

/**====================Include files==============================*/
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "Escaper.h"
/**================================================================*/

/**====================Main entity decleration==============================*/
struct Escaper_t{
    char* email;
    TechnionFaculty faculty;
    int skill_level;
};
/**====================end of entity decleration==============================*/



/**======================Macros===================================*/


/**====================End of Macros==============================*/




/**=====================Static functions declarations=========================*/
/** List of static functions and its usage
 * MailCheck          -checks if a given mail is valid;
 * Facultycheck       -checks if a given faculty is valid;
 */
static bool mailCheck(char *mail);
static bool facultyCheck(TechnionFaculty faculty);
static bool skillLevelCheck(int skill_level);
static bool inputCheck(char* email, TechnionFaculty faculty, int skill_level);

/**===================End of static function declarations.====================*/

/**===================Escaper ADT functions implementation====================*/


/**============EscaperCreate===========================*/
EscaperResult escaperCreate(char *email, TechnionFaculty faculty,
                            int skill_level, Escaper* escaper){
    if(!email || !escaper){
        return ESCAPER_NULL_ARGUMENT;
    }
    if(!inputCheck(email,faculty,skill_level)){
        return ESCAPER_INVALID_PARAMETER;
    }
    *escaper = malloc(sizeof(**escaper));
    if(!*escaper){
        return ESCAPER_OUT_OF_MEMORY;
    }
    (*escaper)->email = malloc(sizeof(char)*(strlen(email)+1));
    if(!(*escaper)->email) {
        escaperDestroy(*escaper);
        return ESCAPER_OUT_OF_MEMORY;
    }
    strcpy((*escaper)->email,email);
    (*escaper)->faculty = faculty;
    (*escaper)->skill_level = skill_level;
    return ESCAPER_SUCCESS;
}

/**=====EscaperDestroy==================================*/
void escaperDestroy(void* escaper){
    if(escaper != NULL) {
        if(((Escaper)escaper)->email != NULL) {
            free(((Escaper)escaper)->email);
        }
        free((Escaper)escaper);
    }
    return;
}

/**=====EscaperCopy==================================*/
void* escaperCopy(void* escaper){
    if(!escaper){
        return NULL;
    }
    Escaper escaper1 = escaper;
    Escaper  new_escaper;
    EscaperResult result = escaperCreate(escaper1->email, escaper1->faculty,
                                         escaper1->skill_level, &new_escaper);
    if(result != ESCAPER_SUCCESS){
        return NULL;
    }
    return new_escaper;
}

/**=====EscaperCmp==================================*/
int escaperCompare(void* escaper1, void* escaper2){
    assert(escaper1!=NULL && escaper2!=NULL);
    Escaper escaper1_ptr = escaper1, escaper2_ptr = escaper2;
    return (strcmp(((Escaper)escaper1)->email,((Escaper)escaper2)->email));
}

/**=====EscaperGetEmail==================================*/
EscaperResult escaperGetEmail(Escaper escaper,char** mail){
    if(!escaper || !mail||escaper->email==NULL){
        return ESCAPER_NULL_ARGUMENT;
    }
    *mail=malloc(sizeof(char)*(strlen(escaper->email) +1));
    if(!*mail){
        return ESCAPER_OUT_OF_MEMORY;
    }
    strcpy(*mail,escaper->email);
    return ESCAPER_SUCCESS;
}

/**=====EscaperGetFaculty=========================*/
EscaperResult escaperGetFaculty(Escaper escaper, TechnionFaculty* faculty){
    if(!escaper || !faculty){
        return ESCAPER_NULL_ARGUMENT;
    }
    *faculty = escaper->faculty;
    return ESCAPER_SUCCESS;
}

/**---------------------Escaper Get Skill Level-------------------------------*/
int escaperGetSkillLevel(Escaper escaper){
    if(!escaper){
        return 0;
    }
    return escaper->skill_level;
}

/** ===============Static functions implementaion==========================*/
static bool mailCheck(char *mail){
    if(mail==NULL){
        return false;
    }
    char* ptr=mail;
    int counter =0;
    while(*ptr!=0){
        if(*ptr=='@') counter++;
        ptr++;
    }
    return (counter == 1);
}

static bool facultyCheck(TechnionFaculty faculty){
    return (((int)faculty >= 0) &&((int)faculty < FACULTY_NUM));
}

/*static EscaperResult GetTime(char *time_format,long* hour,long* days_left){
    assert(time_format&&hour&&days_left);
    char *ptr;
    long ret;
    ret = strtol(time_format, &ptr, 10);
    *days_left=ret;
    ret=strtol(ptr+1,&ptr,10);
    *hour=ret;
    return  Escaper_SUCCESS;
}
*/
static bool skillLevelCheck(int skill_level){
    return ( (skill_level>=MIN_SKILL_LEVEL) && (skill_level<=MAX_SKILL_LEVEL) );
}

static bool inputCheck(char* email, TechnionFaculty faculty, int skill_level){
    return (mailCheck(email) && facultyCheck(faculty)
            && skillLevelCheck(skill_level));
}