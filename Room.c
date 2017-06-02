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

SetElement roomCopy(SetElement room){
    assert(room != NULL);
    Room new_room = roomCreate(((Room)room)->id, ((Room)room)->price,
                               ((Room)room)->num_ppl, ((Room)room)->open_hour,
                               ((Room)room)->close_hour,
                               ((Room)room)->difficulty);
    if(!new_room){
        return NULL;
    }
    return new_room;
}


void roomDestroy(SetElement room){
    if(room != NULL) {
        free(room);
    }
    return;
}

int roomCompare(SetElement room1, SetElement room2){
    assert((room1 != NULL) && (room2 != NULL));
    assert(roomGetId(room1) > 0 );
    assert(roomGetId(room2) > 0 );
    return roomGetId(room1) - roomGetId(room2) ;
}

}

Room roomCreate(int id, int price, int num_ppl, int open_hour,
                      int close_hour, int difficulty){
    Room  room = malloc(sizeof(*room));
    room->id = id;
    room->difficulty = difficulty;
    room->open_hour = open_hour;
    room->close_hour = close_hour;
    room->num_ppl = num_ppl;
    room->price = price;
    return room;
}

int roomGetId(Room room){
    if(!room){
        return 0;
    }
    return room->id;
}

/*RoomResult roomAvailability(Room room, int days_to_order, int order_hour){
}*/
