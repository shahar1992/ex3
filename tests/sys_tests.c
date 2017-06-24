
#include "./test_utilities.h"
#include "../EscapeTechnion.h"
#include <string.h>

static bool testSysCreate(){
    EscapeTechnion sys;
    ASSERT_TEST(escapeTechnionCreate(&sys) == ESCAPE_TECHNION_SUCCESS);
    escapeTechnionDestroy(sys);
    return true;
}

static bool testSysAddCompany(){
    EscapeTechnion sys;
    ASSERT_TEST(escapeTechnionCreate(&sys)==ESCAPE_TECHNION_SUCCESS);
    ASSERT_TEST(escapeTechnionAddCompany(
            sys,"check12@gmail.com", ELECTRICAL_ENGINEERING) ==
                        ESCAPE_TECHNION_SUCCESS);
    ASSERT_TEST(escapeTechnionAddCompany(
            sys,"check12@gmail.com",ELECTRICAL_ENGINEERING) ==
                        ESCAPE_TECHNION_EMAIL_ALREADY_EXISTS);
    ASSERT_TEST(escapeTechnionAddCompany(
            sys,"check12gmail.com",ELECTRICAL_ENGINEERING) ==
                        ESCAPE_TECHNION_INVALID_PARAMETER);
    ASSERT_TEST(escapeTechnionAddCompany(
            sys,"check12@gmail.com",-1) == ESCAPE_TECHNION_INVALID_PARAMETER);
    ASSERT_TEST(escapeTechnionAddCompany(
            sys,"check12@gmail.com",20) == ESCAPE_TECHNION_INVALID_PARAMETER);
    ASSERT_TEST(escapeTechnionAddCompany(
            sys,"@",ELECTRICAL_ENGINEERING) == ESCAPE_TECHNION_SUCCESS);
    ASSERT_TEST(escapeTechnionAddCompany(
            sys,"check12@gmail.com",COMPUTER_SCIENCE) ==
                        ESCAPE_TECHNION_EMAIL_ALREADY_EXISTS);
    ASSERT_TEST(escapeTechnionAddClient(
            sys,"@",MATHEMATICS,1) == ESCAPE_TECHNION_EMAIL_ALREADY_EXISTS);
    escapeTechnionDestroy(sys);
    return true;
}

static bool testSysRemoveCompany(){
    EscapeTechnion sys;
    EscapeTechnionResult result;
    ASSERT_TEST(escapeTechnionCreate(&sys) == ESCAPE_TECHNION_SUCCESS);
    ASSERT_TEST(escapeTechnionAddCompany(
            sys,"eecompany1@",ELECTRICAL_ENGINEERING) ==
                        ESCAPE_TECHNION_SUCCESS);
    ASSERT_TEST((result=escapeTechnionRemoveCompany(
            sys,"check1@gmail.com")) ==
                        ESCAPE_TECHNION_COMPANY_EMAIL_DOES_NOT_EXIST);
    ASSERT_TEST((result=escapeTechnionRemoveCompany(
            sys,"1"))==ESCAPE_TECHNION_INVALID_PARAMETER);
    ASSERT_TEST((result=escapeTechnionRemoveCompany(
            sys,"1@"))==ESCAPE_TECHNION_COMPANY_EMAIL_DOES_NOT_EXIST);
    ASSERT_TEST(escapeTechnionAddClient(
            sys,"EEescaper1@",ELECTRICAL_ENGINEERING,1) ==
                        ESCAPE_TECHNION_SUCCESS);
    ASSERT_TEST(escapeTechnionAddRoom(
            sys,"eecompany1@",1,PRICE_MODULE,1,0,24,5) ==
                        ESCAPE_TECHNION_SUCCESS);
    ASSERT_TEST(escapeTechnionAddOrder(
            sys,"EEescaper1@",ELECTRICAL_ENGINEERING,1,0,12,5) ==
                        ESCAPE_TECHNION_SUCCESS);
    ASSERT_TEST((result=escapeTechnionRemoveCompany(
            sys,"eecompany1@")) == ESCAPE_TECHNION_RESERVATION_EXISTS);
    ASSERT_TEST((result=escapeTechnionRemoveClient(
            sys,"EEescaper1@")) == ESCAPE_TECHNION_SUCCESS);
    ASSERT_TEST((result=escapeTechnionRemoveCompany(
            sys,"eecompany1@")) == ESCAPE_TECHNION_SUCCESS);
    escapeTechnionDestroy(sys);
    return true;
}

