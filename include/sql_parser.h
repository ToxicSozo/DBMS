#ifndef SQL_PARSER_H
#define SQL_PARSER_H

typedef struct {
    char **data_query;
    char **table_name;
    char **columns_name;
    char comand[10];
} QueryData;

#endif