#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#include "list_mtm1.h"
 //avivi is your her

#define NULL_CHECK(condition) \
    if(condition) { \
        return NULL; \
    }

#define LIST_NULL_ARGUMENT_CHECK(condition) \
    if(condition) { \
        return LIST_NULL_ARGUMENT; \
    }

#define NEW_ELEMENT_ALLOCATE_CHECK(new_element) \
    if(!new_element) { \
    return LIST_OUT_OF_MEMORY; \
    }

#define CREATING_NEW_ELEMENT \
    LIST_NULL_ARGUMENT_CHECK(!list); \
    elementsList new_element = createElement(list, element); \
    NEW_ELEMENT_ALLOCATE_CHECK(new_element);

/**
 * Definition of linked list of elements.
 * Each element of the list has a data and a pointer to the next element.
 */
typedef struct Elements{
    ListElement element;
    struct Elements* next;
}*elementsList;

/**
 * Definition of ADT List.
 * Each member of this struct has 2 pointer to functions, linked list of
 * elements, integer counts the linked list size and iterator called
 * 'current_element'.
 */
struct list_t{
    CopyListElement copy_function;
    FreeListElement free_function;
    elementsList current_element;
    elementsList list_elements;
    int list_size;
};

ListResult copyListElements(List new_list, List list);

/**
 * Header function.
 */
List listCreate(CopyListElement copyElement, FreeListElement freeElement){
    if(!(copyElement && freeElement)){
        return NULL;
    }
    List list= malloc(sizeof(*list));
    if(!list) {
        return NULL;
    }
    list->list_elements = NULL;
    list->copy_function = copyElement;
    list->free_function = freeElement;
    list->current_element = malloc(sizeof(elementsList));
    NULL_CHECK(!list->current_element);
    list->current_element->next=NULL;
    list->current_element->element=NULL;
    list->list_size = 0;
    return list;
}

/**
 * Header function.
 */
List listCopy(List list){
    NULL_CHECK(!list);
    List new_list = listCreate(list->copy_function , list->free_function);
    NULL_CHECK(!new_list);
    new_list->copy_function = list->copy_function;
    new_list->free_function = list->free_function;
    if(copyListElements(new_list,list) == LIST_OUT_OF_MEMORY) {
//        listDestroy(new_list);
        return NULL;
    }
    assert(new_list->current_element!=NULL);
    return new_list;
}

/**
 * This function create a new copy of elements list.
 * @param new_list This is the list that we're creating.
 * @param list This is list we're coping from.
 * @return
 *      List_OUT_OF_MEMORY if allocating failed.
 *      LIST_SUCCESS if succeeded.
 */
ListResult copyListElements(List new_list, List list){
    LIST_FOREACH(ListElement,iterator,list){
        if(listInsertFirst(new_list, iterator) == LIST_OUT_OF_MEMORY) {
            return LIST_OUT_OF_MEMORY;
        }
    }
    elementsList ptr = list->list_elements;
    int counter = 0;
    while(ptr != list->current_element){
        ptr = ptr->next;
        counter++;
    }
    ptr = new_list->list_elements;
    while (counter) {
        ptr = ptr->next;
        counter--;
    }
    new_list->current_element = ptr;
    return LIST_SUCCESS;
}

/**
 * This function create a new element.
 * @param new_list The list we're adding the element to.
 * @param element The new element we're creating.
 * @return
 *      NULL if allocating failed.
 *      The new element we created otherwise.
 */
elementsList createElement(List new_list, ListElement element){
    assert((new_list != NULL) && (element != NULL));
    elementsList new_element = malloc(sizeof(*new_element));
    NULL_CHECK(!new_element);
    new_element->element = new_list->copy_function(element);
    if(!new_element->element) {
        free(new_element);
        return NULL;
    }
    new_element->next=NULL;
    return new_element;
}

/**
 * Header function.
 */
int listGetSize(List list) {
    if(!list) {
        return -1;
    }
    else return list->list_size;
}

/**
 * Header function.
 */
ListElement listGetFirst(List list){
    NULL_CHECK((!list) || (listGetSize(list) == 0));
    list->current_element = list->list_elements;
    return list->current_element->element;
}

