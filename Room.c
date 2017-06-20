#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

#include "Room.h"


/**=========================Macros and Defines================================*/
#define PARAMETER_CHECK(condition) \
    if(!(condition)) {\
        return ROOM_INVALID_PARAMETER; \
    }
#define NULL_CHECK(condition) \
    if(!(condition)){ \
        return ROOM_NULL_ARGUMENT; \
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
    int price_per_person;
    int open_hour;
    int close_hour;
    int num_ppl;
    int difficulty;
};

/**=======================Static functions declarations=======================*/



/**======================ADT functions implementations========================*/

/**------------------------Room Create----------------------------------------*/
RoomResult roomCreate(long id, int price, int num_ppl, int open_hour,
                  int close_hour, int difficulty, Room* room){
    assert(room != NULL);
    if(!roominputCheck(id,price,num_ppl,open_hour,close_hour,difficulty)){
        return ROOM_INVALID_PARAMETER;
    }
    *room = malloc(sizeof(**room));
    if(!*room){
        return ROOM_OUT_OF_MEMORY;
    }
    (*room)->id = id;
    (*room)->difficulty = difficulty;
    (*room)->open_hour = open_hour;
    (*room)->close_hour = close_hour;
    (*room)->num_ppl = num_ppl;
    (*room)->price_per_person = price;
    return ROOM_SUCCESS;
}

/**-------------------------Room Copy-----------------------------------------*/
void* roomCopy(void* room){
    assert(room != NULL);
    Room new_room;
#ifndef NDEBUG
     RoomResult result =
#endif
     roomCreate(((Room)room)->id,((Room)room)->price_per_person,
                ((Room)room)->num_ppl, ((Room)room)->open_hour,
                ((Room)room)->close_hour, ((Room)room)->difficulty, &new_room);
    assert(result != ROOM_INVALID_PARAMETER);
    return (!new_room) ? NULL : new_room;
}

/**-------------------------Room Destroy--------------------------------------*/
void roomDestroy(void* room){
    if(room != NULL){
        free(room);
    }
    return;
}

/**-------------------------Room Compare--------------------------------------*/
int roomCompare(void* room1, void* room2){
    assert((room1 != NULL) && (room2 != NULL));
    assert(roomGetId(room1) > 0 );
    assert(roomGetId(room2) > 0 );
    return roomGetId((Room)room2) - roomGetId((Room)room1);
}

/**-------------------------Room Get Id---------------------------------------*/
long roomGetId(Room room){
    assert(room);
    return room->id;
}

/**------------------------Room Get Price-------------------------------------*/
long roomGetPrice(Room room){
    assert(room);
    return room->price_per_person;
}

/**------------------------Room Get Recommended-------------------------------------*/

int roomGetRecommendedNumOfPeople(Room room){
    assert(room);
    return room->num_ppl;
}

/**------------------------Room Get Difficulty-------------------------------------*/

int roomGetDiffuclty(Room room){
    assert(room);
    return room->difficulty;
}

/**------------------------Room Get Open Hour---------------------------------*/

long roomGetOpenHour(Room room){
    assert(room);
    return room->open_hour;
}

/**------------------------Room Get Close Hour--------------------------------*/
long roomGetCloseHour(Room room){
    assert(room);
    return room->close_hour;
}

/**====================Static functions implementations=======================*/

/**--------------------------Input Check--------------------------------------*/
 bool roominputCheck(long id, int price, int num_ppl, int open_hour,
                                int close_hour, int difficulty){
    return ((id > 0)
            && (price > 0)
            && (price %PRICE_MODULE == 0)
            && (num_ppl >= MIN_PLAYERS_FOR_ROOM)
            && (open_hour >= MIN_HOUR)
            && (close_hour > open_hour)
            && (close_hour <= MAX_HOUR)
            && (difficulty >= MIN_DIFFICULTY_LEVEL)
            && (difficulty <= MAX_DIFFICULTY_LEVEL));
}