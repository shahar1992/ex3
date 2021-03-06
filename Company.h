#ifndef EX3_COMPANY_H
#define EX3_COMPANY_H

#include "stdbool.h"
#include "set.h"
#include "Room.h"
#include "mtm_ex3.h"

#define FACULTY_NUM ((int)UNKNOWN)

/**
 *  Abstract company.
 *
 * Implements a Company data type.
 *
 * The following functions are available:
 *   companyCreate	   - Creates a new empty set--TESTED
 *   companyDestroy	   - Deletes an existing company and frees all resources.-TESTED
 *   companyCopy	   - Copies an existing company.-TESTED
 *   companyCompare    - Compares two companies with email as compare key.-TESTED
 *   companyAddRoom    - Adds a new room to the company.-TESTED
 *   companyRemoveRoom - Removes a room from the company.-TESTED
 *   companyGetFaculty - Gets the Faculty who owns the company.-TESTED
 *   companyGetEmail   - Gets the email of the company.-TESTED
 *   companyFindRoom   - Find a room in the company rooms set by its id.
 *   companyCheckInput - Check if Input is valid.
*/

/** Type for defining the Company */
typedef struct company_t *Company;
#ifndef ROOMSET
#define ROOMSET
typedef Set RoomSet;
#endif
/** Type used for returning error codes from company functions */
typedef enum CompanyResult_t {

    COMPANY_OUT_OF_MEMORY,
    COMPANY_NULL_ARGUMENT,
    COMPANY_INVALID_PARAMETER,
    COMPANY_SUCCESS,
    COMPANY_ROOM_DOES_NOT_EXIST,
    COMPANY_ROOM_ALREADY_EXIST
} CompanyResult;

/**
 * This function creates new company and reset all the company's parameters.
 * @param email The new company email address.
 * @param faculty The faculty that the new company belongs to.
 * @return
 *     return the new company if create succeed.
 *     NULL if memory allocate failed.
 */
CompanyResult companyCreate(char *email, TechnionFaculty faculty,
                            Company* company);

/** This function deallocating a company and all of it resources*/
void companyDestroy(SetElement company);

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
 *  This function return in parameter the faculty that owns the company.
 * @param company The relevant company.
 * @return
 *      The faculty of current company.
 */

TechnionFaculty companyGetFaculty(Company company);
/**
 * This function return the email of a company.
 * @param company The relevant company.
 * @return
 *      NULL if company mail is null.
 *      pointer to company mail otherwise.
 */
CompanyResult companyGetEmail(Company company,char** email);//



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
CompanyResult companyFindRoom(Company company, long id, Room* room);

/**
 * This function checks if a id exist in the company's set of rooms.
 * @param company The relevant company.
 * @param id The id to check.
 * @return
 *      true if room with same id has found.
 *      false otherwise.
 */
CompanyResult companySearchRoom(Company company, long id);

/**
 *
 * @param company
 * @return
 */
RoomSet companyGetRoomsSet(Company company);

bool companyInputCheck(char* email, TechnionFaculty faculty);

#endif //EX3_COMPANY_H