static bool testSysAddRoom(){
    EscapeTechnion sys;
    EscapeTechnionResult result;
    ASSERT_TEST(escapeTechnionCreate(&sys)==ESCAPE_TECHNION_SUCCESS);
    ASSERT_TEST(escapeTechnionAddCompany(
            sys,"eecompany1@",ELECTRICAL_ENGINEERING) ==
                        ESCAPE_TECHNION_SUCCESS);
    ASSERT_TEST(escapeTechnionAddRoom(
            sys,"eecompany1@",1,PRICE_MODULE,4,0,24,1) ==
                        ESCAPE_TECHNION_SUCCESS);
    ASSERT_TEST((result=escapeTechnionAddRoom(
            sys,"eecompany1",1,4,1,0,24,1)) ==
                        ESCAPE_TECHNION_INVALID_PARAMETER);
    ASSERT_TEST(escapeTechnionAddRoom(
            sys,"eecompany1@",-1,4,1,0,24,1) ==
                        ESCAPE_TECHNION_INVALID_PARAMETER);
    ASSERT_TEST(escapeTechnionAddRoom(
            sys,"eecompany1@",3,3,1,0,24,1) ==
                        ESCAPE_TECHNION_INVALID_PARAMETER);
    ASSERT_TEST(escapeTechnionAddRoom(
            sys,"eecompany1@",3,3,1,0,25,1) ==
                        ESCAPE_TECHNION_INVALID_PARAMETER);
    ASSERT_TEST(escapeTechnionAddRoom(
            sys,"eecompany1@",3,3,1,0,24,10) ==
                        ESCAPE_TECHNION_INVALID_PARAMETER);
    ASSERT_TEST(escapeTechnionAddRoom(
            sys,"eecompany1@",3,3,1,0,24,-1) ==
                        ESCAPE_TECHNION_INVALID_PARAMETER);
    ASSERT_TEST(escapeTechnionAddRoom(
            sys,"eecompany1@",3,3,1,-1,24,1) ==
                        ESCAPE_TECHNION_INVALID_PARAMETER);
    ASSERT_TEST(result=escapeTechnionAddRoom(
            sys,"check12@gmail.com",-1,4,1,0,24,1) ==
            ESCAPE_TECHNION_INVALID_PARAMETER);
    ASSERT_TEST(result=escapeTechnionAddRoom(
            sys,"check12@gmail.com",1,4,1,0,24,1) ==
            ESCAPE_TECHNION_COMPANY_EMAIL_DOES_NOT_EXIST);
    ASSERT_TEST(escapeTechnionAddRoom(
            sys,"eecompany1@",1,PRICE_MODULE*2,1,0,24,1) ==
                        ESCAPE_TECHNION_ID_ALREADY_EXIST);
    escapeTechnionDestroy(sys);
    return true;
}

