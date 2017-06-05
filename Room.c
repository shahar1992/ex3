#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#include "Room.h"

/**=========================Macros and Defines================================*/
#define PARAMETER_CHECK(condition) \
    if(!(condition)) {\
        return ROOM_INVALID_PARAMETER; \
    }

#define MEMORY_CHECK_NULL(condition,ptr_to_destroy) {\
    if (!(condition)) { \
        roomDestroy(ptr_to_destroy); \
    return ROOM_OUT_OF_MEMORY; \
    } \
}

/**=========================ADT Room Struct===================================*/
struct room_t {
    long id;
    int price;
    int open_hour;
    int close_hour;
    int num_ppl;
    int difficulty;
    Company company;
};

/**=======================Static functions declarations=======================*/
static bool inputCheck(long id, int price, int num_ppl, int open_hour,
                int close_hour, int difficulty);


/**======================ADT functions implementations========================*/

/**------------------------Room Create----------------------------------------*/
RoomResult roomCreate(long id, int price, int num_ppl, int open_hour,
                  int close_hour, int difficulty, Company company, Room* room){
    if(!room){
        return ROOM_NULL_ARGUMENT;
    }
    PARAMETER_CHECK(inputCheck(id,price,num_ppl,open_hour,close_hour,
                               difficulty));
    *room = malloc(sizeof(*room));
    MEMORY_CHECK_NULL(*room, *room);
    (*room)->id = id;
    (*room)->difficulty = difficulty;
    (*room)->open_hour = open_hour;
    (*room)->close_hour = close_hour;
    (*room)->num_ppl = num_ppl;
    (*room)->price = price;
    (*room)->company = company;
    return ROOM_SUCCESS;
}

/**-------------------------Room Copy-----------------------------------------*/
SetElement roomCopy(SetElement room){
    assert(room != NULL);
    Room new_room;
#ifndef NDEBUG
     RoomResult result =
#endif
     roomCreate(((Room)room)->id,((Room)room)->price, ((Room)room)->num_ppl,
                ((Room)room)->open_hour, ((Room)room)->close_hour,
                ((Room)room)->difficulty, ((Room)room)->company, &new_room);
    assert(result != ROOM_INVALID_PARAMETER);
    if(!new_room){
        return NULL;
    }
    return new_room;
}

/**-------------------------Room Destroy--------------------------------------*/
void roomDestroy(SetElement room){
    if(room != NULL) {
        free(room);
    }
    return;
}

/**-------------------------Room Compare--------------------------------------*/
int roomCompare(SetElement room1, SetElement room2){
    assert((room1 != NULL) && (room2 != NULL));
    assert(roomGetId(room1) > 0 );
    assert(roomGetId(room2) > 0 );
    TechnionFaculty faculty1 = companyGetFaculty(((Room)room1)->company);
    TechnionFaculty faculty2 = companyGetFaculty(((Room)room2)->company);
    if(faculty1 == faculty2){
        return roomGetId(room1) - roomGetId(room2);
    }
    return faculty1 - faculty2;
}

/**-------------------------Room Get Id---------------------------------------*/
long roomGetId(Room room){
    if(!room){
        return 0;
    }
    return room->id;
}

Company roomGetCompany(Room room){
    if(!room){
        return NULL;
    }
    return room->company;
}

long orderGetPrice(Room room){
    if(!room){
        return 0;
    }
    return room->price;
}


/**====================Static functions implementations=======================*/

/**--------------------------Input Check--------------------------------------*/
static bool inputCheck(long id, int price, int num_ppl, int open_hour,
                                int close_hour, int difficulty){
    return ((id > 0) && (price > 0) && (price %4 == 0) && (num_ppl > 0)
            && (open_hour >= 0) && (close_hour > open_hour) && (close_hour < 24)
            && (difficulty >= 1) && (difficulty <= 10));
}