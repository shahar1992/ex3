#ifndef EX3_COMPANY_H
#define EX3_COMPANY_H

#include "stdbool.h"

#include "set.h"
#include "mtm_ex3.h"


/** Type for defining the set */
typedef struct company_t *Company;

/** Type used for returning error codes from room functions */
typedef enum CompanyResult_t {
    COMPANY_SUCCESS,
    COMPANY_OUT_OF_MEMORY,
    COMPANY_NULL_ARGUMENT,
    COMPANY_EMAIL_ALREADY_EXISTS,
    COMPANY_DOES_NOT_EXIST,
    COMPANY_ILLEGAL_PARAMETER
} CompanyResult;

/** This function copying a company */
Company copyCompany(Company company);

/** This function deallocating a company and all of it resources*/
void freeCompany(Company company);

/**
* This function used by the Company to identify equal companies by email.
* This function should return:
* 		A positive integer if 'company1' email is lexicographicaly greater.
* 		0 if they're equal;
*		A negative integer if 'company2' email is lexicographicaly greater.
*/
int compareCompanies(Company company1 , Company company2);

/**
 * This function checks if the input parameters are legal company parameters.
 * @param email Company email address to check.
 * @param faculty Faculty to check.
 * @return
 *      False if one or more of the parameters is not legal.
 *      True otherwise.
 */
bool checkIfCompanyParametersLegal(char* email, TechnionFaculty faculty);

/**
 * This function creates new room and reset all the room's parameters.
 * @param email The new company email address.
 * @param faculty The faculty that the new company belongs to.
 * @param company The new company's pointer.
 * @return
 *     return the new company if create succeed.
 *     NULL if memory allocate failed.
 */
CompanyResult createCompany
        (char* email, TechnionFaculty faculty, Company *company);

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
CompanyResult companyGetFaculty(Company company, char** email);

#endif //EX3_COMPANY_H
