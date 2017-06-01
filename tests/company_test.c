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
    Company company = companyCreate("shahar92@campux.technion.ac.il",ELECTRICAL_ENGINEERING);
    Room room = roomCreate(1,4,3,12,19,1);
    companyAddRoom(company,room);
    Company new_company = companyCopy(company);
    ASSERT_TEST(companyCompare(company,new_company)==0);
    TechnionFaculty faculty1,faculty2;
    companyGetFaculty(company,&faculty1);
    companyGetFaculty(new_company,&faculty2);
    ASSERT_TEST(faculty1 == faculty2);
    companyFree(company);
    companyFree(new_company);
    return true;
}

static bool testCompanyCompare(){
    Company company2 = companyCreate("shahar92@campus.technion.ac.il",ELECTRICAL_ENGINEERING);
    Company  company1 = companyCreate("1@2",ELECTRICAL_ENGINEERING);
    ASSERT_TEST(companyCompare(company1,company2) != 0);
    companyFree(company2);
    companyFree(company1);
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