//
// Created by Amir on 31/05/2017.
//
#include <stdlib.h>
#include <assert.h>
#include <string.h>


#include "Escaper.h"


/** List of static functions and its usage
 *
 * MailCheck          -checks if a given mail is valid;
 * Facultycheck       -checks if a given faculty is valid;
 *
 *
 *
 */

#define MEMORY_CHECK_NULL(ptr) \
    if ((ptr) == NULL) { \
        return Escaper_OUT_OF_MEMORY; \
    }

static bool MailCheck(char *mail){

}
static bool FacultyCheck(TechnionFaculty faculty){

}

struct Escaper_t{
    char * Email;
    TechnionFaculty faculty;
    int skill_level;
};


Escaper EscaperCreate(char* email, TechnionFaculty faculty, int skill_level){
    if( !MailCheck(email) || !FacultyCheck(faculty) ){
        return NULL;
    }
    Escaper escaper=malloc(sizeof(*escaper));
    MEMORY_CHECK_NULL(escaper);
    escaper->Email= malloc(sizeof(char)*(strlen(email)+1));
    MEMORY_CHECK_NULL(escaper->Email);
    strcpy(escaper->Email,email);
    escaper->faculty=faculty;
    escaper->skill_level=skill_level;
return escaper;
}



void EscaperDestroy(Escaper escaper){
    assert(escaper);
    free(escaper->Email);
    free(escaper);
    return;
}


Escaper EscaperCopy(Escaper escaper){
    assert(escaper);
    Escaper new_escaper = malloc(sizeof(*new_escaper));
    Escaper_OUT_OF_MEMORY(new_escaper);
    new_escaper->Email=malloc(sizeof(char)*(strlen(escaper->Email)+1) );
    Escaper_OUT_OF_MEMORY(new_escaper);
    strcpy(new_escaper->Email,escaper->Email);
    new_escaper->skill_level=escaper->skill_level;
    new_escaper->faculty=new_escaper->faculty;
    return new_escaper;
}

