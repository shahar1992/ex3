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
    elementsList current_element;
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
    new_list->current_element = new_list->list_elements;
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
    new_list->current_element = new_list->list_elements;
    elementsList ptr = list->list_elements;
    while(ptr){
        if(list->current_element == ptr){
            break;
        }
        new_list->current_element = new_list->current_element->next;
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
    LIST_NULL_ARGUMENT_CHECK(list && element);
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
    LIST_NULL_ARGUMENT_CHECK(list && element);
    elementsList ptr = list->list_elements;
    elementsList new_list_element = createNewListElement(list,element);
    if(!new_list_element){
        return LIST_OUT_OF_MEMORY;
    }
    assert(new_list_element->element != NULL);
    if(listGetSize(list) == 0){
        list->list_elements = new_list_element;
        list->list_elements->next = NULL;
    }
    else {
        while (ptr->next) {
            ptr = ptr->next;
        }
        new_list_element->next = ptr->next;
        ptr->next = new_list_element;
    }
    list->list_size++;
    return LIST_SUCCESS;
}

/**======================listInsertBeforeCurrent==============================*/
ListResult listInsertBeforeCurrent(List list, ListElement element){
    LIST_NULL_ARGUMENT_CHECK(list && element);
    elementsList new_list_element = createNewListElement(list,element);
    if(!new_list_element){
        return LIST_OUT_OF_MEMORY;
    }
    if(list->current_element == NULL){
        if(listGetSize(list) == 0) {
            new_list_element->next = list->list_elements;
            list->list_elements = new_list_element;
            list->list_size++;
            return LIST_SUCCESS;
        }
        list->free_function(new_list_element);
        return LIST_INVALID_CURRENT;
    }
    elementsList ptr = list->list_elements;
    if(list->list_elements == list->current_element){
        list->list_elements = new_list_element;
        new_list_element->next = list->current_element;
        list->list_size++;
        return LIST_SUCCESS;
    }
    while(ptr){
        if(ptr->next == list->current_element){
            new_list_element->next = list->current_element;
            ptr->next = new_list_element;
            list->list_size++;
            return LIST_SUCCESS;
        }
        ptr = ptr->next;
    }
    list->free_function(new_list_element);
    return LIST_INVALID_CURRENT;
}

/**======================listInsertAfterCurrent===============================*/
ListResult listInsertAfterCurrent(List list, ListElement element){
    LIST_NULL_ARGUMENT_CHECK(list && element);
    elementsList new_list_element = createNewListElement(list,element);
    if(!new_list_element){
        return LIST_OUT_OF_MEMORY;
    }
    if(list->current_element == NULL){
        if(listGetSize(list) == 0){
            new_list_element->next = NULL;
            list->list_elements = new_list_element;
            list->list_size++;
            return LIST_SUCCESS;
        }
        list->free_function(new_list_element);
        return LIST_INVALID_CURRENT;
    }
    new_list_element->next = list->current_element->next;
    list->current_element->next = new_list_element;
    list->list_size++;
    return LIST_SUCCESS;
}

/**======================listRemoveCurrent====================================*/
ListResult listRemoveCurrent(List list){
    LIST_NULL_ARGUMENT_CHECK(list);
    elementsList ptr = list->current_element;
    if(!list->current_element){
        return LIST_INVALID_CURRENT;
    }
    while(ptr){
        if(ptr == list->current_element){
            if(list->current_element == list->list_elements){
                list->list_elements = list->list_elements->next;
            }
            list->current_element = list->current_element->next;
            list->free_function(ptr->element);
            free(ptr);
            list->list_size--;
            return LIST_SUCCESS;
        }
        ptr = ptr->next;
    }
    return LIST_INVALID_CURRENT;
}

/**======================listRemoveCurrent====================================*/
ListElement listGetCurrent(List list){
    NULL_CHECK(list && list->current_element);
    return list->current_element->element;
}

/**======================listRemoveCurrent====================================*/
ListElement listGetFirst(List list) {
    NULL_CHECK(list);
    list->current_element = list->list_elements;
    assert(list->current_element == list->list_elements);
    return (listGetSize(list) != 0) ? list->current_element->element : NULL;
}

/**======================listRemoveCurrent====================================*/
ListElement listGetNext(List list){
    NULL_CHECK(list && list->current_element);
    list->current_element = list->current_element->next;
    return (list->current_element != NULL) ? list->current_element->element
                                            : NULL;
}

/**======================listSort=============================================*/
ListResult listSort(List list, CompareListElements compareElement) {
    LIST_NULL_ARGUMENT_CHECK(list);
    List sorted_list = listCreate(list->copy_function, list->free_function);
    ListResult result;
    if (!sorted_list) {
        return LIST_OUT_OF_MEMORY;
    }
    LIST_FOREACH(ListElement, list_element, list) {
        if (list_element == list->list_elements->element) {
            result = listInsertFirst(sorted_list, list_element);
            if (result != LIST_SUCCESS) {
                listDestroy(sorted_list);
                return result;
            }
        } else {
            bool is_element_entered = false;
            LIST_FOREACH(ListElement, element_to_compare_with, sorted_list) {
                if (compareElement(list_element, element_to_compare_with) < 0) {
                    result = listInsertBeforeCurrent(sorted_list, list_element);
                    if (result != LIST_SUCCESS) {
                        listDestroy(sorted_list);
                        return result;
                    }
                    is_element_entered = true;
                    break;
                }
            }
            if (!is_element_entered) {
                result = listInsertLast(sorted_list, list_element);
                if (result != LIST_SUCCESS) {
                    listDestroy(sorted_list);
                    return result;
                }
            }
        }
    }
        listDestroy(list);
        list = listCopy(sorted_list);
        listDestroy(sorted_list);
        return (!list) ? LIST_OUT_OF_MEMORY : LIST_SUCCESS;
}

/**======================listFilter===========================================*/
List listFilter(List list, FilterListElement filterElement, ListFilterKey key) {
    NULL_CHECK(list && filterElement);
    List new_list = listCreate(list->copy_function, list->free_function);
    if (!new_list) {
        return NULL;
    }
    LIST_FOREACH(ListElement, element, list) {
        if (filterElement(element, key)) {
            ListResult result = listInsertLast(new_list, element);
            if (result != LIST_SUCCESS) {
                listDestroy(new_list);
                return NULL;
            }
        }
    }
    return new_list;
}

/**======================listClear============================================*/
    ListResult listClear(List list) {
        LIST_NULL_ARGUMENT_CHECK(list);
        listGetFirst(list);
        while (list->current_element) {
            listRemoveCurrent(list);
        }
        return LIST_SUCCESS;
    }



/**====================Static functions implementations=======================*/
    static elementsList createNewListElement(List list, ListElement element) {
        assert(list && element);
        elementsList new_list_element = malloc(sizeof(*new_list_element));
        if (!new_list_element) {
            return NULL;
        }
        new_list_element->element = list->copy_function(element);
        if (!new_list_element->element) {
            free(new_list_element);
            return NULL;
        }
        return new_list_element;
    }









