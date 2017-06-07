#ifndef EX3_ROOMS_H
#define EX3_ROOMS_H

#include "set.h"
#include "list.h"

/** Type for defining the set */
typedef struct room_t *Room;

/** Type used for returning error codes from room functions */
typedef enum RoomResult_t {
    ROOM_SUCCESS,
    ROOM_OUT_OF_MEMORY,
    ROOM_NULL_ARGUMENT,
    ROOM_INVALID_PARAMETER,
} RoomResult;


/**
 * This function creates new room and reset all the room's parameters.
 * @param id The room's id.
 * @param price The price for a men for enter the room.
 * @param num_ppl Number of people to enter the room together.
 * @param open_hour Room's opening hour.
 * @param close_hour Room's closing hour.
 * @param difficulty The difficulty level for the room.
 * @return
 *      return the new room if create succeed.
 *      NULL if memory allocate failed.
 */
RoomResult roomCreate(long id, int price, int num_ppl, int open_hour,
                int close_hour, int difficulty, Room* room);

/** This function copying a room */
void* roomCopy(void* room);

/** Type of function for deallocating an element of the set */
void roomDestroy(SetElement room);

/**
* This function used by the Room to identify equal Rooms by id.
* This function should return:
* 		A positive integer if the first Room's id is greater;
* 		0 if they're equal;
*		A negative integer if the second Room's id is greater.
*/
int roomCompare(void* room1, void* room2);

/**
 * Function to get id of specific room.
 * @param room The relevant room.
 * @return
 *      0 if 'room' is NULL
 *      id of 'room' otherwise.
 */
long roomGetId(Room room);

long roomGetPrice(Room room);

int roomGetRecommendedNumOfPeople(Room room);

#endif //EX3_ROOMS_H