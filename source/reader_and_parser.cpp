#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "reader_and_parser.h"

static Errors_of_reader process_file_for_data(char *buffer, size_t size_of_file, struct Hash_data *hash_data);
static size_t get_size_of_file(FILE *file_pointer);

static size_t get_size_of_file(FILE *file_pointer)
{
    fseek(file_pointer, 0L, SEEK_END);
    size_t size_of_file = (size_t)ftell(file_pointer);
    fseek(file_pointer, 0L, SEEK_SET);
    return size_of_file;
}

Errors_of_reader get_data_for_table(struct Hash_data *hash_data, FILE *file_pointer)
{
    if (hash_data == NULL)
    {
        return ERROR_OF_PROCESSING_FILE;
    }
    if (file_pointer == NULL)
    {
        return ERROR_OF_OPEN_FILE;
    }
    size_t size_of_file = get_size_of_file(file_pointer);
    char *buffer = (char *)calloc(size_of_file, sizeof(char));
    if (buffer == NULL)
    {
        return ERROR_OF_PROCESSING_FILE;
    }
    size_t result_of_reading = fread(buffer, sizeof(char), size_of_file, file_pointer);
    fclose(file_pointer);
    if (result_of_reading != size_of_file)
    {
        return ERROR_OF_READING_FROM_FILE;
    }
    Errors_of_reader error = process_file_for_data(buffer, size_of_file, hash_data);
    free(buffer);
    if (error != NO_READER_ERRORS)
    {
        return error;
    }
    return NO_READER_ERRORS;
}

static Errors_of_reader process_file_for_data(char *buffer, size_t size_of_file, struct Hash_data *hash_data)
{
    if (buffer == NULL || hash_data == NULL)
    {
        return ERROR_OF_PROCESSING_FILE;
    }
    hash_data->buffer_for_data = (char **) calloc (size_of_file, sizeof(char *));
    if (hash_data->buffer_for_data == NULL)
    {
        return ERROR_OF_PROCESSING_FILE;
    }
    
    size_t index = 0;
    for (size_t id = 0; id < size_of_file; id++)
    {
        char str[100] = {0};
        size_t i = 0;
        while (id < size_of_file && isalnum(buffer[id]))
        {
            str[i] = tolower(buffer[id]);
            i++;
            id++;
        }
        str[i] = '\0';
        size_t len = i;
        if (len != 0)
        {
            (hash_data->buffer_for_data)[index] = (char *) calloc (len + 1, sizeof(char));
            if ((hash_data->buffer_for_data)[index] == NULL)
            {
                return ERROR_OF_PROCESSING_FILE;
            }
            memcpy((hash_data->buffer_for_data)[index], str, len + 1);
            index++;
        }
    }
    hash_data->size_of_buffer_for_data = index;
    return NO_READER_ERRORS;
}