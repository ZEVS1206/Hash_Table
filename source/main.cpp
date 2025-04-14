#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "functions_and_structures.h"
#include "hash_table_dump.h"

int main()
{
    struct Table table = {0};
    Errors error = NO_ERRORS;
    size_t size_of_table = 3;
    error = hash_table_constructor(&table, size_of_table);
    if (error != NO_ERRORS)
    {
        fprintf(stderr, "error = %d\n", error);
        return 1;
    }
    char str_1[] = "He";
    char str_2[] = "st";
    error = hash_table_append(&table, str_1);
    if (error != NO_ERRORS)
    {
        fprintf(stderr, "error = %d\n", error);
        return 1;
    }
    error = hash_table_append(&table, str_2);
    if (error != NO_ERRORS)
    {
        fprintf(stderr, "error = %d\n", error);
        return 1;
    }
    error = graphic_dump(&table, "added_two_elements");
    //error = hash_table_print(&table);
    if (error != NO_ERRORS)
    {
        fprintf(stderr, "error = %d\n", error);
        return 1;
    }
    error = hash_table_destructor(&table);
    if (error != NO_ERRORS)
    {
        fprintf(stderr, "error = %d\n", error);
        return 1;
    }
    return 0;
}