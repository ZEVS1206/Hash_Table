#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "functions_and_structures.h"

int my_two_strcasecmp(const char *str1, const char *str2)
{
    int len = 0;
    __asm__ volatile 
    (
        "xor %%eax, %%eax\n\t"
        "1:\n\t"
        "movzbl (%%rdi), %%ecx\n\t"    // s1[i] → ecx
        "movzbl (%%rsi), %%edx\n\t"    // s2[i] → edx
        "cmp $'A', %%cl\n\t"
        "jb 2f\n\t"
        "cmp $'Z', %%cl\n\t"
        "ja 2f\n\t"
        "add $32, %%cl\n\t"
        "2:\n\t"
        "cmp $'A', %%dl\n\t"
        "jb 3f\n\t"
        "cmp $'Z', %%dl\n\t"
        "ja 3f\n\t"
        "add $32, %%dl\n\t"
        "3:\n\t"
        "test %%cl, %%cl\n\t"
        "jz 4f\n\t"
        "cmp %%cl, %%dl\n\t"
        "jne 5f\n\t"
        "inc %%rdi\n\t"
        "inc %%rsi\n\t"
        "jmp 1b\n\t"
        "4:\n\t"
        "test %%dl, %%dl\n\t"
        "jz 6f\n\t"
        "5:\n\t"
        "movzbl %%cl, %%eax\n\t"
        "movzbl %%dl, %%edx\n\t"
        "sub %%edx, %%eax\n\t"
        "jmp 7f\n\t"
        "6:\n\t"
        "xor %%eax, %%eax\n\t"
        "7:\n\t"
        "mov %%eax, %[res]\n\t"
        : [res] "=r" (len)
        : "D" (str1), "S" (str2)
        : "eax", "ecx", "edx", "cc", "memory"
    );
    return len;
}

static int hash_count(const char *data, size_t size_of_table);

static int hash_count(const char *data, size_t size_of_table)
{   
    size_t position = strlen(data) % size_of_table;
    return position;
}

Errors write_data_to_hash_table(struct Table *table, struct Hash_data *hash_data)
{
    if (table == NULL || hash_data == NULL)
    {
        return ERROR_OF_ANALYZE_TEXT;
    }
    if (table->hash_table == NULL || hash_data->buffer_for_data == NULL)
    {
        return ERROR_OF_ANALYZE_TEXT;
    }
    for (size_t index = 0; index < hash_data->size_of_buffer_for_data; index++)
    {
        //printf("%s\n", (hash_data->buffer_for_data)[index]);
        Errors error = hash_table_append(table, (hash_data->buffer_for_data)[index]);
        if (error != NO_ERRORS)
        {
            return error;
        }
    }
    return NO_ERRORS;
}

Errors hash_table_constructor(struct Table *table, size_t size_of_table)
{
    table->size_of_table = size_of_table;
    table->hash_table = (struct Hash_table **) calloc ((table->size_of_table), sizeof(struct Hash_table *));
    if (table->hash_table == NULL)
    {
        return ERROR_OF_TABLE_CONSTRUCTOR;
    }

    for (size_t index = 0; index < (table->size_of_table); index++)
    {
        (table->hash_table)[index] = (struct Hash_table *) calloc (1, sizeof(struct Hash_table));
        if ((table->hash_table)[index] == NULL)
        {
            return ERROR_OF_TABLE_CONSTRUCTOR;
        }
        //Errors error = list_constructor(((table->hash_table)[index])->list_element);
        ((table->hash_table)[index])->list_element = (struct List *) calloc (1, sizeof(struct List));
        if (((table->hash_table)[index])->list_element == NULL)
        {
            return ERROR_OF_TABLE_CONSTRUCTOR;
        }
        (((table->hash_table)[index])->list_element)->next_element = NULL;
        (((table->hash_table)[index])->list_element)->previous_element = NULL;
        // if (error != NO_ERRORS)
        // {
        //     return ERROR_OF_LIST_CONSTRUCTOR;
        // }
        ((table->hash_table)[index])->key = 0;
    }
    return NO_ERRORS;
}

Errors hash_table_destructor(struct Table *table, struct Hash_data *hash_data)
{
    if (table == NULL || hash_data == NULL)
    {
        return ERROR_OF_TABLE_DESTRUCTOR;
    }
    if (table->hash_table == NULL || hash_data->buffer_for_data == NULL)
    {
        return ERROR_OF_TABLE_DESTRUCTOR;
    }
    for (size_t index = 0; index < (table->size_of_table); index++)
    {
        ((table->hash_table)[index])->key = 0;
        Errors error = list_destructor(((table->hash_table)[index])->list_element);
        if (error != NO_ERRORS)
        {
            return error;
        }
        free((table->hash_table)[index]);
        (table->hash_table)[index] = NULL;
    }
    free(table->hash_table);
    for (size_t index = 0; index < hash_data->size_of_buffer_for_data; index++)
    {
        free(hash_data->buffer_for_data[index]);
    }
    free(hash_data->buffer_for_data);
    hash_data->buffer_for_data = NULL;
    table->hash_table = NULL;
    return NO_ERRORS;
}

Errors hash_table_append(struct Table *table, const char *element)
{
    if (table == NULL || element == NULL)
    {
        return ERROR_OF_APPEND_TO_TABLE;
    }
    
    int hash = hash_count(element, table->size_of_table);
    //printf("hash = %d\n", hash);

    Errors error = NO_ERRORS;
    //printf("((table->hash_table)[hash])->key = %d\n", ((table->hash_table)[hash])->key);
    if (((table->hash_table)[hash])->key == 0)
    {
        error = list_append(((table->hash_table)[hash])->list_element, element);
        if (error != NO_ERRORS)
        {
            return ERROR_OF_APPEND_TO_TABLE;
        }
        ((table->hash_table)[hash])->key = hash;
        ((table->hash_table)[hash])->count_of_list_elements += 1;
    }
    else
    {
        if (strcasecmp(element, (((table->hash_table)[hash])->list_element)->data) != 0)
        {
            bool founded = false;
            list_append_collision(((table->hash_table)[hash])->list_element, element, &founded, NULL, hash);
            ((table->hash_table)[hash])->count_of_list_elements += 1;
        }
        else
        {
            ((((table->hash_table)[hash])->list_element)->frequency)++;
        }
    }
    return error;
}



Errors hash_table_print(struct Table *table)
{
    if (table == NULL)
    {
        return ERROR_OF_PRINT_TABLE;
    }
    if (table->hash_table == NULL)
    {
        return ERROR_OF_PRINT_TABLE;
    }
    for (size_t index = 0; index < (table->size_of_table); index++)
    {
        Errors error = list_print(((table->hash_table)[index])->list_element);
        if (error != NO_ERRORS)
        {
            return error;
        }
    }
    return NO_ERRORS;
}