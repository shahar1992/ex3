#include "tests/list_mtm/test_utilities.h"
#include "list_mtm1/list_mtm1.h"
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

static ListElement copyString(ListElement str){
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

static int compareStrings(ListElement element1, ListElement element2){
    return strcmp((char*)element1,(char*)element2);
}

static bool testListCreate() {
    ASSERT_TEST(listCreate(NULL,NULL) == NULL);
    ASSERT_TEST(listCreate(copyString,NULL) == NULL);
    ASSERT_TEST(listCreate(NULL,freeString) == NULL);
    List list = listCreate(copyString,freeString);
    ASSERT_TEST(list != NULL);
    listDestroy(list);
    return true;
}

static bool testListFilter() {
    char* a[5] = {"aaa","bbb","NI","hello mister fish","I"};
    List list = listCreate(copyString,freeString);
    for (int i=0 ; i <5 ; ++i){
        listInsertFirst(list,a[i]);
    }
    int key = 1;
    ASSERT_TEST(listFilter(NULL,isLongerThan,&key) == NULL);
    ASSERT_TEST(listFilter(list,NULL,&key) == NULL);
    key = 5;
    List filtered = listFilter(list,isLongerThan, &key);
    ASSERT_TEST(listGetSize(filtered) == 1);
    ASSERT_TEST(strcmp(listGetFirst(filtered),a[3])==0);
    listDestroy(list);
    listDestroy(filtered);
    return true;
}

static bool testListCopy() {
    List list = listCreate(copyString,freeString);
    ASSERT_TEST(listCopy(NULL) == NULL);
    List new_list = listCopy(list);
    ASSERT_TEST(listGetSize(new_list) == 0);
    listDestroy(new_list);
    char* a[5] = {"aaa","bbb","NI","hello mister fish","I"};
    for (int i=0;i <5; ++i){
        listInsertFirst(list,a[i]);
    }
    new_list = listCopy(list);
    ASSERT_TEST(strcmp(listGetFirst(list),listGetFirst(new_list)) == 0);
    ASSERT_TEST(listGetSize(list) == listGetSize(new_list));
    listDestroy(list);
    listDestroy(new_list);
    return true;
}

static bool testListGetSize() {
    char* a[5] = {"aaa","bbb","NI","hello mister fish","I"};
    List list = listCreate(copyString,freeString);
    for (int i=0;i <5; ++i){
        listInsertFirst(list,a[i]);
    }
    ASSERT_TEST(listGetSize(list) == 5);
    listDestroy(list);
    list = listCreate(copyString,freeString);
    ASSERT_TEST(listGetSize(list) == 0);
    listDestroy(list);
    return true;
}

static bool testListGetFirst() {
    char* a[3] = {"aaa","bbb","NI"};
    List list = listCreate(copyString,freeString);
    ASSERT_TEST(listGetFirst(list)==NULL);
    for(int i=0 ; i<3 ; i++){
        listInsertLast(list,a[i]);
    }
    listGetFirst(list);
    ASSERT_TEST(strcmp(listGetNext(list),a[1]) == 0);
    listDestroy(list);
    return true;
}

static bool testListGetNext() {
    List list = listCreate(copyString,freeString);
    ASSERT_TEST(listGetNext(list) == NULL);
    char* a[4] = {"saas", "aaa","bbb","NI"};
    for(int i=0 ; i<3 ; i++){
        listInsertLast(list,a[i]);
    }
    listGetFirst(list);
    ASSERT_TEST(strcmp(listGetNext(list),a[1])==0);
    listDestroy(list);
    ASSERT_TEST(listGetNext(NULL) == NULL);
    return true;
}

static bool testListInsertFirst() {
    List list = listCreate(copyString,freeString);
    ASSERT_TEST(listInsertFirst(list,NULL) == LIST_NULL_ARGUMENT);
    ASSERT_TEST(listInsertFirst(NULL,NULL) == LIST_NULL_ARGUMENT);
    ListElement element = "aaa";
    ASSERT_TEST(listInsertFirst(list,element) == LIST_SUCCESS);
    ASSERT_TEST(strcmp(listGetFirst(list),element) == 0);
    listDestroy(list);
    return true;
}

static bool testListInsertLast() {
    List list = listCreate(copyString,freeString);
    char* a[4] = {"aaa","bbb","ccc",NULL};
    ASSERT_TEST(listInsertLast(list,a[3]) == LIST_NULL_ARGUMENT);
    ASSERT_TEST(listInsertLast(NULL,a[0]) == LIST_NULL_ARGUMENT);
    ASSERT_TEST(listInsertLast(list,a[0]) == LIST_SUCCESS);
    ASSERT_TEST(strcmp(listGetFirst(list),a[0]) == 0);
    listInsertLast(list,a[1]);
    listGetNext(list);
    ASSERT_TEST(strcmp(listGetCurrent(list),a[1]) == 0);
    listDestroy(list);
    return true;
}

static bool testListInsertBeforeCurrent() {
    List list = listCreate(copyString,freeString);
    char* a[4] = {"aaa","bbb","ccc",NULL};
    ASSERT_TEST(listInsertBeforeCurrent(list,a[3]) == LIST_NULL_ARGUMENT);
    ASSERT_TEST(listInsertBeforeCurrent(NULL,a[0]) == LIST_NULL_ARGUMENT);
    ASSERT_TEST(listInsertBeforeCurrent(list,a[0]) == LIST_SUCCESS);
    ASSERT_TEST(listInsertBeforeCurrent(list,a[0]) == LIST_INVALID_CURRENT);
    listGetFirst(list);
    ASSERT_TEST(listInsertBeforeCurrent(list,a[0]) == LIST_SUCCESS);
    ASSERT_TEST(strcmp(listGetFirst(list),a[0]) == 0);
    listInsertBeforeCurrent(list,a[1]);
    ASSERT_TEST(strcmp(listGetFirst(list),a[1]) == 0);
    listGetNext(list);
    listInsertBeforeCurrent(list,a[2]);
    ASSERT_TEST(strcmp(listGetFirst(list),a[1]) == 0);
    ASSERT_TEST(listInsertBeforeCurrent(list,a[3]) == LIST_NULL_ARGUMENT);
    listDestroy(list);
    return true;
}

static bool testListInsertAfterCurrent() {
    List list = listCreate(copyString,freeString);
    char* a[4] = {"aaa","bbb","ccc",NULL};
    ASSERT_TEST(listInsertAfterCurrent(list,a[3]) == LIST_NULL_ARGUMENT);
    ASSERT_TEST(listInsertAfterCurrent(NULL,a[0]) == LIST_NULL_ARGUMENT);
    ASSERT_TEST(listInsertAfterCurrent(list,a[0]) == LIST_SUCCESS);
    ASSERT_TEST(listInsertAfterCurrent(list,a[0]) == LIST_INVALID_CURRENT);
    listGetFirst(list);
    ASSERT_TEST(listInsertAfterCurrent(list,a[0]) == LIST_SUCCESS);
    ASSERT_TEST(strcmp(listGetFirst(list),a[0]) == 0);
    listInsertAfterCurrent(list,a[1]);
    ASSERT_TEST(strcmp(listGetNext(list),a[1]) == 0);
    listInsertAfterCurrent(list,a[2]);
    ASSERT_TEST(strcmp(listGetCurrent(list),a[1]) == 0);
    ASSERT_TEST(strcmp(listGetNext(list),a[2]) == 0);
    ASSERT_TEST(listInsertAfterCurrent(list,a[3]) == LIST_NULL_ARGUMENT);
    listDestroy(list);
    return true;
}

static bool testListRemoveCurrent() {
    List list = listCreate(copyString,freeString);
    ASSERT_TEST(listRemoveCurrent(list) == LIST_INVALID_CURRENT);
    char* a[4] = {"aaa","bbb","ccc","ddd"};
    for(int i=0 ; i<4 ; i++){
        listInsertLast(list,a[i]);
    }
    ASSERT_TEST(listRemoveCurrent(NULL) == LIST_NULL_ARGUMENT);
    ASSERT_TEST(listRemoveCurrent(list) == LIST_INVALID_CURRENT);
    listGetFirst(list);
    ASSERT_TEST(listRemoveCurrent(list) == LIST_SUCCESS);
    ASSERT_TEST(strcmp(listGetFirst(list),a[1]) == 0 );
    listDestroy(list);
    return true;
}

static bool testListSort() {
    char* a[5] = {"aaa","bbb","NI","hello mister fish","I"};
    List list = listCreate(copyString,freeString);
    ASSERT_TEST(listSort(list,compareStrings) == LIST_SUCCESS);
    for (int i=0;i <5; ++i){
        listInsertFirst(list,a[i]);
    }
    ASSERT_TEST(listSort(list,compareStrings) == LIST_SUCCESS);
    ASSERT_TEST(strcmp(listGetFirst(list),a[4]) == 0);
    ASSERT_TEST(strcmp(listGetNext(list),a[2]) == 0);
    ASSERT_TEST(strcmp(listGetNext(list),a[0]) == 0);
    ASSERT_TEST(strcmp(listGetNext(list),a[1]) == 0);
    ASSERT_TEST(strcmp(listGetNext(list),a[3]) == 0);
    listDestroy(list);
    return true;
}

static bool testListClear() {
    List list = listCreate(copyString,freeString);
    ASSERT_TEST(listClear(NULL) == LIST_NULL_ARGUMENT);
    char* a[4] = {"aaa","bbb","ccc","ddd"};
    for(int i=0 ; i<4 ; i++){
        listInsertLast(list,a[i]);
    }
    ASSERT_TEST(listClear(list) == LIST_SUCCESS);
    ASSERT_TEST(listGetSize(list) == 0);
    listDestroy(list);
    return true;
}

static bool testListDestroy() {

    return true;
}

int main (int argv, char** arc){
    RUN_TEST(testListCreate);
    RUN_TEST(testListFilter);
    RUN_TEST(testListCopy);
    RUN_TEST(testListGetSize);
    RUN_TEST(testListGetFirst);
    RUN_TEST(testListGetNext);
    RUN_TEST(testListInsertFirst);
    RUN_TEST(testListInsertLast);
    RUN_TEST(testListInsertBeforeCurrent);
    RUN_TEST(testListInsertAfterCurrent);
    RUN_TEST(testListRemoveCurrent);
    RUN_TEST(testListSort);
    RUN_TEST(testListClear);
    RUN_TEST(testListDestroy);
    return 0;
}