static bool testSysRemoveRoom(){
    EscapeTechnion sys;
    ASSERT_TEST(escapeTechnionCreate(&sys)==ESCAPE_TECHNION_SUCCESS);
    ASSERT_TEST(escapeTechnionAddCompany(
            sys,"eecompany1@",ELECTRICAL_ENGINEERING) ==
                        ESCAPE_TECHNION_SUCCESS);
    ASSERT_TEST(escapeTechnionAddRoom(
            sys,"eecompany1@",1,4,1,0,24,1) == ESCAPE_TECHNION_SUCCESS);
    ASSERT_TEST(escapeTechnionRemoveRoom(sys,-1,1) ==
                        ESCAPE_TECHNION_INVALID_PARAMETER);
    ASSERT_TEST(escapeTechnionRemoveRoom(sys,20,1) ==
                        ESCAPE_TECHNION_INVALID_PARAMETER);
    ASSERT_TEST(escapeTechnionRemoveRoom(sys,0,0) ==
                        ESCAPE_TECHNION_INVALID_PARAMETER);
    ASSERT_TEST(escapeTechnionRemoveRoom(
            sys,COMPUTER_SCIENCE,1) == ESCAPE_TECHNION_ID_DOES_NOT_EXIST);
    ASSERT_TEST(escapeTechnionRemoveRoom(
            sys,ELECTRICAL_ENGINEERING,11) ==
                        ESCAPE_TECHNION_ID_DOES_NOT_EXIST);
    //MTM resevation exists
    ASSERT_TEST(escapeTechnionAddClient(
            sys,"escaper1@",ELECTRICAL_ENGINEERING,2) ==
                        ESCAPE_TECHNION_SUCCESS);
    ASSERT_TEST(escapeTechnionAddOrder(
            sys,"escaper1@",ELECTRICAL_ENGINEERING,1,0,12,3) ==
                        ESCAPE_TECHNION_SUCCESS);
    ASSERT_TEST(escapeTechnionRemoveRoom(
            sys,ELECTRICAL_ENGINEERING,1) ==
                        ESCAPE_TECHNION_RESERVATION_EXISTS);
    //SUCCESS
    escapeTechnionRemoveClient(sys,"escaper1@");
    ASSERT_TEST(escapeTechnionRemoveRoom(
            sys,ELECTRICAL_ENGINEERING,1) == ESCAPE_TECHNION_SUCCESS);
    //check if removed room was removed
    ASSERT_TEST(escapeTechnionRemoveRoom(
            sys,ELECTRICAL_ENGINEERING,1) == ESCAPE_TECHNION_ID_DOES_NOT_EXIST);
    escapeTechnionDestroy(sys);
    return true;
}

static bool testAddClient(){
    EscapeTechnion sys;
    ASSERT_TEST(escapeTechnionCreate(&sys) == ESCAPE_TECHNION_SUCCESS);
    //Test success
    ASSERT_TEST(escapeTechnionAddClient(
            sys,"mynameis@gmail.com",MEDICINE,1) == ESCAPE_TECHNION_SUCCESS);
    ASSERT_TEST(escapeTechnionAddClient(
            sys,"mtmisabi--ch@die.com",COMPUTER_SCIENCE,1) ==
                        ESCAPE_TECHNION_SUCCESS);
    ASSERT_TEST(escapeTechnionAddClient(
            sys,"mynsgmail.com",MEDICINE,1) ==
                        ESCAPE_TECHNION_INVALID_PARAMETER);
    ASSERT_TEST(escapeTechnionAddClient(
            sys,"mynameis@gmail.com",-1,1) ==
                        ESCAPE_TECHNION_INVALID_PARAMETER);
    ASSERT_TEST(escapeTechnionAddClient(
            sys,"mynameis@gmail.com",20,1) ==
                        ESCAPE_TECHNION_INVALID_PARAMETER);
    ASSERT_TEST(escapeTechnionAddClient(
            sys,"mynameis@gmail.com",0,0) == ESCAPE_TECHNION_INVALID_PARAMETER);
    ASSERT_TEST(escapeTechnionAddClient(
            sys,"mynameis@gmail.com",0,11) ==
                        ESCAPE_TECHNION_INVALID_PARAMETER);
    //Test same mail
    ASSERT_TEST(escapeTechnionAddClient(
            sys,"mynameis@gmail.com",CHEMISTRY,1) ==
                        ESCAPE_TECHNION_EMAIL_ALREADY_EXISTS);
    ASSERT_TEST(escapeTechnionAddClient(
            sys,"mynameis@gmail.com",CHEMISTRY,1) ==
                        ESCAPE_TECHNION_EMAIL_ALREADY_EXISTS);
    ASSERT_TEST(escapeTechnionAddClient(
            sys,"mtmisabi--ch@die.com",MATHEMATICS,1) ==
                        ESCAPE_TECHNION_EMAIL_ALREADY_EXISTS);
    escapeTechnionDestroy(sys);
    return  true;
}

