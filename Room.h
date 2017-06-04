#ifndef EX3_ROOMS_H
#define EX3_ROOMS_H

#include "set.h"

/** Type for defining the set */
typedef struct room_t *Room;

/** Type used for returning error codes from room functions */
typedef enum RoomResult_t {
    ROOM_SUCCESS,
    ROOM_OUT_OF_MEMORY,
    ROOM_NULL_ARGUMENT
} RoomResult;

/** This function copying a room */
SetElement roomCopy(SetElement room);

/** Type of function for deallocating an element of the set */
void roomDestroy(SetElement room);

/**
* This function used by the Room to identify equal Rooms by id.
* This function should return:
* 		A positive integer if the first Room's id is greater;
* 		0 if they're equal;
*		A negative integer if the second Room's id is greater.
*/
int roomCompare(SetElement room1, SetElement room2);

/**
 * This function checks if the input parameters are legal room parameters.
 * @param id The room's id.
 * @param price The price for a men for enter the room.
 * @param num_ppl Number of people to enter the room together.
 * @param working_hrs The working hours for the room.
 * @return
 *      False if one or more of the parameters is not legal.
 *      True otherwise.
 */
bool roomCheckIfParametersLegal(int id, int price, int num_ppl, int open_hour,
                                int close_hour, int difficulty);

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
Room roomCreate(int id, int price, int num_ppl, int open_hour,
                      int close_hour, int difficulty);

/**
 *
 * @param room Room to enter.
 * @param days_to_order Number of days to the order's day.
 * @param order_hour The hour in the day for the order.
 * @return
 *      ROOM_NULL_ARGUMENT if 'room' is NULL.
 *      ROOM_IS_NOT_AVAILABLE if room is not available.
 *      ROOM_SUCCESS otherwise.
 */
RoomResult roomAvailability(Room room, int days_to_order, int order_hour);

/**
 * Function to get id of specific room.
 * @param room The relevant room.
 * @return
 *      0 if 'room' is NULL
 *      id of 'room' otherwise.
 */
int roomGetId(Room room);

#endif //EX3_ROOMS_H