#include "../include/json_parser.h"

void skip_whitespace(const char **json) {
    while (isspace(**json)) {
        (*json)++;
    }
}

char* parse_string(const char **json) {
    skip_whitespace(json);

    if (**json != '"') return NULL;

    (*json)++;

    const char *start = *json;

    while (**json != '"') (*json)++;

    size_t len = *json - start;

    char *str = calloc(len + 1, sizeof(char));
    strncpy(str, start, len);
    str[len] = '\0';

    (*json)++;

    return str;
}

int parse_number(const char **json) {
    skip_whitespace(json);

    int number = 0;
    
    while (isdigit(**json)) {
        number = number * 10 + (**json - '0');
        (*json)++;
    }

    return number;
}

ColumnsList* parse_columns_array(const char** json) {
    skip_whitespace(json);
    if (**json != '[') {
        return NULL;
    }
    (*json)++;
    ColumnsList* columns = malloc(sizeof(ColumnsList));
    columns->count = 0;
    columns->columns = NULL;
    while (**json != ']') {
        skip_whitespace(json);
        if (**json == '"') {
            char* column = parse_string(json);
            columns->count++;
            columns->columns = (char**)realloc(columns->columns, columns->count * sizeof(char*));
            columns->columns[columns->count - 1] = column;
        }
        skip_whitespace(json);
        if (**json == ',') {
            (*json)++;
        }
    }
    (*json)++;
    return columns;
}

TableEntry* parse_tables_object(const char** json, size_t* table_count) {
    skip_whitespace(json);

    if (**json != '{') return NULL;
    (*json)++;

    TableEntry* tables = NULL;
    *table_count = 0;

    while (**json != '}') {
        skip_whitespace(json);
        if (**json == '"') {
            char* table_name = parse_string(json);
            
            skip_whitespace(json);
            if (**json == ':') {
                (*json)++;

                ColumnsList* columns = parse_columns_array(json);

                (*table_count)++;

                tables = (TableEntry*)realloc(tables, *table_count * sizeof(TableEntry));
                tables[*table_count - 1].table_name = table_name;
                tables[*table_count - 1].columns = columns;
            }
        }
        skip_whitespace(json);
        if (**json == ',') {
            (*json)++;
        }
    }
    (*json)++;
    return tables;
}

DataBase* parse_json(const char *json) {
    DataBase *schema = malloc(sizeof(DataBase));

    skip_whitespace(&json);
    if(*json != '{') return NULL;

    json++;

    while (*json != '}') {
        skip_whitespace(&json);

        if (*json == '"') {
            char *key = parse_string(&json);
            skip_whitespace(&json);

            if (*json == ':') {
                json++;

                if (strcmp(key, "name") == 0) {
                    schema->name = parse_string(&json);
                } else if (strcmp(key, "tuples_limit") == 0) {
                    schema->tuples_limit = parse_number(&json);
                } else if (strcmp(key, "structure") == 0) {
                    schema->tables = parse_tables_object(&json, &schema->table_count);
                }
            }
            free(key);
        }
        skip_whitespace(&json);

        if (*json == ',') {
            json++;
        }
    }

    return schema;
}