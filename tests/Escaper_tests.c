//
#include "./test_utilities.h"
#include "../Escaper.h"
#include <string.h>

static bool testescaperCreate() {
    Escaper escaper;//
    //skill_level checks
    ASSERT_TEST(escaperCreate("am@",FACULTY_NUM-1,MIN_SKILL_LEVEL-1,&escaper) ==
                        ESCAPER_INVALID_PARAMETER);
    ASSERT_TEST(escaperCreate("am@",FACULTY_NUM-1,MAX_SKILL_LEVEL+1,&escaper) ==
                        ESCAPER_INVALID_PARAMETER);
    ASSERT_TEST(escaperCreate("am@",FACULTY_NUM-1,MAX_SKILL_LEVEL,&escaper) ==
                ESCAPER_SUCCESS);
    escaperDestroy(escaper);
    ASSERT_TEST(escaperCreate("am@",FACULTY_NUM-1,MIN_SKILL_LEVEL,&escaper) ==
                ESCAPER_SUCCESS);
    escaperDestroy(escaper);
    //faculty checks
    ASSERT_TEST(escaperCreate("am@",0-1,MIN_SKILL_LEVEL,&escaper) ==
                        ESCAPER_INVALID_PARAMETER);
    ASSERT_TEST(escaperCreate("am@",FACULTY_NUM+1,MIN_SKILL_LEVEL,&escaper)==
                        ESCAPER_INVALID_PARAMETER);
    ASSERT_TEST(escaperCreate("am@",FACULTY_NUM-1,MIN_SKILL_LEVEL,&escaper)==
                        ESCAPER_SUCCESS);
    escaperDestroy(escaper);
    //mail checks
    ASSERT_TEST(escaperCreate("am",FACULTY_NUM-1,MIN_SKILL_LEVEL,&escaper) ==
                        ESCAPER_INVALID_PARAMETER);
    ASSERT_TEST(escaperCreate("am@@",FACULTY_NUM-1,MIN_SKILL_LEVEL,&escaper) ==
                ESCAPER_INVALID_PARAMETER);
    ASSERT_TEST(escaperCreate("am@",FACULTY_NUM-1,MIN_SKILL_LEVEL,&escaper) ==
                ESCAPER_SUCCESS);
    escaperDestroy(escaper);
    return true;
}

static bool testescaperCopy() {
    Escaper escaper;
    escaperCreate("check12@gmail.com",FACULTY_NUM-1,MIN_SKILL_LEVEL,&escaper);
    Escaper escaper_cpy = escaperCopy(escaper);
    ASSERT_TEST(escaperCompare(escaper,escaper_cpy) == 0);
    escaperDestroy(escaper);
    escaperDestroy(escaper_cpy);
    return true;
}

static bool testescaperGetMail() {
    Escaper escaper;
    char* mail_cpy;
    escaperCreate("check12@gmail.com",FACULTY_NUM-1,MIN_SKILL_LEVEL,&escaper);
    ASSERT_TEST(escaperGetEmail(escaper,&mail_cpy) == ESCAPER_SUCCESS);
    ASSERT_TEST(strcmp(mail_cpy,"check12@gmail.com") == 0);
    escaperDestroy(escaper);
    return true;
}

static bool testescaperGetFaculty() {
    Escaper escaper;
    //logic tests
    escaperCreate("check12@gmail.com",FACULTY_NUM-1,MIN_SKILL_LEVEL,&escaper);
    ASSERT_TEST(escaperGetFaculty(escaper) == FACULTY_NUM-1);
    escaperDestroy(escaper);
    return true;
}

static  bool testEscaperGetSkillLevel(){
    Escaper escaper;
    escaperCreate("check12@gmail.com",FACULTY_NUM-1,MIN_SKILL_LEVEL,&escaper);
    int skill_level = escaperGetSkillLevel(escaper);
    ASSERT_TEST((skill_level>=MIN_SKILL_LEVEL)&&(skill_level<=MAX_SKILL_LEVEL));
    escaperDestroy(escaper);
    return true;
}

int main (int argv, char** arc){
    RUN_TEST(testescaperCreate);
    RUN_TEST(testescaperCopy);
    RUN_TEST(testescaperGetMail);
    RUN_TEST(testescaperGetFaculty);
    RUN_TEST(testEscaperGetSkillLevel);
    return 0;
}

