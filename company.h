#ifndef EX3_COMPANY_H
#define EX3_COMPANY_H

#include "stdbool.h"
#include "set.h"
#include "Room.h"
#include "mtm_ex3.h"

/**
 *  Abstract company.
 *
 * Implements a Company data type.
 *
 * The following functions are available:
 *   companyCreate	   - Creates a new empty set
 *   companyFree	   - Deletes an existing company and frees all resources.
 *   companyCopy	   - Copies an existing company.
 *   companyCompare    - Compares two companies with email as compare key.
 *   companyAddRoom    - Adds a new room to the company.
 *   companyRemoveRoom - Removes a room from the company.
 *   companyGetFaculty - Gets the Faculty who owns the company.
 *   companyGetEmail   - Gets the email of the company.
 *   companyFindRoom   - Find a room in the company rooms set by its id.
*/

/** Type for defining the Company */
typedef struct company_t *Company;

typedef Set RoomSet;

/** Type used for returning error codes from company functions */
typedef enum CompanyResult_t {
    COMPANY_SUCCESS,
    COMPANY_OUT_OF_MEMORY,
    COMPANY_NULL_ARGUMENT,
    COMPANY_ROOM_ALREADY_EXISTS,
    COMPANY_ROOM_DOES_NOT_EXIST
} CompanyResult;

/**
 * This function creates new room and reset all the room's parameters.
 * @param email The new company email address.
 * @param faculty The faculty that the new company belongs to.
 * @param company The new company's pointer.
 * @return
 *     return the new company if create succeed.
 *     NULL if memory allocate failed.
 */
Company companyCreate(char *email, TechnionFaculty faculty);

/** This function deallocating a company and all of it resources*/
void companyFree(SetElement company);

/** This function copying a company */
SetElement companyCopy(SetElement company);

/**
* This function used by the Company to identify equal companies by email.
* This function should return:
* 		A positive integer if 'company1' email is lexicographicaly greater.
* 		0 if they're equal;
*		A negative integer if 'company2' email is lexicographicaly greater.
*/
int companyCompare(SetElement company1, SetElement company2);

/**
 * This function add a room to the company rooms list.
 * @param company The relevant company.
 * @param room The room to add.
 * @return
 *      COMPANY_NULL_ARGUMENT if 'company' or 'room' are null.
 *      COMPANY_OUT_OF_MEMORY if allocate failed.
 *      COMPANY_SUCCESS otherwise.
 */
CompanyResult companyAddRoom(Company company, Room room);

/**
 * This function remove a room from the company rooms list.
 * @param company The relevant company.
 * @param room The room to remove.
 * @return
 *      COMPANY_NULL_ARGUMENT if 'company' or 'room' are null.
 *      COMPANY_SUCCESS otherwise.
 */
CompanyResult companyRemoveRoom(Company company, Room room);

/**
 *  This function return in parameter the faculty that ownes the company.
 * @param company The relevant company.
 * @param faculty A pointer to a integer to save the id value.
 * @return
 *      COMPANY_NULL_ARGUMENT if 'company' or 'faculty' are NULL.
 *      COMPANY_SUCCESS otherwise.
 */

CompanyResult companyGetFaculty(Company company, TechnionFaculty* faculty);
/**
 *  This function return in parameter the faculty that ownes the company.
 * @param company The relevant company.
 * @param email A pointer to a string to save the email to.
 * @return
 *      COMPANY_NULL_ARGUMENT if 'company' or 'email' are NULL.
 *      COMPANY_SUCCESS otherwise.
 */
CompanyResult companyGetEmail(Company company, char** email);



/**
 * This function finds a room in company rooms list by id.
 * @param company The relevant company.
 * @param id The id of wanted room.
 * @param room Paramete to save the wanted room if found.
 * @return
 *      COMPANY_NULL_PARAMETER if 'company' is null.
 *      COMPANY_ROOM_NOT_EXIST if no room founds.
 *      COMPNY_SUCCESS otherwise.
 */
CompanyResult companyFindRoom(Company company, int id, Room* room);


/**
 * This function checks if the input parameters are legal company parameters.
 * @param email Company email address to check.
 * @param faculty Faculty to check.
 * @return
 *      False if one or more of the parameters is not legal.
 *      True otherwise.
 */
//bool checkIfCompanyParametersLegal(char* email, int faculty);


#endif //EX3_COMPANY_H