//
// Created by Amir on 31/05/2017.
//

#include "test_utilities.h"
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "../Order.h"

static bool testOrderCreate() {
    ASSERT_TEST(listCreate(NULL,NULL) == NULL);
    ASSERT_TEST(listCreate(NULL,NULL) == NULL);
    ASSERT_TEST(listCreate(copyString,NULL) == NULL);
    ASSERT_TEST(listCreate(NULL,freeString) == NULL);

    return true;
}