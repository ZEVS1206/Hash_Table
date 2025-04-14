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
    

    for (size_t index = 0; index < table->size_of_table; index++)
    {
        fprintf(file_pointer, "box%d "
            "[shape = record,"
            " label = \"{<node_id>index = %d|<node_adr>address = %p|<node_k>key = %d|<node_lad>list_address = %p}}\"];\n",
            index, index, (table->hash_table)[index], ((table->hash_table)[index])->key, ((table->hash_table)[index])->list_element);
    }
    return NO_ERRORS;
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
    for (size_t index = 1; index < table->size_of_table; index++)
    {
        fprintf(file_pointer, "box%d->box%d [color=red];\n", index - 1, index);
    }
    return NO_ERRORS;

}


static Errors create_command_for_console(const char *file_in_name, const char *file_out_name)
{
    if (file_in_name == NULL || file_out_name == NULL)
    {
        return ERROR_OF_DUMP;
    }

    char command_for_console[100] = "";
    snprintf(command_for_console, 100, "sudo dot -Tpng %s -o %s.png", file_in_name, file_out_name);
    printf("command for console = %s\n", command_for_console);
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