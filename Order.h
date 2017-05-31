//
// Created by Amir on 31/05/2017.
//

#ifndef EX3_ORDER_H
#define EX3_ORDER_H

#include <stdbool.h>
#include "mtm_ex3.h"

/**
*  Order ADT
*
* Implements an Order struct
* An Order is a struct of a future order in the system
*
* The following functions are available:
*
*   OrderCreate               - Creates a new Order
*   OrderDestroy              - Deletes an existing Order and frees all resources
*   OrderCopy                 - Copies an existing Order
*   OrderGetFaculty           -return the faculty enum for a given order
 *  OrderCmpByTime            -cmp 2 orders by time
 *  OrderCmpByTime_Faculty_ID   -cmp 2 orders by time,faculty,id
 *  OrderPrint                 -Print A given order
*
*/

/** Type for defining an Order*/
typedef struct Order_t* Order;

/** Type used for returning error codes from Order functions */
typedef enum OrderResult_t {
    Order_SUCCESS,
    Order_OUT_OF_MEMORY,
    Order_NULL_ARGUMENT,
    Orders_ARE_EQUAL,
    Orders_NOT_EQUAL,
} OrderResult;

/** Allocates a new Order */
Order OrderCreate(char* email, TechnionFaculty faculty, int id,char* time,int num_of_ppl);

/** Frees an existing Order object */
OrderResult OrderDestroy(Order order);

/** Allocates a new Order which is a copy of the argument */
Order OrderCopy(Order order);

/** Get the order's faculty */
TechnionFaculty OrderGetFaculty(Order order);

/** Compare 2 orders by time only return 0 if equal */
int OrderCmpByTime(Order order1,Order order2);

/**Compare 2 orders by room faculty,id and time */
int OrderCmpByTime_Faculty_ID(Order order1,Order order2);


/**Print A GIVEN ORDER*/
void OrderPrint(Order order);



#endif //EX3_ORDER_H
