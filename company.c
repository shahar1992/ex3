#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#include "company.h"
#include "set.h"

/**----------------------Macros and structs-----------------------------------*/
#define FACULTIES_NUMBER 19

#define NULL_PARAMETER_CHECK(ptr) \
    if((ptr) == NULL) {\
        return COMPANY_NULL_ARGUMENT; \
    }

#define MEMORY_CHECK_NULL(ptr) \
    if ((ptr) == NULL) { \
        return COMPANY_OUT_OF_MEMORY; \
    }

struct company_t{
    char* email;
    TechnionFaculty faculty;
    RoomSet Rooms;
};



/**====================End of macros and structs==============================*/


/**---------------------Static functions declarations.------------------------*/

/**
 * Create a copy of each room belongs to 'company1' in 'company2'.
 */
static CompanyResult copyRoomsSet(RoomSet company1, RoomSet company2);

/**
 * Destroy and deallocate all rooms in company.
 */
static void destroyRoomSet(RoomSet rooms);

/**===================End of static function declarations.====================*/


/**-------------------Company ADT functions implementation--------------------*/

Company copyCompany(Company company){
    assert(company != NULL);
    Company new_company;
    if(createCompany(company->email, company->faculty, &new_company)
       != COMPANY_SUCCESS) {
        return NULL;
    }
    if(copyRoomsSet(company->Rooms,new_company->Rooms) != COMPANY_SUCCESS){
        freeCompany(new_company);
        return NULL;
    }
    return new_company;
}

void freeCompany(Company company){
    if(company != NULL){
        destroyRoomSet(company->Rooms);
        free(company);
    }
    return;
}

int compareCompanies(Company company1 , Company company2){
    assert((company1 != NULL ) && (company2 != NULL));
    return strcmp(company1->email, company2->email);
}

bool checkIfCompanyParametersLegal(char* email, int faculty){
    assert(email != NULL);
    if ((faculty >= 0 && faculty <= (FACULTIES_NUMBER-1))){
        if(emailIsLegal(email)){
            return true;
        }
    }
    return false;
}

CompanyResult createCompany
        (char* email, TechnionFaculty faculty, Company *company){
    NULL_PARAMETER_CHECK(email);
    NULL_PARAMETER_CHECK(company);
    *company = malloc(sizeof(**company));
    MEMORY_CHECK_NULL(*company);
    (*company)->email = malloc(sizeof(char)*(strlen(email)+1));
    if(!(*company)->email){
        freeCompany(*company);
        return COMPANY_OUT_OF_MEMORY;
    }
    (*company)->email = strcpy((*company)->email, email);
    (*company)->faculty = faculty;
    (*company)->Rooms = NULL;
    return COMPANY_SUCCESS;
}

CompanyResult companyGetFaculty(Company company, TechnionFaculty* faculty){

}

CompanyResult companyGetEmail(Company company, char** email){

}

CompanyResult companyAddRoom(Company company, Room room){

}

CompanyResult companyRemoveRoom(Company company, Room room){

}

CompanyResult companyFindRoom(Company company, int id, Room* room){

}

/**==================END of company ADT functions implementation==============*/

/**-------------------static functions implementation-------------------------*/
static CompanyResult copyRoomsSet(RoomSet company1, RoomSet company2){

}

static void destroyRoomSet(RoomSet rooms){

}

static bool emailIsLegal(char* email){

}

/**==================End of static functions implementation===================*/