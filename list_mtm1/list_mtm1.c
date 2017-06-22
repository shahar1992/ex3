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


/**=====================Static function declaration===========================*/
static elementsList createNewListElement(List list, ListElement element);


/**======================listCreate===========================================*/
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

/**======================listDestroy==========================================*/
void listDestroy(List list){
    if(!list) {
        return;
    }
    listClear(list);
    free(list);
}

/**======================listCopy=============================================*/
List listCopy(List list){
    NULL_CHECK(list);
    List new_list = listCreate(list->copy_function,list->free_function);
    NULL_CHECK(new_list);
    LIST_FOREACH(ListElement, current_element, list){
        ListResult result = listInsertLast(new_list,current_element);
        if(result != LIST_SUCCESS){
            listDestroy(new_list);
            return NULL;
        }
    }
    new_list->current_element = &new_list->list_elements;
    elementsList ptr = list->list_elements;
    while(ptr != list->current_element){
        if((*new_list->current_element)->next == NULL){
            *new_list->current_element = NULL;
            break;
        }
        new_list->current_element = &((*new_list->current_element)->next);
        ptr = ptr->next;
    }
    new_list->list_size = list->list_size;
    return new_list;
}

/**======================listGetSize==========================================*/
int listGetSize(List list) {
    if(!list) {
        return -1;
    }
    return list->list_size;
}

/**======================listInsertFirst======================================*/
ListResult listInsertFirst(List list, ListElement element){
    LIST_NULL_ARGUMENT_CHECK(list);
    assert(element != NULL);
    elementsList new_list_element = createNewListElement(list,element);
    if(!new_list_element){
        return LIST_OUT_OF_MEMORY;
    }
    assert(new_list_element->element != NULL);
    new_list_element->next = list->list_elements;
    list->list_elements = new_list_element;
    list->list_size++;
    return LIST_SUCCESS;
}

/**======================listInsertLast=======================================*/
ListResult listInsertLast(List list, ListElement element){
    LIST_NULL_ARGUMENT_CHECK(list);
    assert(element != NULL);
    elementsList ptr = list->list_elements;
    if(!ptr){
        return listInsertFirst(list,element);
    }
    elementsList new_list_element = createNewListElement(list,element);
    if(!new_list_element){
        return LIST_OUT_OF_MEMORY;
    }
    assert(new_list_element->element != NULL);
    while(ptr->next){
        ptr = ptr->next;
    }
    new_list_element->next = ptr->next;
    ptr->next = new_list_element;
    list->list_size++;
    return LIST_SUCCESS;
}

/**======================listInsertBeforeCurrent==============================*/
ListResult listInsertBeforeCurrent(List list, ListElement element){
    LIST_NULL_ARGUMENT_CHECK(list);
    if(!list->current_element){
        return LIST_INVALID_CURRENT;
    }
    assert(element != NULL);
    elementsList new_list_element = createNewListElement(list,element);
    if(!new_list_element){
        return LIST_OUT_OF_MEMORY;
    }
    assert(new_list_element->element != NULL);
    if(*list->current_element == list->list_elements) {
        new_list_element->next = *list->current_element;
        list->list_elements = new_list_element;
        new_element->next = list->current_element;
    }
    else {
        elementsList ptr = list->list_elements;
        while (ptr->next != *list->current_element) {
            ptr = ptr->next;
        }
        new_list_element->next = ptr->next;
        ptr->next = new_list_element;
    }
    list->list_size++;
    return LIST_SUCCESS;
}

/**======================listInsertAfterCurrent===============================*/
ListResult listInsertAfterCurrent(List list, ListElement element){
    LIST_NULL_ARGUMENT_CHECK(list);
    if(!list->current_element){
        return LIST_INVALID_CURRENT;
    }
    assert(element != NULL);
    elementsList new_list_element = createNewListElement(list,element);
    if(!new_list_element){
        return LIST_OUT_OF_MEMORY;
    }
    assert(new_list_element->element != NULL);
    new_list_element->next = list->current_element->next;
    list->current_element->next = new_list_element;
    list->list_size++;
    return LIST_SUCCESS;
}

