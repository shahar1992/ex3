#include <assert.h>
#include <string.h>
#include <stdlib.h>

#include "Company.h"
#include "set.h"

/**======================Macros and structs===================================*/
#define FACULTY_NUM ((int)UNKNOWN)

#define NULL_ARGUMENT_CHECK(condition) {\
    if(!(condition)) {\
        return COMPANY_NULL_ARGUMENT; \
    } \
}

#define MEMORY_CHECK(condition, company_to_destroy) {\
    if (!(condition)) { \
        companyDestroy(company_to_destroy); \
        return COMPANY_OUT_OF_MEMORY; \
    } \
}

#define PARAMETER_CHECK(condition){ \
    if(!(condition)){ \
        return COMPANY_INVALID_PARAMETER; \
    } \
}

struct company_t{
    char* email;
    TechnionFaculty faculty;
    RoomSet rooms;
};

/**====================End of macros and structs==============================*/




/**=====================Static functions declarations=========================*/

/**converts the result type from set result to comapny result*/
static  CompanyResult convertReturnTypeFromSet(SetResult result);

static bool isEmailLegal(char* email);
//static CompanyResult convertReturnTypeFromRoom(RoomResult result);

/**===================End of static function declarations.====================*/




/**===================Company ADT functions implementation====================*/

CompanyResult companyCreate(char *email, TechnionFaculty faculty,
                            Company* company){
    assert(company);
    if(!email){
        return COMPANY_NULL_ARGUMENT;
    }
    if(!companyInputCheck(email,faculty)){
        return COMPANY_INVALID_PARAMETER;
    }
    *company = malloc(sizeof(**company));
    if(!*company){
        return COMPANY_OUT_OF_MEMORY;
    }
    (*company)->email = malloc(sizeof(char)*(strlen(email)+1));
    if(!(*company)->email){
        companyDestroy(*company);
        return COMPANY_OUT_OF_MEMORY;
    }
    strcpy((*company)->email, email);
    (*company)->faculty = faculty;
    (*company)->rooms = setCreate(roomCopy, roomDestroy,roomCompare);
    if(!(*company)->rooms){
        companyDestroy(*company);
        return COMPANY_OUT_OF_MEMORY;
    }
    return COMPANY_SUCCESS;
}

void companyDestroy(void* company){
    if(company != NULL) {
        setDestroy(((Company) company)->rooms);
        if (((Company) company)->email != NULL) {
            free(((Company) company)->email);
        }
        free(company);
    }
    return;
}

void* companyCopy(void* company){
    Company new_company;
#ifndef NDEBUG
    CompanyResult result =
#endif
    companyCreate(((Company)company)->email,((Company)company)->faculty,
                  &new_company);
    assert(result != COMPANY_INVALID_PARAMETER);
    if(!new_company) {
        return NULL;
    }
    new_company->rooms = setCopy(((Company)company)->rooms);
    if(!new_company->rooms){
        companyDestroy(new_company);
        return NULL;
    }
    return new_company;
}

int companyCompare(SetElement company1, SetElement company2){
    assert((company1 != NULL ) && (company2 != NULL));
    return strcmp(((Company)company1)->email, ((Company)company2)->email);
}

CompanyResult companyAddRoom(Company company, Room room){
    assert(company);
    if(!room){
        return COMPANY_NULL_ARGUMENT;
    }
    CompanyResult result = convertReturnTypeFromSet(
            setAdd(company->rooms, room));
    return result;
}

CompanyResult companyRemoveRoom(Company company, Room room){
    assert(company);
    assert(company->rooms);
    if(!room){
        return COMPANY_NULL_ARGUMENT;
    }
    return (setRemove(company->rooms,room) == SET_SUCCESS) ? COMPANY_SUCCESS :
           COMPANY_ROOM_DOES_NOT_EXIST;
}

TechnionFaculty companyGetFaculty(Company company){
    assert(company);
    return company->faculty;
}

CompanyResult companyGetEmail(Company company,char** email){
    assert(company);
    *email = malloc(sizeof(**email)*(strlen(company->email)+1));
    if(!*email){
        return COMPANY_OUT_OF_MEMORY;
    }
    strcpy(*email,company->email);
    return COMPANY_SUCCESS;
}

CompanyResult companyFindRoom(Company company, long id, Room* room){
    assert(company);
    if(!room){
        return COMPANY_NULL_ARGUMENT;
    }
    if(id < 0){
        return COMPANY_INVALID_PARAMETER;
    }
    SET_FOREACH(Room,current_room,company->rooms){
        if(roomGetId(current_room) == id){
            *room = roomCopy(current_room);
            if(!*room){
                return COMPANY_OUT_OF_MEMORY;
            }
            return COMPANY_SUCCESS;
        }
    }
    *room = NULL;
    return COMPANY_ROOM_DOES_NOT_EXIST;
}

CompanyResult companySearchRoom(Company company, long id){
    assert(company);
    SET_FOREACH(RoomSet, current_room, company->rooms){
        if(roomGetId((Room)current_room) == id){
            return COMPANY_SUCCESS;
        }
    }
    return COMPANY_ROOM_DOES_NOT_EXIST;
}

RoomSet companyGetRoomsSet(Company company){
    assert(company);
    return setCopy(company->rooms);
}

/**==================END of company ADT functions implementation==============*/




/**===================static functions implementation=========================*/

static  CompanyResult convertReturnTypeFromSet(SetResult result){
    switch(result){
        case SET_SUCCESS:
            return COMPANY_SUCCESS;
        case SET_ITEM_DOES_NOT_EXIST:
            return COMPANY_ROOM_DOES_NOT_EXIST;
        case SET_OUT_OF_MEMORY:
            return COMPANY_OUT_OF_MEMORY;
        case SET_ITEM_ALREADY_EXISTS:
            return COMPANY_ROOM_ALREADY_EXIST;

        default:
            return COMPANY_NULL_ARGUMENT;
    }
}
/*
static CompanyResult convertReturnTypeFromRoom(RoomResult result){
    switch (result){
        case ROOM_SUCCESS:
            return COMPANY_SUCCESS;
        case ROOM_OUT_OF_MEMORY:
            return COMPANY_OUT_OF_MEMORY;
        case ROOM_INVALID_PARAMETER:
            return COMPANY_INVALID_PARAMETER;

        default:
            return COMPANY_NULL_ARGUMENT;
    }
}
 */

 bool companyInputCheck(char* email, TechnionFaculty faculty){
    assert(email);
    return (isEmailLegal(email) && (faculty >= 0) && (faculty < FACULTY_NUM));
}

static bool isEmailLegal(char* email){
    assert(email);
    char* ptr=email;
    int counter=0;
    while(*ptr){
        if((*ptr) == '@'){
            counter++;
        }
        ptr++;
    }
    return (counter==1);
}



/**==================End of static functions implementation===================*/