#include "list_mtm/test_utilities.h"
#include "../Company.h"
#include <string.h>
#include "stdlib.h"

/*tatic ListElement copyString(ListElement str){
    assert(str);
    char* copy = malloc(strlen(str)+1);
    return copy != NULL ? strcpy(copy, str) : NULL;
}

static void freeString(ListElement str){
    free(str);
}

static bool isLongerThan(ListElement element,ListFilterKey number) {
    char* string = element;
    return strlen(string) > *(int*)number;
}
*/
static bool testCompanyCreate() {
    Company company;
    ASSERT_TEST(companyCreate("shahar92@campux.technion.ac.il",ELECTRICAL_ENGINEERING,NULL)
                ==COMPANY_NULL_ARGUMENT);
    ASSERT_TEST(companyCreate(NULL,ELECTRICAL_ENGINEERING,&company)
                ==COMPANY_NULL_ARGUMENT);
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
    companyCreate("shahar92@campux.technion.ac.il",ELECTRICAL_ENGINEERING,&company);
    companyAddRoom(company,1,PRICE_MOUDLE,MIN_PLAYERS_FOR_ROOM,MIN_HOUR,MAX_HOUR,
                   MIN_DIFFICULTY_LEVEL);
    Company new_company = companyCopy(company);
    ASSERT_TEST(companyCompare(company,new_company)==0);
    TechnionFaculty faculty1 = companyGetFaculty(company,&faculty1),
            faculty2 = companyGetFaculty(new_company,&faculty2);
    ASSERT_TEST(faculty1 == faculty2);
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
    companyAddRoom(company,1,PRICE_MOUDLE,MIN_PLAYERS_FOR_ROOM,MIN_HOUR,MAX_HOUR,
                   MIN_DIFFICULTY_LEVEL);
    Room room;
    ASSERT_TEST(companyFindRoom(company,1,&room)==COMPANY_SUCCESS);
    ASSERT_TEST(companyFindRoom(company,2,&room)!=COMPANY_SUCCESS);
    companyDestroy(company);

    return  true;
}

static bool testCompanyRemoveRoom(){
    Company company;
    companyCreate("shahar92@campux.technion.ac.il",ELECTRICAL_ENGINEERING,&company);
    companyAddRoom(company,1,PRICE_MOUDLE,MIN_PLAYERS_FOR_ROOM,MIN_HOUR,MAX_HOUR,
                   MIN_DIFFICULTY_LEVEL);
    Room room;
    ASSERT_TEST(companyFindRoom(company,1,&room)==COMPANY_SUCCESS);
    companyRemoveRoom(company,1);
    ASSERT_TEST(companyFindRoom(company,1,&room)!=COMPANY_SUCCESS);
    companyDestroy(company);

    return  true;
}

static bool testCompanyGetFaculty(){
    Company company;
    companyCreate("shahar92@campux.technion.ac.il",ELECTRICAL_ENGINEERING,&company);
    companyAddRoom(company,1,PRICE_MOUDLE,MIN_PLAYERS_FOR_ROOM,MIN_HOUR,MAX_HOUR,
                   MIN_DIFFICULTY_LEVEL);
    TechnionFaculty faculty;
    ASSERT_TEST(companyGetFaculty(company,&faculty)==COMPANY_SUCCESS);
    ASSERT_TEST(faculty==ELECTRICAL_ENGINEERING);
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
    companyAddRoom(company,1,PRICE_MOUDLE,MIN_PLAYERS_FOR_ROOM,MIN_HOUR,MAX_HOUR,
                   MIN_DIFFICULTY_LEVEL);
    Room room;
    ASSERT_TEST(companyFindRoom(company,1,&room)==COMPANY_SUCCESS);
    ASSERT_TEST(companyFindRoom(company,2,&room)==COMPANY_ROOM_DOES_NOT_EXIST);
    ASSERT_TEST(companyFindRoom(NULL,1,&room)==COMPANY_NULL_ARGUMENT);
    ASSERT_TEST(companyFindRoom(company,1,NULL)==COMPANY_NULL_ARGUMENT);
    ASSERT_TEST(companyFindRoom(company,0,&room)==COMPANY_INVALID_PARAMETER);
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