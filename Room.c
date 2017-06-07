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
static bool inputCheck(long id, int price, int num_ppl, int open_hour,
                int close_hour, int difficulty);


/**======================ADT functions implementations========================*/

/**------------------------Room Create----------------------------------------*/
RoomResult roomCreate(long id, int price, int num_ppl, int open_hour,
                  int close_hour, int difficulty, Room* room){
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
    if(!new_room){
        return NULL;
    }
    return new_room;
}

/**-------------------------Room Destroy--------------------------------------*/
void roomDestroy(void* room){
    if(room != NULL) {
        free(room);
    }
    return;
}

/**-------------------------Room Compare--------------------------------------*/
int roomCompare(void* room1, void* room2){
    assert((room1 != NULL) && (room2 != NULL));
    assert(roomGetId(room1) > 0 );
    assert(roomGetId(room2) > 0 );
    return roomGetId((Room)room1) - roomGetId((Room)room2);
}

/**-------------------------Room Get Id---------------------------------------*/
long roomGetId(Room room){
    if(!room){
        return 0;
    }
    return room->id;
}

/**------------------------Room Get Price-------------------------------------*/
long roomGetPrice(Room room){
    if(!room){
        return 0;
    }
    return room->price_per_person;
}



int roomGetRecommendedNumOfPeople(Room room){
    if(!room){
        return 0;
    }
    return room->num_ppl;
}


/**====================Static functions implementations=======================*/

/**--------------------------Input Check--------------------------------------*/
static bool inputCheck(long id, int price, int num_ppl, int open_hour,
                                int close_hour, int difficulty){
    return ((id > 0) && (price > 0) && (price %4 == 0) && (num_ppl > 0)
            && (open_hour >= 0) && (close_hour > open_hour)
            && (close_hour <= 24) && (difficulty >= 1) && (difficulty <= 10));
}

