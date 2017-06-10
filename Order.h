#ifndef EX3_ORDER_H
#define EX3_ORDER_H

#include <stdbool.h>
#include "mtm_ex3.h"
#include "Escaper.h"
#include "Room.h"

/**
 *  Order ADT
 *
 * Implements an order struct.
 * An Order is a struct of a future order in the system.
 *
 * The following functions are available:
 *
 *   OrderCreate                - Creates a new Order
 *   OrderDestroy               - Deletes an existing Order and frees all resources
 *   OrderCopy                  - Copies an existing Order
 *   OrderGetFaculty            - return the faculty enum for a given order
 *  OrderCmpByTime              - cmp 2 orders by time
 *  OrderCmpByTime_Faculty_ID   - cmp 2 orders by time,faculty,id
 *  OrderPrint                  - Print A given order
 *  orderGetEmail               - Get the company email of an order.
 *  orderGetId                  - Get the id of the ordered room.
 *  orderGetFaculty             - Get the faculty owns the ordered room.
 */

/** Type for defining an Order*/
typedef struct Order_t* Order;

/** Type used for returning error codes from Order functions */
typedef enum OrderResult_t {
    ORDER_SUCCESS,
    ORDER_OUT_OF_MEMORY,
    ORDER_NULL_ARGUMENT,
    ORDER_INVALID_PARAMETER
} OrderResult;

/** Allocates a new Order */
OrderResult orderCreate(int num_of_ppl, int hour, int day,
                        TechnionFaculty faculty, Room room, Escaper escaper,
                        Order* order);

/** Frees an existing Order object */
void orderDestroy(void* order);

/** Allocates a new Order which is a copy of the argument */
void* orderCopy(void* order);

/** Get the order's faculty */
OrderResult OrderGetFaculty(Order order, TechnionFaculty* faculty);

/** Compare 2 orders by time only return 0 if equal */
int orderCompare(void* order1, void* order2);

/**Get the faculty owns the ordered room*/
OrderResult orderGetFaculty(Order order, TechnionFaculty* faculty);

Escaper orderGetEscaper(Order order);

/*Get the ordered room.*/
Room orderGetRoom(Order order);

int orderGetDay(Order order);

/**Get the price of an order*/
long orderCalculatePrice(Order order);

/**can be used as list filter function*/
bool orderFilterByFacultiesAndId(void* order, void* faculty_and_id_array);

bool orderFilterByDayAndHour(void* order, void* time_array);

#endif //EX3_ORDER_H
