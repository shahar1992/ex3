#ifndef EX3_ESCAPER_H
#define EX3_ESCAPER_H

#include <stdbool.h>
#include "mtm_ex3.h"
#include "Order.h"
#include "set.h"
/**
*  Escaper ADT
*
* Implements an Escaper struct
* An escaper is visitor currently playing or has a future order to play in the
 * escape technion system.
*
* The following functions are available:
*
*   EscaperCreate               - Creates a new Escaper
*   EscaperDestroy              - Deletes an existing Escaper and frees all resources
*   EscaperCopy                 - Copies an existing Escaper
*   EscaperCmp                  -checks if 2 escapers are identical(0=identical)
*   EscaperGetEmail             -Returns a copy of the escaper mail.
*   EscaperGetFaculty           -Return enum faculty of the escaper
*   EscaperAddOrder             -adds an order to the escaper.
*   EscaperRemoveOrder          -Remove an order of the escaper
*   EscaperRemoveAllOrders      -Remove all orders of the escaper
*   EscaperCheckAvailabilty     -Checks if the escaper is available in given time
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
void escaperDestroy(Escaper escaper);

/** Allocates a new Escaper which is a copy of the argument */
Escaper escaperCopy(Escaper escaper);

/** checks if 2 escapers are identical. return true if do and false otherwise.
 */
bool EscaperCompare(Escaper escaper1, Escaper escaper2);

/** Gets an escaper and returns a pointer to the mail address.
 */
char* escaperGetEmail(Escaper escaper);

/**Returns the faculty of the escaper*/
TechnionFaculty escaperGetFaculty(Escaper escaper);



#endif //EX3_ESCAPER_H
