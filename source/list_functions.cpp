#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "functions_and_structures.h"

static void clear_list(struct List *list);


Errors list_constructor(struct List **list)
{
    *list = (struct List *) calloc (1, sizeof(struct List));
    if (*list == NULL)
    {
        return ERROR_OF_LIST_CONSTRUCTOR;
    }
    (*list)->previous_element = NULL;
    (*list)->next_element = NULL;
    return NO_ERRORS;
}


static void clear_list(struct List *list)
{
    if (list->next_element != NULL)
    {
        clear_list(list->next_element);
    }
    free(list->data);
    list->data = NULL;
    free(list);
    list = NULL;
    return;
}

Errors list_destructor(struct List *list)
{
    if (list == NULL)
    {
        return ERROR_OF_LIST_DESTRUCTOR;
    }
    clear_list(list);
    return NO_ERRORS;
}

Errors list_append(struct List *list, const char *element)
{
    if (list == NULL || element == NULL)
    {
        return ERROR_OF_APPEND_TO_LIST;
    }
    //printf("element in list_append = %s\n", element);
    const char *old_element = element;
    size_t len_of_element = strlen(element);
    element = old_element;
    list->data = (char *) calloc (len_of_element + 1, sizeof(char));
    list->frequency = 1;
    if (list->data == NULL)
    {
        return ERROR_OF_APPEND_TO_LIST;
    }

    memcpy(list->data, element, len_of_element + 1);

    list->len_of_data = len_of_element;
    
    return NO_ERRORS;
}

Errors list_delete(struct List *list)
{
    if (list == NULL)
    {
        return ERROR_OF_DELETE_FROM_LIST;
    }
    if (list->next_element != NULL)
    {
        (list->next_element)->previous_element = (list->previous_element);
        (list->previous_element)->next_element = (list->next_element);
    }
    else
    {
        (list->previous_element)->next_element = NULL;
    }
    free(list->data);
    list->len_of_data = 0;
    list->data = NULL;
    list = NULL;
    return NO_ERRORS;
}

Errors list_print(struct List *list)
{
    if (list == NULL)
    {
        return ERROR_OF_PRINT_LIST;
    }
    struct List *current = list;
    while (current != NULL)
    {
        printf("list data - %s||frequency = %lu\n", (current->data), (current->frequency));

        current = current->next_element;
    }
    return NO_ERRORS;
}


void list_append_collision(struct List *list, const char *element, bool *founded, struct List *previous, int hash)
{
    if (*founded)
    {
        return;
    }
    // while (list != NULL)
    // {
    //     // int verdict = strcasecmp(list->data, element);
    //     // if (verdict == 0)
    //     // {
    //     //     (list->frequency) += 1;
    //     //     *founded = true;
    //     //     return;
    //     // }
    //     int verdict = -1;
    //     if (hash % 2 == 0)
    //     {
    //         verdict = my_strcasecmp(list->data, element);
    //     }
    //     else
    //     {
    //         verdict = my_two_strcasecmp(list->data, element);
    //     }
    //     if (verdict == 0)
    //     {
    //         (list->frequency) += 1;
    //         *founded = true;
    //         return;
    //     }
    //     previous = list;
    //     list = list->next_element;
    // }
    if (list != NULL)
    {
        int verdict = strcasecmp(list->data, element);
        if (verdict == 0)
        {
            (list->frequency) += 1;
            *founded = true;
            return;
        }
        list_append_collision(list->next_element, element, founded, list, hash);
    }
    if (!(*founded))
    {
        Errors error = list_constructor(&(list));
        if (error != NO_ERRORS)
        {
            printf("ERROR IN list_append_collision\n");
            return;
        }
        list->previous_element = previous;
        if (previous != NULL)
        {
            previous->next_element = list;
        }
        error = list_append(list, element);
        if (error != NO_ERRORS)
        {
            printf("ERROR IN list_append_collision\n");
            return;
        }
        *founded = true;
    }
    return;   
}