static bool testRemoveClient(){
    EscapeTechnion sys;
    ASSERT_TEST(escapeTechnionCreate(&sys) == ESCAPE_TECHNION_SUCCESS);
    //make clients
    ASSERT_TEST(escapeTechnionAddClient(
            sys,"mynameis@gmail.com",MEDICINE,1) == ESCAPE_TECHNION_SUCCESS);
    ASSERT_TEST(escapeTechnionAddClient(
            sys,"mtmisabi--ch@die.com",COMPUTER_SCIENCE,1) ==
                        ESCAPE_TECHNION_SUCCESS);
    //Test parameters
    ASSERT_TEST(escapeTechnionRemoveClient(
            sys,"mtmisabi--chdie.com") == ESCAPE_TECHNION_INVALID_PARAMETER);
    //Test no mail
    ASSERT_TEST(escapeTechnionRemoveClient(
            sys,"mtmisabi@chdie.com") ==
                        ESCAPE_TECHNION_CLIENT_EMAIL_DOES_NOT_EXIST);
    //TEST SUCCESS
    ASSERT_TEST(escapeTechnionRemoveClient(
            sys,"mynameis@gmail.com") == ESCAPE_TECHNION_SUCCESS);
    ASSERT_TEST(escapeTechnionRemoveClient(
            sys,"mynameis@gmail.com") ==
                        ESCAPE_TECHNION_CLIENT_EMAIL_DOES_NOT_EXIST);
    escapeTechnionDestroy(sys);
    return  true;
}

static bool testAddOrder(){
    EscapeTechnion sys;
    ASSERT_TEST(escapeTechnionCreate(&sys) == ESCAPE_TECHNION_SUCCESS);
    ASSERT_TEST(escapeTechnionAddCompany(
            sys,"eecompany1@",ELECTRICAL_ENGINEERING) ==
                        ESCAPE_TECHNION_SUCCESS);
    ASSERT_TEST(escapeTechnionAddRoom(
            sys,"eecompany1@",1,4,1,0,24,1) == ESCAPE_TECHNION_SUCCESS);
    ASSERT_TEST(escapeTechnionAddClient(
            sys,"escaper1@",MEDICINE,1) == ESCAPE_TECHNION_SUCCESS);
    //Null tests
    //Test parameters
    ASSERT_TEST(escapeTechnionAddOrder(
            sys,"WHATDOESTHEFOXSAY?",1,1,1,1,1) ==
                        ESCAPE_TECHNION_INVALID_PARAMETER);//MAIL CHECK
    ASSERT_TEST(escapeTechnionAddOrder(
            sys,"escaper1@",-1,1,1,00,1) ==
                        ESCAPE_TECHNION_INVALID_PARAMETER);//FACULTY CHECK
    ASSERT_TEST(escapeTechnionAddOrder(
            sys,"escaper1@",20,1,1,00,1) ==
                        ESCAPE_TECHNION_INVALID_PARAMETER);//FACULTY CHECK
    ASSERT_TEST(escapeTechnionAddOrder(
            sys,"escaper1@",1,0,1,00,1) ==
                        ESCAPE_TECHNION_INVALID_PARAMETER);//ID CHECK
    ASSERT_TEST(escapeTechnionAddOrder(
            sys,"escaper1@",1,1,-1,00,1) ==
                        ESCAPE_TECHNION_INVALID_PARAMETER);//DAY CHECK
    ASSERT_TEST(escapeTechnionAddOrder(
            sys,"escaper1@",1,1,0,-1,1) ==
                        ESCAPE_TECHNION_INVALID_PARAMETER);//HOUR CHECK
    ASSERT_TEST(escapeTechnionAddOrder(
            sys,"escaper1@",1,1,0,25,1) ==
                        ESCAPE_TECHNION_INVALID_PARAMETER);//HOUR CHECK
    ASSERT_TEST(escapeTechnionAddOrder(
            sys,"escaper1@",1,1,0,12,0) ==
                        ESCAPE_TECHNION_INVALID_PARAMETER);//ppl CHECK
    //Client does not exist
    ASSERT_TEST(escapeTechnionAddOrder(
            sys,"IkillYOU!@gmail.com",ELECTRICAL_ENGINEERING,1,0,12,3) ==
                        ESCAPE_TECHNION_CLIENT_EMAIL_DOES_NOT_EXIST);
    //Test ROOM id does not exist
    ASSERT_TEST(escapeTechnionAddOrder(
            sys,"escaper1@",ELECTRICAL_ENGINEERING,11,0,12,3) ==
                        ESCAPE_TECHNION_ID_DOES_NOT_EXIST);
    //TEST CLIENT IN ROOM
    escapeTechnionAddRoom(sys,"eecompany1@",2,PRICE_MODULE,3,0,21,5);
    ASSERT_TEST(escapeTechnionAddOrder(
            sys,"escaper1@",ELECTRICAL_ENGINEERING,1,0,0,1) ==
                        ESCAPE_TECHNION_SUCCESS);//ESCAPER 1 FOR ROOM 1 TODAY AT 0
    ASSERT_TEST(escapeTechnionAddOrder(
            sys,"escaper1@",ELECTRICAL_ENGINEERING,2,0,0,1) ==
                        ESCAPE_TECHNION_CLIENT_IN_ROOM);
    //room unavailable
    escapeTechnionAddClient(sys,"escaper2@",ELECTRICAL_ENGINEERING,2);
    ASSERT_TEST(escapeTechnionAddOrder(
            sys,"escaper2@",ELECTRICAL_ENGINEERING,1,0,0,7) ==
                        ESCAPE_TECHNION_ROOM_NOT_AVAILABLE);
    escapeTechnionRemoveClient(sys,"escaper1@");
    ASSERT_TEST(escapeTechnionAddOrder(
            sys,"escaper1@",ELECTRICAL_ENGINEERING,1,0,0,7) ==
                        ESCAPE_TECHNION_CLIENT_EMAIL_DOES_NOT_EXIST);
    ASSERT_TEST(escapeTechnionAddOrder(
            sys,"escaper2@",ELECTRICAL_ENGINEERING,1,0,0,7) ==
                        ESCAPE_TECHNION_SUCCESS);
    escapeTechnionDestroy(sys);
    return  true;
}

