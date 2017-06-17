#ifndef EX3_ORDER_H
#define EX3_ORDER_H

#include <stdbool.h>
#include "mtm_ex3.h"
#include "Escaper.h"
#include "Room.h"
//
#define DISCOUNT_FOT_FACULTY_MEMBERS 3/4
/**
 *  Order ADT
 *
 * Implements an order struct.
 * An Order is a struct of a future order in the system.
 *
 * The following functions are available:
 *
 *   OrderCreate                - Creates a new Order-TESTED
 *   OrderDestroy               - Deletes an existing Order and frees all resources
 *   OrderCopy                  - Copies an existing Orde-Tested
 *   orderCompare               -Compare orders by day and time-Tested
 *   OrderGetFaculty            - return the faculty enum for a given order-Tested
 *  orderGetRoom                 - Print A given order-Tested
 *  orderGetEscaper             - Get the company email of an order.-Tested
 *  orderCalculatePrice             - Get the id of the ordered room.-Tested
 *  orderGetNumOfPeople           - Get the faculty owns the ordered room.-Tested
 *  OrderResult orderGetTimeAndDay - Get Order time and day
 *  OrderResult OrderGetRoomId(Order order,long* id)
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
                        TechnionFaculty faculty,Room  room,Escaper escaper,
                        Order* order);
/** Frees an existing Order object */
void orderDestroy(void* order);

/** Allocates a new Order which is a copy of the argument */
void* orderCopy(void* order);

/** Compare 2 orders by time only return 0 if equal */
int orderCompare(Order order1, Order order2);

/**Get the faculty owns the ordered room*/
OrderResult orderGetFaculty(Order order, TechnionFaculty* faculty);

/**Get the ordered room.*/
Room orderGetRoom(Order order);

/**Get the Customer.*/
Escaper orderGetEscaper(Order order);

/**Get the price of an order.*/
long orderCalculatePrice(Order order);

/**Get the order's number of people.*/
int orderGetNumOfPeople(Order order);

/***/
OrderResult orderGetTimeAndDay(Order order,long* hour,long* day);

OrderResult orderGetRoomId(Order order,long* id);

#endif //EX3_ORDER_H
