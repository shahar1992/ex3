#include <assert.h>
#include <string.h>
#include <stdlib.h>

#include "company.h"

/**======================Macros and structs===================================*/
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




/**=====================Static functions declarations=========================*/

/**
 * Create a copy of each room belongs to 'company1' in 'company2'.
 */
static CompanyResult copyRoomsSet(RoomSet company1, RoomSet company2);

/**
 * Destroy and deallocate all rooms in company.
 */
static void destroyRoomSet(RoomSet rooms);

/**
 * converts the result type from set result to comapny result
 */
static  CompanyResult convertReturnType(SetResult result);

/**===================End of static function declarations.====================*/




/**===================Company ADT functions implementation====================*/

Company companyCopy(Company company){
    assert(company != NULL);
    Company new_company;
    new_company = createCompany(company->email, company->faculty);
    if(!new_company) {
        return NULL;
    }
    new_company->Rooms = setCopy(company->Rooms);
    if(!new_company){
        freeCompany(new_company);
        return NULL;
    }
    return new_company;
}

void freeCompany(Company company){
    if(company != NULL){
        setDestroy(company->Rooms);
        if(company->email != NULL) {
            free(company->email);
        }
        free(company);
    }
    return;
}

int companyCompare(Company company1, Company company2){
    assert((company1 != NULL ) && (company2 != NULL));
    return strcmp(company1->email, company2->email);
}

Company createCompany(char* email, TechnionFaculty faculty){
    assert(email != NULL);
    Company  company = malloc(sizeof(*company));
    if(!company){
        return NULL;
    }
    (company)->email = malloc(sizeof(char)*(strlen(email)+1));
    if(!company->email){
        freeCompany(company);
        return NULL;
    }
    strcpy(company->email, email);
    company->faculty = faculty;
    company->Rooms = setCreate(roomCopy, roomFree, roomCompare);
    if(!company->Rooms){
        freeCompany(company);
        return NULL;
    }
    return company;
}

CompanyResult companyGetFaculty(Company company, TechnionFaculty* faculty){
    if(!company) {
        return COMPANY_NULL_ARGUMENT;
    }
    *faculty = company->faculty;
    return COMPANY_SUCCESS;
}

CompanyResult companyGetEmail(Company company, char** email){
    if(!email || !company){
        return COMPANY_NULL_ARGUMENT;
    }
    *email = malloc(sizeof(char)*(strlen(company->email)+1));
    MEMORY_CHECK_NULL(*email);
    strcpy(*email,company->email);
    return COMPANY_SUCCESS;
}

CompanyResult companyAddRoom(Company company, Room room){
    if(!company || !room){
        return COMPANY_NULL_ARGUMENT;
    }
    CompanyResult result = convertReturnType(setAdd(company->Rooms,room));
    return result;
}

CompanyResult companyRemoveRoom(Company company, Room room){
    if(!company || !room){
        return COMPANY_NULL_ARGUMENT;
    }
    CompanyResult result = convertReturnType(setRemove(company->Rooms,room));
    return result;
}

CompanyResult companyFindRoom(Company company, int id, Room* room){
    if(!company || !room){
        return COMPANY_NULL_ARGUMENT;
    }
    SET_FOREACH(Room,current_room,company->Rooms){
        int current_id;
        assert(current_room != NULL)
        roomGetId(current_room,&current_id);
        if(current_id == id){
            *room = current_room;
            return COMPANY_SUCCESS;
        }
    }
    return COMPANY_ROOM_DOES_NOT_EXIST;
}

/**==================END of company ADT functions implementation==============*/




/**===================static functions implementation=========================*/

static  CompanyResult convertReturnType(SetResult result){
    switch(result){
        case SET_SUCCESS:
            return COMPANY_SUCCESS;
        case SET_ITEM_ALREADY_EXISTS:
            return COMPANY_ROOM_ALREADY_EXISTS;
        case SET_ITEM_DOES_NOT_EXIST:
            return COMPANY_ROOM_DOES_NOT_EXIST;
        case SET_NULL_ARGUMENT:
            return COMPANY_NULL_ARGUMENT;
        case SET_OUT_OF_MEMORY:
            return COMPANY_OUT_OF_MEMORY;
    }
}

/**==================End of static functions implementation===================*/