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
static bool mailCheck(char *email);
static bool facultyCheck(TechnionFaculty faculty);
static bool skillLevelCheck(int skill_level);
static bool inputCheck(char* email, TechnionFaculty faculty, int skill_level);

/**===================End of static function declarations.====================*/



/**===================Escaper ADT functions implementation====================*/


/**----------------------Escaper Create---------------------------------------*/
EscaperResult escaperCreate(char *email, TechnionFaculty faculty,
                            int skill_level, Escaper* escaper){
    assert(escaper);
    if(!email){
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

/**---------------------Escaper Destroy---------------------------------------*/
void escaperDestroy(void* escaper){
    if(escaper != NULL) {
        if (((Escaper) escaper)->email != NULL) {
            free(((Escaper) escaper)->email);
        }
        free((Escaper) escaper);
    }
    return;
}

/**---------------------Escaper Copy------------------------------------------*/
void* escaperCopy(void* void_escaper){
    assert(void_escaper);
    Escaper escaper = void_escaper;
    Escaper new_escaper;
#ifndef NDEBUG
    EscaperResult result =
#endif
     escaperCreate(escaper->email, escaper->faculty,escaper->skill_level,
                   &new_escaper);
    assert(result != ESCAPER_INVALID_PARAMETER);
    return (!new_escaper) ? NULL : new_escaper;
}

/**---------------------Escaper Compare---------------------------------------*/
int escaperCompare(void* escaper1, void* escaper2){
    assert(escaper1!=NULL && escaper2!=NULL);
    Escaper escaper1_ptr = escaper1, escaper2_ptr = escaper2;
    return (strcmp(((Escaper)escaper1)->email,((Escaper)escaper2)->email));
}

/**---------------------Escaper Get Email-------------------------------------*/
EscaperResult escaperGetEmail(Escaper escaper,char** email){
    assert(escaper);
    assert(escaper->email != NULL);
    if(!email){
        return ESCAPER_NULL_ARGUMENT;
    }
    *email=malloc(sizeof(char)*(strlen(escaper->email) +1));
    if(!*email){
        return ESCAPER_OUT_OF_MEMORY;
    }
    strcpy(*email,escaper->email);
    return ESCAPER_SUCCESS;
}

/**---------------------Escaper Get Faculty-----------------------------------*/
TechnionFaculty escaperGetFaculty(Escaper escaper){
    assert(escaper);
    return escaper->faculty;
}

/**---------------------Escaper Get Skill Level-------------------------------*/
int escaperGetSkillLevel(Escaper escaper){
    assert(escaper);
    return escaper->skill_level;
}

/**===================Static functions implementaion==========================*/
static bool mailCheck(char *email){
    assert(email);
    char* ptr = email;
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

static bool skillLevelCheck(int skill_level){
    return ((skill_level >= MIN_SKILL_LEVEL) &&
            (skill_level <= MAX_SKILL_LEVEL));
}

static bool inputCheck(char* email, TechnionFaculty faculty, int skill_level){
    return (mailCheck(email) && facultyCheck(faculty)
            && skillLevelCheck(skill_level));
}