#include "list_mtm/test_utilities.h"
#include "../Company.h"
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

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
    return true;
}

static bool testCompanyCopy() {
    Company company;
    companyCreate("shahar92@campux.technion.ac.il",ELECTRICAL_ENGINEERING,&company);
    Room room;
    roomCreate(1,4,3,12,19,1,&room);
    companyAddRoom(company,room);
    Company new_company = companyCopy(company);
    ASSERT_TEST(companyCompare(company,new_company)==0);
    TechnionFaculty faculty1 = companyGetFaculty(company),faculty2 = companyGetFaculty(new_company);
    ASSERT_TEST(faculty1 == faculty2);
    companyDestroy(company);
    companyDestroy(new_company);
    roomDestroy(room);
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

int main (int argv, char** arc){
    RUN_TEST(testCompanyCreate);
    RUN_TEST(testCompanyCreate);
    RUN_TEST(testCompanyCopy);
    //RUN_TEST(testCompanyCompare);
    //RUN_TEST(testCompanyAddRoom);
    //RUN_TEST(testCompanyRemoveRoom);
    //RUN_TEST(testCompanyGetFaculty);
    //RUN_TEST(testCompanyGetEmail);
    //RUN_TEST(testCompanyFindRoom);
    //return 0;
}