static bool testRecommendedOrder(){
    EscapeTechnion sys;
    ASSERT_TEST(escapeTechnionCreate(&sys) == ESCAPE_TECHNION_SUCCESS);
    ASSERT_TEST(escapeTechnionAddCompany(
            sys,"check12@gmail.com",ELECTRICAL_ENGINEERING) ==
                        ESCAPE_TECHNION_SUCCESS);
    ASSERT_TEST(escapeTechnionAddRoom(
            sys,"check12@gmail.com",7,4,4,0,24,5));
    ASSERT_TEST(escapeTechnionAddClient(
            sys,"@",MEDICINE,5) == ESCAPE_TECHNION_SUCCESS);
    ASSERT_TEST(escapeTechnionAddClient(
            sys,"1@",MEDICINE,5) == ESCAPE_TECHNION_SUCCESS);
    ASSERT_TEST(escapeTechnionRecommendedRoomOrder(
            sys,"@",4) == ESCAPE_TECHNION_SUCCESS);
    ASSERT_TEST(escapeTechnionAddOrder(
            sys,"1@",ELECTRICAL_ENGINEERING,7,0,0,4) ==
                        ESCAPE_TECHNION_ROOM_NOT_AVAILABLE);
    return true;
}

int main (int argv, char** arc){
    RUN_TEST(testSysCreate);
    RUN_TEST(testSysAddCompany);
    RUN_TEST(testSysRemoveCompany);
    RUN_TEST(testSysAddRoom);
    RUN_TEST(testSysRemoveRoom);
    RUN_TEST(testAddClient);
    RUN_TEST(testRemoveClient);
    RUN_TEST(testAddOrder);
    RUN_TEST(testRecommendedOrder);
    return 0;
}