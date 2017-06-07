#include "list_mtm/test_utilities.h"
#include "../Order.h"
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

static bool testOrderCreate() {
    Order new_order=orderCreate("amir@walla.com",UNKNOWN,21412421,"10-10",4);
    OrderPrint(new_order);
    OrderDestroy(new_order);
    new_order=orderCreate("amirwalla.com",UNKNOWN,12124,"10-10",4);
    assert(new_order==NULL);
    new_order=orderCreate("Sahar@isgay",MATHEMATICS,201,"2-24",15);
    assert(new_order==NULL);
    //OrderPrint(new_order2);
    //OrderDestroy(new_order2);

    return true;
}

static bool testOrderCopy() {
    Order order= orderCreate("shahar92@campux.technion.ac.il",ELECTRICAL_ENGINEERING,5060,"2-01",5);
    Order order1=OrderCopy(order);
    OrderPrint(order1);
    OrderDestroy(order1);
    OrderDestroy(order);
    return true;
}

static bool testOrderCmp(){
    Order order1 = orderCreate("shahar92@campus.technion.ac.il",ELECTRICAL_ENGINEERING,23,"01-11",8);
    Order  order2 = orderCreate("1@2",ELECTRICAL_ENGINEERING,44,"00-9",3);
    OrderPrint(order1);
    OrderPrint(order2);
    printf("The orders are %s",OrderCmpByTime(order1,order2)?"DIFFRENT":"SAME");
    OrderDestroy(order1);
    OrderDestroy(order2);
    return true;
}

int main (int argv, char** arc){
    RUN_TEST(testOrderCreate);
    RUN_TEST(testOrderCopy);
    RUN_TEST(testOrderCmp);
    //RUN_TEST(testCompanyAddRoom);
    //RUN_TEST(testCompanyRemoveRoom);
    //RUN_TEST(testCompanyGetFaculty);
    //RUN_TEST(testCompanyGetEmail);
    //RUN_TEST(testCompanyFindRoom);
    return 0;
}