//
// Created by Amir on 16/06/2017.
//

#include "list_mtm/test_utilities.h"
#include "../EscapeTechnion.h"
#include <string.h>

static bool testSysCreate(){
    EscapeTechnion sys;
    ASSERT_TEST(escapeTechnionCreate(NULL)==ESCAPE_TECHNION_NULL_PARAMETER);
    ASSERT_TEST(escapeTechnionCreate(&sys)==ESCAPE_TECHNION_SUCCESS);
    escapeTechnionDestroy(sys);
    return true;
}

static bool testSysAddCompany(){
    EscapeTechnion sys;
    ASSERT_TEST(escapeTechnionCreate(&sys)==ESCAPE_TECHNION_SUCCESS);
    ASSERT_TEST(escapeTechnionAddCompany(sys,"check12@gmail.com",ELECTRICAL_ENGINEERING)==ESCAPE_TECHNION_SUCCESS);
    ASSERT_TEST(escapeTechnionAddCompany(sys,"check12@gmail.com",ELECTRICAL_ENGINEERING)==ESCAPE_TECHNION_EMAIL_ALREADY_EXISTS);
    ASSERT_TEST(escapeTechnionAddCompany(sys,"check12gmail.com",ELECTRICAL_ENGINEERING)==ESCAPE_TECHNION_INVALID_PARAMETER);
    ASSERT_TEST(escapeTechnionAddCompany(sys,"check12@gmail.com",-1)==ESCAPE_TECHNION_INVALID_PARAMETER);
    ASSERT_TEST(escapeTechnionAddCompany(sys,"check12@gmail.com",20)==ESCAPE_TECHNION_INVALID_PARAMETER);
    ASSERT_TEST(escapeTechnionAddCompany(sys,"@",ELECTRICAL_ENGINEERING)==ESCAPE_TECHNION_SUCCESS);
    ASSERT_TEST(escapeTechnionAddCompany(sys,"check12@gmail.com",COMPUTER_SCIENCE)==ESCAPE_TECHNION_EMAIL_ALREADY_EXISTS);
    escapeTechnionDestroy(sys);
    return true;
}

static bool testSysRemoveCompany(){
    EscapeTechnion sys;
    EscapeTechnionResult result;
    ASSERT_TEST(escapeTechnionCreate(&sys)==ESCAPE_TECHNION_SUCCESS);
    ASSERT_TEST(escapeTechnionAddCompany(sys,"check12@gmail.com",ELECTRICAL_ENGINEERING)==ESCAPE_TECHNION_SUCCESS);
    ASSERT_TEST((result=escapeTechnionRemoveCompany(sys,"check1@gmail.com"))==ESCAPE_TECHNION_COMPANY_EMAIL_DOES_NOT_EXIST);
    ASSERT_TEST(result=escapeTechnionRemoveCompany(sys,"check12@gmail.com")==ESCAPE_TECHNION_SUCCESS);
    escapeTechnionDestroy(sys);
    return true;
}

static bool testSysAddRoom(){
    EscapeTechnion sys;
    EscapeTechnionResult result;
    ASSERT_TEST(escapeTechnionCreate(&sys)==ESCAPE_TECHNION_SUCCESS);
    ASSERT_TEST(escapeTechnionAddCompany(sys,"check12@gmail.com",ELECTRICAL_ENGINEERING)==ESCAPE_TECHNION_SUCCESS);
    ASSERT_TEST(escapeTechnionAddRoom(sys,"check12@gmail.com",1,4,1,0,24,1)==ESCAPE_TECHNION_SUCCESS);
    //null check
    ASSERT_TEST(escapeTechnionAddRoom(sys,NULL,1,4,1,0,24,1)==ESCAPE_TECHNION_NULL_PARAMETER);
    //Parameters check
    ASSERT_TEST(escapeTechnionAddRoom(sys,"check12gmail.com",1,4,1,0,24,1)==ESCAPE_TECHNION_COMPANY_EMAIL_DOES_NOT_EXIST);
    ASSERT_TEST(escapeTechnionAddRoom(sys,"check12@gmail.com",-1,4,1,0,24,1)==ESCAPE_TECHNION_INVALID_PARAMETER);
    ASSERT_TEST(escapeTechnionAddRoom(sys,"check12@gmail.com",3,3,1,0,24,1)==ESCAPE_TECHNION_INVALID_PARAMETER);
    ASSERT_TEST(escapeTechnionAddRoom(sys,"check12@gmail.com",3,3,1,0,25,1)==ESCAPE_TECHNION_INVALID_PARAMETER);
    ASSERT_TEST(escapeTechnionAddRoom(sys,"check12@gmail.com",3,3,1,0,24,10)==ESCAPE_TECHNION_INVALID_PARAMETER);
    ASSERT_TEST(escapeTechnionAddRoom(sys,"check12@gmail.com",3,3,1,0,24,-1)==ESCAPE_TECHNION_INVALID_PARAMETER);
    ASSERT_TEST(escapeTechnionAddRoom(sys,"check12@gmail.com",3,3,1,-1,24,1)==ESCAPE_TECHNION_INVALID_PARAMETER);
    ASSERT_TEST(result=escapeTechnionAddRoom(sys,"check12@gmail.com",1,4,1,0,24,1)==ESCAPE_TECHNION_ID_ALREADY_EXIST);
    escapeTechnionDestroy(sys);
    return true;
}

static bool testSysRemoveRoom(){
    EscapeTechnion sys;
    EscapeTechnionResult result;
    ASSERT_TEST(escapeTechnionCreate(&sys)==ESCAPE_TECHNION_SUCCESS);
    ASSERT_TEST(escapeTechnionAddCompany(sys,"check12@gmail.com",ELECTRICAL_ENGINEERING)==ESCAPE_TECHNION_SUCCESS);
    ASSERT_TEST(escapeTechnionAddRoom(sys,"check12@gmail.com",1,4,1,0,24,1)==ESCAPE_TECHNION_SUCCESS);
    //Parameters check
    ASSERT_TEST(escapeTechnionRemoveRoom(sys,ELECTRICAL_ENGINEERING,1)==ESCAPE_TECHNION_SUCCESS);
    return true;
}

int main (int argv, char** arc){
    RUN_TEST(testSysCreate);
    RUN_TEST(testSysAddCompany);
    RUN_TEST(testSysRemoveCompany);
    RUN_TEST(testSysAddRoom);
    RUN_TEST(testSysRemoveRoom);
    return 0;
}