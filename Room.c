#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#include "Room.h"

#define NULL_PARAMETER_CHECK(ptr) \
    if((ptr) == NULL) {\
        return ROOM_NULL_ARGUMENT; \
    }

#define MEMORY_CHECK_NULL(ptr) \
    if ((ptr) == NULL) { \
        return ROOM_OUT_OF_MEMORY; \
    }

struct room_t {
    int id;
    int price;
    int open_hour;
    int close_hour;
    int num_ppl;
    int difficulty;
};

Room copyRoom(Room room){
    assert(room != NULL);
    Room new_room;
    if(createRoom(room->id,room->price,room->num_ppl, room->open_hour,
               room->open_hour, room->difficulty, &new_room) != ROOM_SUCCESS) {
        return NULL;
    }
    return new_room;
}


void freeRoom(Room room){
    if(room != NULL) {
        free(room);
    }
    return;
}

int compareRooms(Room room1 , Room room2){
    assert((room1 !== NULL) && (room2!=NULL));
    return room1->id - room2->id;
}

bool checkIsRoomParametersLegal(int id,int price,int num_ppl,int open_hour,
                                int close_hour ,int difficulty){
    return (id > 0 && price > 0 && price%4 == 0 && num_ppl > 0 && open_hour < 0
    && open_hour <24 && close_hour > open_hour && close_hour < 24 &&
    difficulty >= 1 && difficulty <= 10);
}

RoomResult createRoom(int id, int price, int num_ppl, int open_hour,
                      int close_hour, int difficulty, Room* room){
    NULL_PARAMETER_CHECK(room);
    *room = malloc(sizeof(**room));
    MEMORY_CHECK_NULL((*room));
    (*room)->id = id;
    (*room)->difficulty = difficulty;
    (*room)->open_hour = open_hour;
    (*room)->close_hour = close_hour;
    (*room)->num_ppl = num_ppl;
    (*room)->price = price;
    return ROOM_SUCCESS;
}

RoomResult roomAvailability(Room room, int days_to_order, int order_hour){
}

RoomResult roomGetId(Room room, int* id){
    NULL_PARAMETER_CHECK(room);
    NULL_PARAMETER_CHECK(id);
    *id = room->id;
    return ROOM_SUCCESS;
}