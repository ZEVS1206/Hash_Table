#ifndef MAIN_STRUCTURES_H
#define MAIN_STRUCTURES_H

#include "reader_and_parser.h"

enum Errors
{
    NO_ERRORS                  = 0,
    ERROR_OF_LIST_CONSTRUCTOR  = 1,
    ERROR_OF_LIST_DESTRUCTOR   = 2,
    //ERROR_OF_OPEN_FILE         = 3,
    ERROR_OF_ANALYZE_TEXT      = 4,
    ERROR_OF_APPEND_TO_LIST    = 5,
    ERROR_OF_DELETE_FROM_LIST  = 6,
    ERROR_OF_PRINT_LIST        = 7,
    ERROR_OF_TABLE_CONSTRUCTOR = 8,
    ERROR_OF_TABLE_DESTRUCTOR  = 9,
    ERROR_OF_APPEND_TO_TABLE   = 10,
    ERROR_OF_PRINT_TABLE       = 11,
    ERROR_OF_DUMP              = 12,
    ERROR_OF_DANGEROUS_COMMAND = 13
};

struct List
{
    size_t len_of_data;
    char* data;
    size_t frequency;
    struct List *next_element;
    struct List *previous_element;
};

struct Hash_table
{
    int key;
    struct List *list_element;
    size_t count_of_list_elements;
};

struct Table
{
    struct Hash_table **hash_table;
    size_t size_of_table;
};


extern "C" int my_strcasecmp(const char *str1, const char *str2);
extern "C" int my_strlen(const char *str);

Errors hash_table_constructor(struct Table *table, size_t size_of_table);
Errors hash_table_destructor(struct Table *table, struct Hash_data *hash_data);
Errors hash_table_append(struct Table *table, const char *element);
Errors hash_table_print(struct Table *table);
Errors write_data_to_hash_table(struct Table *table, struct Hash_data *hash_data);
int my_two_strcasecmp(const char *str1, const char *str2);

Errors list_constructor(struct List **list);
Errors list_destructor(struct List *list);
Errors list_append(struct List *list, const char *element);
void list_append_collision(struct List *list, const char *element, bool *founded, struct List *previous, int hash);
Errors list_delete(struct List *list);
Errors list_print(struct List *list);

#endif