#ifndef EX3_ESCAPER_H
#define EX3_ESCAPER_H

#include <stdbool.h>
#include "mtm_ex3.h"

#define FACULTY_NUM ((int)UNKNOWN)
#define MIN_SKILL_LEVEL 1
#define MAX_SKILL_LEVEL 10

/**
*  Escaper ADT
*
* Implements an Escaper struct
* An escaper is visitor currently playing or has a future order to play in the
 * escape technion system.
*
* The following functions are available:
*
*   EscaperCreate               - Creates a new Escaper-TESTED
*   EscaperDestroy              - Deletes an existing Escaper and frees all resources-TESTED
*   EscaperCopy                 - Copies an existing Escaper-TESTED
*   EscaperCmp                  -checks if 2 escapers are identical-TESTED
*   EscaperGetEmail             -Returns a copy of the escaper mail.-TESTED
*   EscaperGetFaculty           -Return enum faculty of the escaper-TESTED
*
*/

/** Type for defining the list */
typedef struct Escaper_t* Escaper;

/** Type used for returning error codes from list functions */
/** Type used for returning error codes from room functions */
typedef enum EscaperResult_t {
    ESCAPER_SUCCESS,
    ESCAPER_INVALID_PARAMETER,
    ESCAPER_OUT_OF_MEMORY,
    ESCAPER_NULL_ARGUMENT
} EscaperResult;

/** Allocates a new escaper */
EscaperResult escaperCreate(char *email, TechnionFaculty faculty,
                            int skill_level, Escaper* escaper);

/** Frees an existing Escaper object */
void escaperDestroy(void* escaper);

/** Allocates a new Escaper which is a copy of the argument */
void* escaperCopy(void* escaper);

/** checks if 2 escapers are identical. return true if do and false otherwise.
 */
int escaperCompare(void* escaper1, void* escaper2);

/** Gets an escaper and returns a pointer to the mail address.
 */
EscaperResult escaperGetEmail(Escaper escaper,char** email);

/**Returns the faculty of the escaper*/
TechnionFaculty escaperGetFaculty(Escaper escaper);

/**Gets an escaper skill level*/
int escaperGetSkillLevel(Escaper escaper);


#endif //EX3_ESCAPER_H