/**
 * Header function.
 */
ListElement listGetNext(List list){
    NULL_CHECK((!list) || (!list->current_element->next));
    list->current_element = list->current_element->next;
    return list->current_element->element;
}

/**
 * Header function.
 */
ListElement listGetCurrent(List list){
    NULL_CHECK((!list));
    return list->current_element->element;
}


/**
 * Header function.
 */
ListResult listInsertFirst(List list, ListElement element){
    LIST_NULL_ARGUMENT_CHECK(!list);
    elementsList new_element =
            createElement(list, element);
    NEW_ELEMENT_ALLOCATE_CHECK(new_element);
    new_element->next = list->list_elements;
    list->list_elements = new_element;
    list->list_size++;
    return LIST_SUCCESS;
}

/**
 * Header function.
 */
ListResult listInsertLast(List list, ListElement element){
    LIST_NULL_ARGUMENT_CHECK(!list);
    elementsList new_element =
            createElement(list, element);
    NEW_ELEMENT_ALLOCATE_CHECK(new_element);
    elementsList ptr = list->list_elements;
    while(ptr->next) {
        ptr = ptr->next;
    }
    ptr->next = new_element;
    list->list_size++;
    return LIST_SUCCESS;
}

/**
 * Header function.
 */
ListResult listInsertBeforeCurrent(List list, ListElement element){
    LIST_NULL_ARGUMENT_CHECK(!list);
    elementsList new_element =
            createElement(list, element);
    NEW_ELEMENT_ALLOCATE_CHECK(new_element);
    if(list->current_element == list->list_elements) {
        list->list_elements = new_element;
        new_element->next = list->current_element;
        return LIST_SUCCESS;
    }
    elementsList ptr = list->list_elements;
    while(ptr->next != list->current_element) {
        ptr = ptr->next;
    }
    ptr->next = new_element;
    new_element->next = list->current_element;
    list->list_size++;
    return LIST_SUCCESS;
}

/**
 * Header function.
 */
ListResult listInsertAfterCurrent(List list, ListElement element){
    LIST_NULL_ARGUMENT_CHECK(!list);
    elementsList new_element =
            createElement(list, element);
    NEW_ELEMENT_ALLOCATE_CHECK(new_element);
    new_element->next = list->current_element->next;
    list->current_element->next=new_element;
    list->list_size++;
    return LIST_SUCCESS;
}

/**
 * Header function.
 */
ListResult listRemoveCurrent(List list){
    LIST_NULL_ARGUMENT_CHECK(!list);
    if(list->current_element == list->list_elements){
        list->list_elements = list->list_elements->next;
        list->free_function(list->current_element->element);
        free(list->current_element);
        list->current_element = list->list_elements;
        return LIST_SUCCESS;
    }
    elementsList ptr = list->list_elements;
    while(ptr->next != list->current_element){
        ptr = ptr->next;
    }
    ptr->next = list->current_element->next;
    list->free_function(list->current_element->element);
    list->current_element = ptr->next;
}

/**
 * Header function
 */
/*ListResult listSort(List list, CompareListElements compareElement){
    LIST_NULL_ARGUMENT_CHECK((!list) || (!compareElement));
    elementsList list_as_array =
            malloc(sizeof(elementsList)*listGetSize(list));
    LIST_NULL_ARGUMENT_CHECK(list_as_array);
    int i=0;
    elementsList ptr = list->list_elements;
    while(ptr->next){
        (list_as_array+i)->element = list->copy_function(ptr->element);

    }
    return LIST_SUCCESS;
}*/

/**
 * Header function.
 */
ListResult listClear(List list){
    LIST_NULL_ARGUMENT_CHECK(!list);
    elementsList ptr;
    while(list->list_elements) {
        ptr = list->list_elements;
        list->free_function(ptr->element);
        list->list_elements = ptr->next;
        free(ptr);
    }
    return LIST_SUCCESS;
}

/**
 * Header function
 */
void listDestroy(List list){
    if(!list) {
        return;
    }
    listClear(list);
    free(list->list_elements);
    free(list);
}









