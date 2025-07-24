#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "functions_and_structures.h"
#include "hash_table_dump.h"

static Errors create_nodes_with_table_elements_in_dump(struct Table *table, FILE *file_pointer);
static Errors create_command_for_console(const char *file_in_name, const char *file_out_name);
static Errors create_connections_between_table_elements(struct Table *table, FILE *file_pointer);
static Errors check_command(const char *command, size_t size);
static Errors create_nodes_with_list_elements(struct List *list, FILE *file_pointer);
static Errors create_connections_between_list_elements(struct List *list, FILE *file_pointer);

static Errors create_nodes_with_list_elements(struct List *list, FILE *file_pointer)
{
    if (file_pointer == NULL)
    {
        return ERROR_OF_DUMP;
    }
    struct List *current = list;
    while (current != NULL)
    {
        if (current->frequency >= 1000)
        {
            current->frequency /= 1000;
        }
        else
        {
            current->frequency %= 1000;
        }
        fprintf(file_pointer, "box%p "
            "[shape = record,"
            " label = \"{<node_adr>address = %p}|{<node_d>data = %s|<node_f>frequency = %lu}\"];\n",
            current, current, current->data, current->frequency);
        current = current->next_element;
    }
    return NO_ERRORS;
}

static Errors create_connections_between_list_elements(struct List *list, FILE *file_pointer)
{
    if (file_pointer == NULL)
    {
        return ERROR_OF_DUMP;
    }
    struct List *next = list->next_element;
    struct List *current = list;
    while (next != NULL)
    {
        fprintf(file_pointer, "box%p:<node_adr>->box%p:<node_adr> [color=green];\n", current, next);
        next = next->next_element;
        current = current->next_element;
    }
    return NO_ERRORS;
}

static Errors create_nodes_with_table_elements_in_dump(struct Table *table, FILE *file_pointer)
{
    if (table == NULL || file_pointer == NULL)
    {
        return ERROR_OF_DUMP;
    }
    if (table->hash_table == NULL)
    {
        return ERROR_OF_DUMP;
    }
    Errors error = NO_ERRORS;

    for (size_t index = 0; index < table->size_of_table; index++)
    {
        fprintf(file_pointer, "box%lu "
            "[shape = record,"
            " label = \"{<node_id>index = %lu|<node_adr>address = %p|<node_k>key = %d|<node_lad>list_address = %p}\"];\n",
            index, index, (table->hash_table)[index], ((table->hash_table)[index])->key, ((table->hash_table)[index])->list_element);
        error = create_nodes_with_list_elements(((table->hash_table)[index])->list_element, file_pointer);
        if (error != NO_ERRORS)
        {
            return error;
        }
    }
    return error;
}

static Errors create_connections_between_table_elements(struct Table *table, FILE *file_pointer)
{
    if (table == NULL || file_pointer == NULL)
    {
        return ERROR_OF_DUMP;
    }
    if (table->hash_table == NULL)
    {
        return ERROR_OF_DUMP;
    }
    Errors error = NO_ERRORS;
    fprintf(file_pointer, "\nrankdir = \"LR\";\n");
    for (size_t index = 1; index < table->size_of_table; index++)
    {
        fprintf(file_pointer, "box%lu->box%lu [color=red];\n", index - 1, index);
        fprintf(file_pointer, "box%lu:<node_lad>->box%p:<node_adr> [color=green];\n", index - 1, 
            ((table->hash_table)[index - 1])->list_element);
        error = create_connections_between_list_elements(((table->hash_table)[index])->list_element, file_pointer);
        if (error != NO_ERRORS)
        {
            return error;
        }
    }
    fprintf(file_pointer, "box%lu:<node_lad>->box%p:<node_adr> [color=green];\n", table->size_of_table - 1, 
        ((table->hash_table)[table->size_of_table - 1])->list_element);
    fprintf(file_pointer, "\nrankdir = \"TB\";\n");
    return error;

}


static Errors create_command_for_console(const char *file_in_name, const char *file_out_name)
{
    if (file_in_name == NULL || file_out_name == NULL)
    {
        return ERROR_OF_DUMP;
    }

    char command_for_console[100] = "";
    snprintf(command_for_console, 100, "sudo dot -Tpng %s -o %s.png", file_in_name, file_out_name);
    //printf("command for console = %s\n", command_for_console);
    Errors error = check_command(command_for_console, 100);
    if (error != NO_ERRORS)
    {
        return error;
    }
    system(command_for_console);
    return NO_ERRORS;
}

static Errors check_command(const char *command, size_t size)
{
    char not_dangerous_symbols[] = {'-', '.', '/', '_', '\0'};
    size_t size_of_not_dangersous_symbols = sizeof(not_dangerous_symbols) / sizeof(char);
    for (size_t i = 0; i < size; i++)
    {
        char symbol = command[i];
        if (!isalpha(symbol) && !isspace(symbol))
        {
            bool is_not_bad_symbol = false;
            for (size_t j = 0; j < size_of_not_dangersous_symbols; j++)
            {
                if (symbol == not_dangerous_symbols[j])
                {
                    is_not_bad_symbol = true;
                    break;
                }
            }
            if (!is_not_bad_symbol)
            {
                return ERROR_OF_DANGEROUS_COMMAND;
            }
        }
    }
    return NO_ERRORS;
}


Errors graphic_dump(struct Table *table, char *operation)
{
    if (table == NULL || operation == NULL)
    {
        return ERROR_OF_DUMP;
    }
    const char file_name[] = "dump/dump.txt";
    FILE *file_pointer = fopen(file_name, "w");
    if (file_pointer == NULL)
    {
        return ERROR_OF_DUMP;
    }
    Errors error = NO_ERRORS;


    fprintf(file_pointer, "digraph Tree {\n");
    fprintf(file_pointer, "node [margin = \"0.01\"];\nrankdir = \"TB\";\n");

    char file_out_name[100] = "dump/";
    strncat(file_out_name, operation, 100);

    error = create_nodes_with_table_elements_in_dump(table, file_pointer);
    if (error != NO_ERRORS)
    {
        return error;
    }

    error = create_connections_between_table_elements(table, file_pointer);
    if (error != NO_ERRORS)
    {
        return error;
    }

    fprintf(file_pointer, "}\n");
    fclose(file_pointer);

    error = create_command_for_console(file_name, file_out_name);
    if (error != NO_ERRORS)
    {
        return error;
    }

    return NO_ERRORS;

}