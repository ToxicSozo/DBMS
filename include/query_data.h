#ifndef QUERY_DATA_H
#define QUERY_DATA_H

typedef struct {
    char **data_query;
    char **table_name;
    char **columns_name;
    char comand[10];
} QueryData;

#endif