#include "list_mtm/test_utilities.h"
#include "../Order.h"
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

static bool testOrderCreate() {
    Order new_order=OrderCreate("amir@walla.com",UNKNOWN,21412421,"10-10",4);
    OrderPrint(new_order);
    OrderDestroy(new_order);
    new_order=OrderCreate("amirwalla.com",UNKNOWN,12124,"10-10",4);
    assert(new_order==NULL);
    new_order=OrderCreate("Sahar@isgay",MATHEMATICS,201,"2-24",15);
    assert(new_order==NULL);
    //OrderPrint(new_order2);
    //OrderDestroy(new_order2);

    return true;
}

static bool testOrderCopy() {
    Order order= OrderCreate("shahar92@campux.technion.ac.il",ELECTRICAL_ENGINEERING,5060,"2-01",5);
    Order order1=OrderCopy(order);
    OrderPrint(order1);
    OrderDestroy(order1);
    OrderDestroy(order);
    return true;
}
/*
static bool testCompanyCompare(){
    Company company2 = companyCreate("shahar92@campus.technion.ac.il",ELECTRICAL_ENGINEERING);
    Company  company1 = companyCreate("1@2",ELECTRICAL_ENGINEERING);
    ASSERT_TEST(companyCompare(company1,company2) != 0);
    companyFree(company2);
    companyFree(company1);
    return true;
}
*/
int main (int argv, char** arc){
    RUN_TEST(testOrderCreate);
    RUN_TEST(testOrderCopy);
    //RUN_TEST(testCompanyCompare);
    //RUN_TEST(testCompanyAddRoom);
    //RUN_TEST(testCompanyRemoveRoom);
    //RUN_TEST(testCompanyGetFaculty);
    //RUN_TEST(testCompanyGetEmail);
    //RUN_TEST(testCompanyFindRoom);
    return 0;
}