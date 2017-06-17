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
    ASSERT_TEST(escapeTechnionRemoveRoom(NULL,ELECTRICAL_ENGINEERING,1)==ESCAPE_TECHNION_NULL_PARAMETER);
    ASSERT_TEST(escapeTechnionRemoveRoom(sys,-1,1)==ESCAPE_TECHNION_INVALID_PARAMETER);
    ASSERT_TEST(escapeTechnionRemoveRoom(sys,20,1)==ESCAPE_TECHNION_INVALID_PARAMETER);
    ASSERT_TEST(escapeTechnionRemoveRoom(sys,0,0)==ESCAPE_TECHNION_INVALID_PARAMETER);
    //no room exists
    ASSERT_TEST(escapeTechnionRemoveRoom(sys,COMPUTER_SCIENCE,1)==ESCAPE_TECHNION_ID_DOES_NOT_EXIST);
    ASSERT_TEST(escapeTechnionRemoveRoom(sys,ELECTRICAL_ENGINEERING,11)==ESCAPE_TECHNION_ID_DOES_NOT_EXIST);
    //SUCCESS
    ASSERT_TEST(escapeTechnionRemoveRoom(sys,ELECTRICAL_ENGINEERING,1)==ESCAPE_TECHNION_SUCCESS);
    //check if removed room was removed
    ASSERT_TEST(escapeTechnionRemoveRoom(sys,ELECTRICAL_ENGINEERING,1)==ESCAPE_TECHNION_ID_DOES_NOT_EXIST);
    escapeTechnionDestroy(sys);
    return true;
}

static bool testAddClient(){
    EscapeTechnion sys;
    EscapeTechnionResult result;
    ASSERT_TEST(escapeTechnionCreate(&sys)==ESCAPE_TECHNION_SUCCESS);
    ASSERT_TEST(escapeTechnionAddCompany(sys,"check12@gmail.com",ELECTRICAL_ENGINEERING)==ESCAPE_TECHNION_SUCCESS);
    ASSERT_TEST(escapeTechnionAddRoom(sys,"check12@gmail.com",1,4,1,0,24,1)==ESCAPE_TECHNION_SUCCESS);
    //Test success
    ASSERT_TEST(escapeTechnionAddClient(sys,"mynameis@gmail.com",MEDICINE,1)==ESCAPE_TECHNION_SUCCESS);
    ASSERT_TEST(escapeTechnionAddClient(sys,"mtmisabi--ch@die.com",COMPUTER_SCIENCE,1)==ESCAPE_TECHNION_SUCCESS);
    //Test parameters
    ASSERT_TEST(escapeTechnionAddClient(NULL,"RANDOMNAME@gmail.com",MEDICINE,1)==ESCAPE_TECHNION_NULL_PARAMETER);
    ASSERT_TEST(escapeTechnionAddClient(sys,"mynsgmail.com",MEDICINE,1)==ESCAPE_TECHNION_INVALID_PARAMETER);
    ASSERT_TEST(escapeTechnionAddClient(sys,"RANDOMNAME@gmail.com",-1,1)==ESCAPE_TECHNION_INVALID_PARAMETER);
    ASSERT_TEST(escapeTechnionAddClient(sys,"RANDOMNAME@gmail.com",20,1)==ESCAPE_TECHNION_INVALID_PARAMETER);
    ASSERT_TEST(escapeTechnionAddClient(sys,"RANDOMNAME@gmail.com",0,0)==ESCAPE_TECHNION_INVALID_PARAMETER);
    ASSERT_TEST(escapeTechnionAddClient(sys,"RANDOMNAME@gmail.com",0,11)==ESCAPE_TECHNION_INVALID_PARAMETER);
    //Test same mail
    ASSERT_TEST(escapeTechnionAddClient(sys,"mynameis@gmail.com",CHEMISTRY,1)==ESCAPE_TECHNION_EMAIL_ALREADY_EXISTS);
    ASSERT_TEST(escapeTechnionAddClient(sys,"mynameis@gmail.com",CHEMISTRY,1)==ESCAPE_TECHNION_EMAIL_ALREADY_EXISTS);
    ASSERT_TEST(escapeTechnionAddClient(sys,"mtmisabi--ch@die.com",MATHEMATICS,1)==ESCAPE_TECHNION_EMAIL_ALREADY_EXISTS);
    escapeTechnionDestroy(sys);
    return  true;
}

