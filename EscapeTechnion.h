#ifndef EX3_ESCAPETECHNION_H
#define EX3_ESCAPETECHNION_H

#include <stdbool.h>

#include "list.h"
#include "set.h"
#include "Company.h"
#include "order.h"
#include "Escaper.h"


/**
 *  Abstract company.
 *
 * Implements a Company data type.
 *
 * The following functions are available:
 *   systemCreate	   - Creates a new empty set
 *   systemDestroy	   - Deletes an existing company and frees all resources.
 *   systemAddRoom    - Adds a new room to a company in the system.
 *   companyRemoveRoom - Removes a room from a company in the system.
 *   companyFindRoom   - Find a room in the company rooms set by its id.
*/

/** Type for defining the Company */
typedef struct system_t *System;

typedef Set CompanySet;

typedef List OrdersList;

typedef Set EscaperSet;

typedef Set RoomsSet;

/**
 * This function creates new system and reset all the system's parameters.
 * @param system The parameter to save a pointer to the new system.
 * @return
 *     MTM_OUT_OF_MEMORY if memory allocate failed.
 *     MTM_SUCCESS the new system if create succeed.
 */
MtmErrorCode systemCreate(System* system);

/**
 * This function deletes an existing company and frees all resources.
 * @param system The system to destroy.
 */
void systemDestroy(System system);

/**
 * This function add a new company to the system.
 * @param system The relevant system
 * @param email The email of the new company.
 * @param faculty The faculty that the new company belongs to.
 * @return
 *      SYSTEM_INVALID_PARAMETER if email or faculty is not valid.
 *      SYSTEM_OUT_OF_MEMORY if allocation failed
 *      SYSTEM_SUCCESS otherwise.
 */
MtmErrorCode systemAddCompany(System system, char* email,
                              TechnionFaculty faculty);

/**
 * This function remove a company from the system.
 * @param system The relebant system.
 * @param email The mail to identify the company.
 * @return
 *      SYSTEM_COMPANY_EMAIL_DOES_NOT_EXIST if email does not belong to
 *                                          any company.
 *      SYSTEM_NULL_ARGUMENT if system or email are null.
 *      SYSTEM_SUCEES otherwise.
 */
MtmErrorCode systemRemoveCompany(System system, char* email);

/* This function add a room to a company in the system.*/
/**
 *
 * @param system The relevant system.
 * @param email The email of the company that the new room belongs to.
 * @param id The id of the new room.
 * @param price The price for a men in the new room.
 * @param num_ppl Number of people that can enter the room.
 * @param open_hour The opening hour of the room.
 * @param close_hour The closing hour of the room.
 * @param difficulty The difficulty level of the room.
 * @return
 *      SYSTEM_NULL_ARGUMENT if system is null.
 *      SYSTEM_OUT_OF_MEMORY if allocation failed.
 *      SYSTEM_ID_IS_ALREADY_EXIST Room with same id exist in the same company.
 *      SYSTEM_COMPANY_EMAIL_DOES_NOT_EXIST if email does not belong to
 *                                          any company.
 *      SYSTEM_SUCCESS otherwise.
 */
MtmErrorCode systemAddRoom(System system, char* email, long id, long price,
                            int num_ppl, int open_hour, int close_hour,
                            int difficulty);

/**
 * This function remove a room from the company rooms list.
 * @param company The relevant company.
 * @param room The room to remove.
 * @return
 *      COMPANY_NULL_ARGUMENT if 'company' or 'room' are null.
 *      COMPANY_SUCCESS otherwise.
 */
MtmErrorCode systemRemoveRoom(System system, TechnionFaculty, long id);


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



#endif //EX3_ESCAPETECHNION_H