/**======================listRemoveCurrent====================================*/
ListResult listRemoveCurrent(List list){
    LIST_NULL_ARGUMENT_CHECK(list);
    if(*list->current_element == list->list_elements){
        list->list_elements = list->list_elements->next;
        list->free_function((*list->current_element)->element);
        free(*list->current_element);
        list->current_element = &list->list_elements;
    }
    else {
        elementsList ptr = list->list_elements;
        while(ptr->next != *list->current_element){
            ptr = ptr->next;
        }
        ptr->next = list->current_element->next;
        list->free_function((*list->current_element)->element);
        free(*list->current_element);
        list->current_element = &ptr->next;
    }
    list->list_size--;
    return LIST_SUCCESS;
}

/**======================listRemoveCurrent====================================*/
ListElement listGetCurrent(List list){
    NULL_CHECK(list && list->current_element);
    return (*list->current_element)->element;
}

/**======================listRemoveCurrent====================================*/
ListElement listGetFirst(List list) {
    NULL_CHECK(list);
    list->current_element = &list->list_elements;
    assert(*list->current_element == list->list_elements);
    return *list->current_element;
}

/**======================listRemoveCurrent====================================*/
ListElement listGetNext(List list){
    NULL_CHECK(list && list->current_element);
    NULL_CHECK(*list->current_element);
    list->current_element = &((*list->current_element)->next);
    return (*list->current_element)->element;
}

/**======================listSort=============================================*/
ListResult listSort(List list, CompareListElements compareElement){
    LIST_NULL_ARGUMENT_CHECK(list);
    List sorted_list = listCreate(list->copy_function,list->free_function);
    ListResult result;
    if(!sorted_list) {
        return LIST_OUT_OF_MEMORY;
    }
    LIST_FOREACH(ListElement,list_element,list){
        if(list_element == list->list_elements->element) {
            result = listInsertFirst(sorted_list,list_element);
            if(result != LIST_SUCCESS) {
                listDestroy(sorted_list);
                return result;
            }
        }
        else{
            bool is_element_entered = false;
            LIST_FOREACH(ListElement,element_to_compare,sorted_list) {
                if (compareElement(list_element,element_to_compare) < 0 ) {
                    result = listInsertBeforeCurrent(sorted_list, list_element);
                    if(result != LIST_SUCCESS){
                        listDestroy(sorted_list);
                        return result;
                    }
                    break;
                }
            }
            result = listInsertLast(sorted_list,list_element);
            if(result != LIST_SUCCESS){
                listDestroy(sorted_list);
                return result;
            }
        }
    }
    listDestroy(list);
    list = sorted_list;
    return LIST_SUCCESS;
}

/**======================listFilter===========================================*/
List listFilter(List list, FilterListElement filterElement, ListFilterKey key){
    LIST_NULL_ARGUMENT_CHECK(list && filterElement);
    List new_list = listCreate(list->copy_function,list->free_function);
    if(!new_list){
        return NULL;
    }
    LIST_FOREACH(ListElement,element,list){
        if(filterElement(element,key)){
            ListResult result = listInsertLast(new_list,element);
            if(result != LIST_SUCCESS){
                listDestroy(new_list);
                return result;
            }
        }
    }
    return LIST_SUCCESS;
}

/**======================listClear============================================*/
ListResult listClear(List list){
    LIST_NULL_ARGUMENT_CHECK(list);
    while(list->list_elements){
        elementsList ptr = list->list_elements;
        list->free_function(ptr->element);
        list->list_elements = ptr->next;
        free(ptr);
    }
    return LIST_SUCCESS;
}



/**====================Static functions implementations=======================*/
static elementsList createNewListElement(List list, ListElement element){
    assert(list && element);
    elementsList new_list_element = malloc(sizeof(*new_list_element));
    if(!new_list_element){
        return NULL;
    }
    new_list_element->element = list->copy_function(element);
    if(!new_list_element->element){
        free(new_list_element);
        return NULL;
    }
}









