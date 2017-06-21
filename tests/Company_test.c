#include "list_mtm/test_utilities.h"
#include "../Company.h"
#include <string.h>
#include "stdlib.h"

static bool testCompanyCreate() {
    Company company;

    ASSERT_TEST(companyCreate("@campux.technion.ac.il",-1,&company)
                ==COMPANY_INVALID_PARAMETER);
    ASSERT_TEST(companyCreate("campux.technion.ac.il",CHEMICAL_ENGINEERING,&company)
                ==COMPANY_INVALID_PARAMETER);
    ASSERT_TEST(companyCreate("@",CHEMICAL_ENGINEERING,&company)
                ==COMPANY_SUCCESS);
    companyDestroy(company);
    return true;
}
static bool testCompanyCopy() {
    Company company;
    Room room;
    companyCreate("shahar92@campux.technion.ac.il",ELECTRICAL_ENGINEERING,&company);
    roomCreate(1,PRICE_MODULE,MIN_PLAYERS_FOR_ROOM,MIN_HOUR,MAX_HOUR,MIN_DIFFICULTY_LEVEL,&room);
    companyAddRoom(company,room);
    Company new_company = companyCopy(company);
    ASSERT_TEST(companyCompare(company,new_company)==0);
    companyDestroy(company);
    companyDestroy(new_company);
    return true;
}

static bool testCompanyCompare(){
    Company company1,company2;
    companyCreate("shahar92@campus.technion.ac.il",ELECTRICAL_ENGINEERING,&company1);
    companyCreate("1@2",ELECTRICAL_ENGINEERING,&company2);
    ASSERT_TEST(companyCompare(company1,company2) != 0);
    companyDestroy(company2);
    companyDestroy(company1);
    return true;
}

static bool testCompanyAddRoom(){
    Company company;
    companyCreate("shahar92@campux.technion.ac.il",ELECTRICAL_ENGINEERING,&company);
    Room room;
    roomCreate(1,PRICE_MODULE,MIN_PLAYERS_FOR_ROOM,MIN_HOUR,MAX_HOUR,MIN_DIFFICULTY_LEVEL,&room);
    companyAddRoom(company,room);
    ASSERT_TEST(companyFindRoom(company,1,&room)==COMPANY_SUCCESS);
    ASSERT_TEST(companyFindRoom(company,2,&room)!=COMPANY_SUCCESS);
    companyDestroy(company);

    return  true;
}

static bool testCompanyRemoveRoom(){
    Company company;
    Room room;
    companyCreate("shahar92@campux.technion.ac.il",ELECTRICAL_ENGINEERING,&company);
    roomCreate(1,PRICE_MODULE,MIN_PLAYERS_FOR_ROOM,MIN_HOUR,MAX_HOUR,MIN_DIFFICULTY_LEVEL,&room);
    companyAddRoom(company,room);
    ASSERT_TEST(companyFindRoom(company,1,&room)==COMPANY_SUCCESS);
    companyRemoveRoom(company,room);
    ASSERT_TEST(companyFindRoom(company,1,&room)!=COMPANY_SUCCESS);
    companyDestroy(company);

    return  true;
}

static bool testCompanyGetFaculty(){
    Company company;
    companyCreate("shahar92@campux.technion.ac.il",ELECTRICAL_ENGINEERING,&company);
    Room room;
    roomCreate(1,PRICE_MODULE,MIN_PLAYERS_FOR_ROOM,MIN_HOUR,MAX_HOUR,MIN_DIFFICULTY_LEVEL,&room);
    companyAddRoom(company,room);
    TechnionFaculty faculty;
    companyDestroy(company);
    return true;
}

static bool testCompanyGetEmail(){
    Company company;
    companyCreate("shahar92@campux.technion.ac.il",ELECTRICAL_ENGINEERING,&company);
    char* mail;
    ASSERT_TEST(companyGetEmail(company,&mail)==COMPANY_SUCCESS);
    ASSERT_TEST(strcmp(mail,"shahar92@campux.technion.ac.il")==0);
    companyDestroy(company);
    free(mail);
    return true;
}

static bool testcompanyFindRoom(){
    Company company;
    companyCreate("shahar92@campux.technion.ac.il",ELECTRICAL_ENGINEERING,&company);
    Room room;
    roomCreate(1,PRICE_MODULE,MIN_PLAYERS_FOR_ROOM,MIN_HOUR,MAX_HOUR,MIN_DIFFICULTY_LEVEL,&room);
    companyAddRoom(company,room);
    ASSERT_TEST(companyFindRoom(company,1,&room)==COMPANY_SUCCESS);
    ASSERT_TEST(companyFindRoom(company,2,&room)==COMPANY_ROOM_DOES_NOT_EXIST);
    companyDestroy(company);
    return  true;
}
int main (int argv, char** arc){
    RUN_TEST(testCompanyCreate);
    RUN_TEST(testCompanyCopy);
    RUN_TEST(testCompanyCompare);
    RUN_TEST(testCompanyAddRoom);
    RUN_TEST(testCompanyRemoveRoom);
    RUN_TEST(testCompanyGetFaculty);
    RUN_TEST(testCompanyGetEmail);
    RUN_TEST(testcompanyFindRoom);//
    //return 0;
}