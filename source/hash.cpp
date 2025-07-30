#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "functions_and_structures.h"
#include "hash_table_dump.h"
#include "reader_and_parser.h"

int main(int argc, char *argv[])
{
    struct Table table = {0};
    struct Hash_data hash_data = {0};
    FILE *file_pointer = fopen("source/data.txt", "rb");
    Errors_of_reader error_reader = get_data_for_table(&hash_data, file_pointer);
    if (error_reader != NO_READER_ERRORS)
    {
        fprintf(stderr, "error of reader = %d\n", error_reader);
        return 1;
    }
    // for (size_t index = 0; index < (hash_data.size_of_buffer_for_data); index++)
    // {
    //     printf("%s\n", (hash_data.buffer_for_data)[index]);
    // }
    Errors error = NO_ERRORS;
    size_t size_of_table = 50;
    error = hash_table_constructor(&table, size_of_table);
    if (error != NO_ERRORS)
    {
        fprintf(stderr, "error = %d\n", error);
        return 1;
    }
    for (size_t index = 0; index < 1000; index++)
    {
        error = write_data_to_hash_table(&table, &hash_data);
        if (error != NO_ERRORS)
        {
            fprintf(stderr, "error = %d\n", error);
            return 1;
        }
    }
    // char str_1[] = "He";
    // char str_2[] = "st";
    // error = hash_table_append(&table, str_1);
    // if (error != NO_ERRORS)
    // {
    //     fprintf(stderr, "error = %d\n", error);
    //     return 1;
    // }
    // error = hash_table_append(&table, str_2);
    // if (error != NO_ERRORS)
    // {
    //     fprintf(stderr, "error = %d\n", error);
    //     return 1;
    // }
    // error = hash_table_append(&table, str_2);
    // error = hash_table_print(&table);
    // if (error != NO_ERRORS)
    // {
    //     fprintf(stderr, "error = %d\n", error);
    //     return 1;
    // }
    if (argc < 2)
    {
        fprintf(stderr, "Error! There is no flag\n");
        return 1;
    }
    char *flag = argv[1];
    if (flag[1] == 'y')
    {
        error = graphic_dump(&table, "added_elements");
        if (error != NO_ERRORS)
        {
            fprintf(stderr, "error = %d\n", error);
            return 1;
        }
    }
    error = hash_table_destructor(&table, &hash_data);
    if (error != NO_ERRORS)
    {
        fprintf(stderr, "error = %d\n", error);
        return 1;
    }
    return 0;
}