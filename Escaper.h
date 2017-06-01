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
    Escaper_SUCCESS,
    Escaper_OUT_OF_MEMORY,
    Escaper_NULL_ARGUMENT
} EscaperResult;

/** Allocates a new escaper */
Escaper EscaperCreate(char* email, TechnionFaculty faculty, int skill_level);

/** Frees an existing Escaper object */
void EscaperDestroy(Escaper escaper);

/** Allocates a new Escaper which is a copy of the argument */
Escaper EscaperCopy(Escaper escaper);

/** checks if 2 escapers are identical. return true if dentical or false if
 * not */
bool EscaperCmp(Escaper escaper1,Escaper escaper2);

/** Gets an escaper and a pointer to char[] and returns a copy of the mail in it
 * */
EscaperResult EscaperGetEmail(Escaper escaper,char* mail);

/** Connect order to Given escaper* */
SetResult EscaperAddOrder(Escaper escaper,Order order);

/** Remove order from Given escaper* */
SetResult EscaperRemoveOrder(Escaper escaper,Order order);

/** Remove all orders from Given escaper* */
void EscaperRemoveAllOrders(Escaper escaper);

/**Returns the faculty of the escaper*/
TechnionFaculty EscaperGetFaculty(Escaper escaper);

/**Check availabilty**/
SetResult EscaperCheckAvailabilty(Escaper,char* time);



#endif //EX3_ESCAPER_H
