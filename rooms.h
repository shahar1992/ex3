#ifndef EX3_ROOMS_H
#define EX3_ROOMS_H

#include "set.h"

/** Type for defining the set */
typedef struct room_t *Room;

/** Type used for returning error codes from room functions */
typedef enum RoomResult_t {
    ROOM_SUCCESS,
    ROOM_OUT_OF_MEMORY,
    ROOM_NULL_ARGUMENT,
    ROOM_EMAIL_ALREADY_EXISTS,
    ROOM_DOES_NOT_EXIST,
    ROOM_ILLEGAL_PARAMETER,
    ROOM_IS_NOT_AVAILABLE
} RoomResult;

/** This function copying a room */
Room copyRoom(Room room);

/** Type of function for deallocating an element of the set */
void freeRoom(Room room);

/**
* This function used by the Room to identify equal Rooms by id.
* This function should return:
* 		A positive integer if the first Room's id is greater;
* 		0 if they're equal;
*		A negative integer if the second Room's id is greater.
*/
int compareRooms(Room room1 , Room room2);

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
bool checkIsRoomParameters(int id,int price,int num_ppl,char* working_hrs,
                           int difficulty);

/**
 *
 * @param email The mail of the company ownes the room.
 * @param id The room's id.
 * @param price The price for a men for enter the room.
 * @param num_ppl Number of people to enter the room together.
 * @param working_hrs The working hours for the room.
 * @param difficulty The difficulty level for the room.
 * @return
 *      ROOM_SUCCESS if create succeed.
 *      ROOM_OUT_OF_MEMORY if memory allocate failed.
 */
Room createRoom(char* email, int id, int price, int num_ppl, char* working_hrs,
                int difficulty);
/**
 * This function remove a room from the system and release all the resources.
 * @param room The room to destroy.
 */
void destroyRoom(Room room);

/**
 *
 * @param room The room to enter.
 * @param days_to_order Number of days to the order's day.
 * @param order_hour The hour in the day for the order.
 * @return
 *      ROOM_NULL_ARGUMENT if 'room' is NULL.
 *      ROOM_IS_NOT_AVAILABLE if room is not available.
 *      ROOM_SUCCESS otherwise.
 */
RoomResult roomAvailability(Room room, int days_to_order, int order_hour);

/**
 *
 * @param room The releavant room.
 * @param id
 * @return
 */
RoomResult roomGetId(Room room, int* id);

#endif //EX3_ROOMS_H
