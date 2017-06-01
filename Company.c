#include <assert.h>
#include <string.h>
#include <stdlib.h>

#include "Company.h"

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

/**converts the result type from set result to comapny result*/
static  CompanyResult convertReturnType(SetResult result);

/**===================End of static function declarations.====================*/




/**===================Company ADT functions implementation====================*/

Company companyCreate(char *email, TechnionFaculty faculty){
    assert(email != NULL);
    Company  company = malloc(sizeof(*company));
    if(!company){
        return NULL;
    }
    (company)->email = malloc(sizeof(char)*(strlen(email)+1));
    if(!company->email){
        companyFree(company);
        return NULL;
    }
    strcpy(company->email, email);
    company->faculty = faculty;
    company->Rooms =
            (RoomSet)setCreate((SetElement)roomCopy, roomFree, roomCompare);
    if(!company->Rooms){
        companyFree(company);
        return NULL;
    }
}

void companyFree(SetElement company){
    if(company != NULL){
        setDestroy(((Company)company)->Rooms);
        if(((Company)company)->email != NULL) {
            free(((Company)company)->email);
        }
        free(company);
    }
    return;
}

SetElement companyCopy(SetElement company){
    assert(company != NULL);
    Company new_company = companyCreate(((Company)company)->email,
                                ((Company)company)->faculty);
    if(!new_company) {
        return NULL;
    }
    new_company->Rooms = setCopy(((Company)company)->Rooms);
    if(!new_company){
        companyFree(new_company);
        return NULL;
    }
    return new_company;
}



int companyCompare(SetElement company1, SetElement company2){
    assert((company1 != NULL ) && (company2 != NULL));
    return strcmp(((Company)company1)->email, ((Company)company2)->email);
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

CompanyResult companyFindRoom(Company company, int id, Room* room){
    if(!company || !room){
        return COMPANY_NULL_ARGUMENT;
    }
    SET_FOREACH(Room,current_room,company->Rooms){
        assert(current_room != NULL);
        if(roomGetId(current_room) == id){
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