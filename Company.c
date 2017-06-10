#include <assert.h>
#include <string.h>
#include <stdlib.h>

#include "Company.h"

/**======================Macros and structs===================================*/
#define FACULTY_NUM 19

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
static bool isInputLegal(char* email, TechnionFaculty faculty);
static bool isEmailLegal(char* email);
static CompanyResult convertReturnTypeFromRoom(RoomResult result);

/**===================End of static function declarations.====================*/




/**===================Company ADT functions implementation====================*/

CompanyResult companyCreate(char *email, TechnionFaculty faculty,
                            Company* company){
    NULL_ARGUMENT_CHECK(company && email);
    PARAMETER_CHECK(isInputLegal(email,faculty));
    *company = malloc(sizeof(**company));
    MEMORY_CHECK(*company,*company);
    (*company)->email = malloc(sizeof(char)*(strlen(email)+1));
    MEMORY_CHECK((*company)->email,*company);
    strcpy((*company)->email, email);
    (*company)->faculty = faculty;
    (*company)->rooms = setCreate(roomCopy, roomDestroy, roomCompare);
    MEMORY_CHECK((*company)->rooms,*company);
    return COMPANY_SUCCESS;
}

void companyDestroy(SetElement company){
    if(company != NULL){
        setDestroy(((Company)company)->rooms);
        if(((Company)company)->email != NULL) {
            free(((Company)company)->email);
        }
        free(company);
    }
    return;
}

SetElement companyCopy(SetElement company){
    Company new_company;
#ifndef NDEBUG
    CompanyResult result =
#endif
    companyCreate(((Company)company)->email,((Company)company)->faculty,
                  &new_company);
    assert(result != COMPANY_INVALID_PARAMETER);
    if(result != COMPANY_SUCCESS) {
        return NULL;
    }
    new_company->rooms = setCopy(((Company)company)->rooms);
    MEMORY_CHECK(new_company->rooms,new_company);
    return new_company;
}

int companyCompare(SetElement company1, SetElement company2){
    assert((company1 != NULL ) && (company2 != NULL));
    return strcmp(((Company)company1)->email, ((Company)company2)->email);
}

CompanyResult companyAddRoom(Company company, long id, long price, int num_ppl,
                             int open_hour, int close_hour, int difficult){

    Room room;
    CompanyResult result = convertReturnTypeFromRoom(
            roomCreate(id,price,num_ppl,open_hour,close_hour,difficult,&room));
    if(result != COMPANY_SUCCESS){
        return result;
    }
    NULL_ARGUMENT_CHECK(company);
    result = convertReturnTypeFromSet(setAdd(company->rooms, room));
    roomDestroy(room);
    return result;
}

CompanyResult companyRemoveRoom(Company company, long id){
    NULL_ARGUMENT_CHECK(company);
    if(id < 0){
        return COMPANY_INVALID_PARAMETER;
    }
    CompanyResult result;
    SET_FOREACH(Room, room, company->rooms){
        if(roomGetId(room) == id){
            result = convertReturnTypeFromSet(setRemove(company->rooms,room));
            if(result != COMPANY_ROOM_DOES_NOT_EXIST){
                return result;
            }
        }
    }
    return COMPANY_ROOM_DOES_NOT_EXIST;
}

CompanyResult companyGetFaculty(Company company, TechnionFaculty* faculty){
    NULL_ARGUMENT_CHECK(company && faculty);
    *faculty = company->faculty;
    return COMPANY_SUCCESS;
}

char* companyGetEmail(Company company){
    if(!company){
        return NULL;
    }
    return company->email;
}

CompanyResult companyFindRoom(Company company, long id, Room* room){
    PARAMETER_CHECK(id > 0);
    NULL_ARGUMENT_CHECK(company && room);
    SET_FOREACH(Room,current_room,company->rooms){
        assert(current_room != NULL);
        if(roomGetId(current_room) == id){
            *room = current_room;
            return COMPANY_SUCCESS;
        }
    }
    *room = NULL;
    return COMPANY_ROOM_DOES_NOT_EXIST;
}

CompanyResult companySearchRoom(Company company, long id){
    NULL_ARGUMENT_CHECK(company);
    SET_FOREACH(Room, current_room,company->rooms){
        if(roomGetId(current_room) == id){
            return COMPANY_SUCCESS;
        }
    }
    return COMPANY_ROOM_DOES_NOT_EXIST;
}

RoomSet companyGetRoomsSet(Company company){
    if(!company){
        return NULL;
    }
    return  setCopy(company->rooms);
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

static bool isInputLegal(char* email, TechnionFaculty faculty){
    assert(email);
    return (isEmailLegal(email) && (faculty >= 0) && (faculty < FACULTY_NUM));
}

static bool isEmailLegal(char* email){
    assert(email);
    bool sign_flag = false;
    while(email){
        if(*(email++) == '@'){
            if(sign_flag){
                return false;
            }
            sign_flag = true;
        }
    }
    return true;
}



/**==================End of static functions implementation===================*/