#ifndef READER_AND_PARSER_H
#define READER_AND_PARSER_H

struct Hash_data
{
    char **buffer_for_data;
    size_t size_of_buffer_for_data;
};

enum Errors_of_reader
{
    NO_READER_ERRORS             = 0,
    ERROR_OF_OPEN_FILE           = 1,
    ERROR_OF_READING_FROM_FILE   = 2,
    ERROR_OF_CREATING_OUT_FILE   = 3,
    ERROR_OF_PROCESSING_FILE     = 4
};

Errors_of_reader get_data_for_table(struct Hash_data *hash_data, FILE *file_pointer);

#endif