#ifndef EX3_ESCAPETECHNION_H
#define EX3_ESCAPETECHNION_H

#include <stdbool.h>

#include "list.h"
#include "set.h"
#include "Company.h"
#include "order.h"
#include "Escaper.h"


/**
 *  Abstract escapeTechnion data type.
 *
 * The following functions are available:
 *   escapeTechnionCreate	   - Creates a new empty Escape Technion system.
 *   escapeTechnionDestroy	   - Deletes an existing company and frees all resources.
 *   escapeTechnionAddRoom    - Adds a new room to a company in the system.
 *   escapeTechnionRemoveRoom - Removes a room from a company in the system.
 *
*/

/** Type for defining the Company */
typedef struct EscapeTechnion_t *EscapeTechnion;

typedef Set CompanySet;
typedef List OrdersList;
typedef Set EscaperSet;
typedef Set RoomSet;


typedef enum {
    ESCAPE_TECHNION_OUT_OF_MEMORY,
    ESCAPE_TECHNION_NULL_PARAMETER,
    ESCAPE_TECHNION_INVALID_PARAMETER,
    ESCAPE_TECHNION_EMAIL_ALREADY_EXISTS,
    ESCAPE_TECHNION_COMPANY_EMAIL_DOES_NOT_EXIST,
    ESCAPE_TECHNION_CLIENT_EMAIL_DOES_NOT_EXIST,
    ESCAPE_TECHNION_ID_ALREADY_EXIST,
    ESCAPE_TECHNION_ID_DOES_NOT_EXIST,
    ESCAPE_TECHNION_CLIENT_IN_ROOM,
    ESCAPE_TECHNION_ROOM_NOT_AVAILABLE,
    ESCAPE_TECHNION_RESERVATION_EXISTS,
    ESCAPE_TECHNION_NO_ROOMS_AVAILABLE,
    ESCAPE_TECHNION_SUCCESS,
} EscapeTechnionResult;

/**
 * This function creates new system and reset all the system's parameters.
 * @param system The parameter to save a pointer to the new system.
 * @return
 *     MTM_OUT_OF_MEMORY if memory allocate failed.
 *     MTM_SUCCESS the new system if create succeed.
 */
EscapeTechnionResult escapeTechnionCreate(EscapeTechnion *system);

/**
 * This function deletes an existing company and frees all resources.
 * @param system The system to destroy.
 */
void escapeTechnionDestroy(EscapeTechnion system);

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
EscapeTechnionResult escapeTechnionAddCompany(EscapeTechnion system,
                                              char *email,
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
EscapeTechnionResult escapeTechnionRemoveCompany(EscapeTechnion system, char *email);

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
EscapeTechnionResult escapeTechnionAddRoom(EscapeTechnion system, char *email,
                                           long id, long price, int num_ppl,
                                           int open_hour, int close_hour,
                                           int difficulty);

/**
 * This function remove a room from the system.
 * @param system The relevant system.
 * @param faculty The room to remove.
 * @return
 *      ESCAPE_TECHNION_NULL_ARGUMENT if 'company' or 'room' are null.
 *      ESCAPE_TECHNION_RESERVATION_EXISTS if a reservation exist to the relevant room.
 *      ESCAPE_TECHNION_SUCCESS otherwise.
 */
EscapeTechnionResult escapeTechnionRemoveRoom(EscapeTechnion system,
                                              TechnionFaculty faculty, long id);

/**
 * This function adds a client to the system's clients set.
 * @param system The relevant system
 * @param email The client email address.
 * @param faculty The client's faculty .
 * @param skill_level the client's skill level.
 * @return
 *      ESCAPE_TECHNION_NULL_ARGUMENT if system is null.
 *      ESCAPE_TECHNION_OUT_OF_MEMORY if allocation failed.
 *      ESCAPE_TECHNION_ID_IS_ALREADY_EXIST Room with same id
 *          exist in the same company.
 *      ESCAPE_TECHNION_COMPANY_EMAIL_DOES_NOT_EXIST if email does not belong
 *          to any company.
 *      ESCAPE_TECHNION_SUCCESS otherwise.
 */
EscapeTechnionResult escapeTechnionAddClient(EscapeTechnion system, char* email,
                                             TechnionFaculty faculty,
                                             int skill_level);

#endif //EX3_ESCAPETECHNION_H