static bool testRemoveClient(){
    EscapeTechnion sys;
    EscapeTechnionResult result;
    ASSERT_TEST(escapeTechnionCreate(&sys)==ESCAPE_TECHNION_SUCCESS);
    ASSERT_TEST(escapeTechnionAddCompany(sys,"check12@gmail.com",ELECTRICAL_ENGINEERING)==ESCAPE_TECHNION_SUCCESS);
    ASSERT_TEST(escapeTechnionAddRoom(sys,"check12@gmail.com",1,4,1,0,24,1)==ESCAPE_TECHNION_SUCCESS);
    ASSERT_TEST(escapeTechnionAddClient(sys,"mynameis@gmail.com",MEDICINE,1)==ESCAPE_TECHNION_SUCCESS);
    ASSERT_TEST(escapeTechnionAddClient(sys,"mtmisabi--ch@die.com",COMPUTER_SCIENCE,1)==ESCAPE_TECHNION_SUCCESS);
    //Null tests
    ASSERT_TEST(escapeTechnionRemoveClient(sys,NULL)==ESCAPE_TECHNION_NULL_PARAMETER);
    ASSERT_TEST(escapeTechnionRemoveClient(NULL,"mtmisabi--ch@die.com")==ESCAPE_TECHNION_NULL_PARAMETER);
    //Test parameters
    ASSERT_TEST(escapeTechnionRemoveClient(sys,"mtmisabi--chdie.com")==ESCAPE_TECHNION_INVALID_PARAMETER);
    //Test no mail
    ASSERT_TEST(escapeTechnionRemoveClient(sys,"mtmisabi@chdie.com")==ESCAPE_TECHNION_CLIENT_EMAIL_DOES_NOT_EXIST);
    //TEST SUCCESS
    ASSERT_TEST(escapeTechnionRemoveClient(sys,"mynameis@gmail.com")==ESCAPE_TECHNION_SUCCESS);
    ASSERT_TEST(escapeTechnionRemoveClient(sys,"mynameis@gmail.com")==ESCAPE_TECHNION_CLIENT_EMAIL_DOES_NOT_EXIST);

    escapeTechnionDestroy(sys);

    return  true;
}

