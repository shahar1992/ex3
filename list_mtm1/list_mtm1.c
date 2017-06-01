#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#include "list_mtm1.h"

#define NULL_CHECK(condition) \
    if(!(condition)) { \
        return NULL; \
    }

#define LIST_NULL_ARGUMENT_CHECK(condition) \
    if(!(condition)) { \
        return LIST_NULL_ARGUMENT; \
    }

#define NEW_ELEMENT_ALLOCATE_CHECK(new_element) \
    if(!(new_element)) { \
    return LIST_OUT_OF_MEMORY; \
    }

/*#define CREATING_NEW_ELEMENT \
    LIST_NULL_ARGUMENT_CHECK(!list); \
    elementsList new_element = createElement(list, element); \
    NEW_ELEMENT_ALLOCATE_CHECK(new_element);
*/

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
    elementsList *current_element;
    elementsList list_elements;
    int list_size;
};

/**
 * Header function.
 */
List listCreate(CopyListElement copyElement, FreeListElement freeElement){
    NULL_CHECK(copyElement && freeElement);
    List new_list = malloc(sizeof(*new_list));
    NULL_CHECK(new_list);
    new_list->list_elements = NULL;
    new_list->current_element = &new_list->list_elements;
    new_list->copy_function = copyElement;
    new_list->free_function = freeElement;
    new_list->list_size = 0;
    return new_list;
}

/**
 * Header function.
 */
List listCopy(List list){
    NULL_CHECK(list);
    List new_list = listCreate(list->copy_function,list->free_function);
    NULL_CHECK(new_list);
    LIST_FOREACH(elementsList,current_element,list){
        elementsList new_list_element = malloc(sizeof(*new_element));
        if(!new_list_element){
            listDestroy(new_list);
            return NULL;
        }
        new_list_element->next = NULL;
        new_list_element->element =
                list->copy_function(current_element->element);
        (*new_list->current_element)->next = new_list_element;
    }

    new_list->list_size = list->list_size;
    return new_list;
}

/**
 * Header function.
 */
int listGetSize(List list) {
    if(!list) {
        return -1;
    }
    return list->list_size;
}

/**
 * Header function.
 */
ListElement listGetFirst(List list){
    NULL_CHECK((!list) || (listGetSize(list) == 0));
    NULL_CHECK(!list->current_element);
    list->current_element = list->list_elements;
    return list->current_element->element;
}

/**
 * Header function.
 */
ListElement listGetNext(List list){
    NULL_CHECK((!list) || (!list->current_element->next));
    list->current_element = list->current_element->next;
    NULL_CHECK(!list->current_element);
    return list->current_element->element;
}

/**
 * Header function.
 */
ListElement listGetCurrent(List list){
    NULL_CHECK((!list));
    NULL_CHECK(!list->current_element);
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
ListResult listSort(List list, CompareListElements compareElement){
    LIST_NULL_ARGUMENT_CHECK((!list) || (!compareElement));
    elementsList list_as_array = malloc(sizeof(elementsList)*listGetSize(list));
    LIST_NULL_ARGUMENT_CHECK(list_as_array);
    int i=0;
    elementsList ptr = list->list_elements;
    while(ptr){
        (list_as_array+i)->element = list->copy_function(ptr->element);
    }

    free(list_as_array);
    return LIST_SUCCESS;
}

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