static bool testAddOrder(){
    EscapeTechnion sys;
    EscapeTechnionResult result;
    ASSERT_TEST(escapeTechnionCreate(&sys)==ESCAPE_TECHNION_SUCCESS);
    ASSERT_TEST(escapeTechnionAddCompany(sys,"check12@gmail.com",ELECTRICAL_ENGINEERING)==ESCAPE_TECHNION_SUCCESS);
    ASSERT_TEST(escapeTechnionAddCompany(sys,"mtmCompany2@gmail.com",COMPUTER_SCIENCE)==ESCAPE_TECHNION_SUCCESS);
    ASSERT_TEST(escapeTechnionAddRoom(sys,"check12@gmail.com",1,4,1,0,24,1)==ESCAPE_TECHNION_SUCCESS);
    ASSERT_TEST(escapeTechnionAddRoom(sys,"mtmCompany2@gmail.com",1,4,1,0,24,1)==ESCAPE_TECHNION_SUCCESS);
    ASSERT_TEST(escapeTechnionAddClient(sys,"mynameis@gmail.com",MEDICINE,1)==ESCAPE_TECHNION_SUCCESS);
    ASSERT_TEST(escapeTechnionAddClient(sys,"WHATDOESTHEFOXSAY?@",COMPUTER_SCIENCE,1)==ESCAPE_TECHNION_SUCCESS);
    ASSERT_TEST(escapeTechnionAddClient(sys,"ibeliveicanfly@gmail.com",MATHEMATICS,5)==ESCAPE_TECHNION_SUCCESS);
    //Null tests
    ASSERT_TEST(escapeTechnionAddOrder(sys,NULL,1,1,1,1,1)==ESCAPE_TECHNION_NULL_PARAMETER);
    ASSERT_TEST(escapeTechnionAddOrder(NULL,"WHATDOESTHEFOXSAY?@",1,1,1,1,1)==ESCAPE_TECHNION_NULL_PARAMETER);
    //Test parameters
    ASSERT_TEST(escapeTechnionAddOrder(sys,"WHATDOESTHEFOXSAY?",1,1,1,1,1)==ESCAPE_TECHNION_INVALID_PARAMETER);//MAIL CHECK
    ASSERT_TEST(escapeTechnionAddOrder(sys,"WHATDOESTHEFOXSAY?@",-1,1,1,00,1)==ESCAPE_TECHNION_INVALID_PARAMETER);//FACULTY CHECK
    ASSERT_TEST(escapeTechnionAddOrder(sys,"WHATDOESTHEFOXSAY?@",20,1,1,00,1)==ESCAPE_TECHNION_INVALID_PARAMETER);//FACULTY CHECK
    ASSERT_TEST(escapeTechnionAddOrder(sys,"WHATDOESTHEFOXSAY?@",1,0,1,00,1)==ESCAPE_TECHNION_INVALID_PARAMETER);//ID CHECK
    ASSERT_TEST(escapeTechnionAddOrder(sys,"WHATDOESTHEFOXSAY?@",1,1,-1,00,1)==ESCAPE_TECHNION_INVALID_PARAMETER);//DAY CHECK
    ASSERT_TEST(escapeTechnionAddOrder(sys,"WHATDOESTHEFOXSAY?@",1,1,0,-1,1)==ESCAPE_TECHNION_INVALID_PARAMETER);//HOUR CHECK
    ASSERT_TEST(escapeTechnionAddOrder(sys,"WHATDOESTHEFOXSAY?@",1,1,0,25,1)==ESCAPE_TECHNION_INVALID_PARAMETER);//HOUR CHECK
    ASSERT_TEST(escapeTechnionAddOrder(sys,"WHATDOESTHEFOXSAY?@",1,1,0,12,0)==ESCAPE_TECHNION_INVALID_PARAMETER);//ppl CHECK
    //Client does not exist
    ASSERT_TEST(escapeTechnionAddOrder(sys,"IkillYOU!@gmail.com",1,1,1,1,1)==ESCAPE_TECHNION_CLIENT_EMAIL_DOES_NOT_EXIST);
    ASSERT_TEST(escapeTechnionAddOrder(sys,"mynamei@gmail.com",1,1,1,1,1)==ESCAPE_TECHNION_CLIENT_EMAIL_DOES_NOT_EXIST);
    //Test ROOM id does not exist
    ASSERT_TEST(escapeTechnionAddOrder(sys,"mynameis@gmail.com",ELECTRICAL_ENGINEERING,2,2,2,2)==ESCAPE_TECHNION_ID_DOES_NOT_EXIST);
    //Test success
    ASSERT_TEST(escapeTechnionAddOrder(sys,"mynameis@gmail.com",ELECTRICAL_ENGINEERING,1,0,12,2)==ESCAPE_TECHNION_SUCCESS);
    ASSERT_TEST(escapeTechnionAddOrder(sys,"WHATDOESTHEFOXSAY?@",COMPUTER_SCIENCE,1,0,12,2)==ESCAPE_TECHNION_SUCCESS);
    //TEST client in room
    ASSERT_TEST(escapeTechnionAddOrder(sys,"WHATDOESTHEFOXSAY?@",ELECTRICAL_ENGINEERING,1,0,12,2)==ESCAPE_TECHNION_CLIENT_IN_ROOM);
    //test room not available
    ASSERT_TEST(escapeTechnionAddOrder(sys,"ibeliveicanfly@gmail.com",ELECTRICAL_ENGINEERING,1,0,12,2)==ESCAPE_TECHNION_ROOM_NOT_AVAILABLE);
    escapeTechnionDestroy(sys);

    return  true;
}

static bool testRecommendedOrder(){
    EscapeTechnion sys;
    EscapeTechnionResult result;
    ASSERT_TEST(escapeTechnionCreate(&sys)==ESCAPE_TECHNION_SUCCESS);
    ASSERT_TEST(escapeTechnionAddCompany(sys,"check12@gmail.com",ELECTRICAL_ENGINEERING)==ESCAPE_TECHNION_SUCCESS);
    ASSERT_TEST(escapeTechnionAddRoom(sys,"check12@gmail.com",7,4,4,0,24,5));
    ASSERT_TEST(escapeTechnionAddClient(sys,"@",MEDICINE,5)==ESCAPE_TECHNION_SUCCESS);
    ASSERT_TEST(escapeTechnionAddClient(sys,"1@",MEDICINE,5)==ESCAPE_TECHNION_SUCCESS);
    ASSERT_TEST(escapeTechnionRecommendedRoomOrder(sys,"@",4)==ESCAPE_TECHNION_SUCCESS);
    ASSERT_TEST(escapeTechnionAddOrder(sys,"1@",ELECTRICAL_ENGINEERING,7,0,0,4)==ESCAPE_TECHNION_ROOM_NOT_AVAILABLE);
    //
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