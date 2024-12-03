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

void parse_columns_array(const char** json, Table* table) {
    skip_whitespace(json);
    if (**json != '[') return;
    (*json)++;
    while (**json != ']') {
        skip_whitespace(json);
        if (**json == '"') {
            char* column_name = parse_string(json);
            add_column_to_table(table, column_name);
            free(column_name);
        }
        skip_whitespace(json);
        if (**json == ',') (*json)++;
    }
    (*json)++;
}

void parse_tables_object(const char** json, DataBase *db) {
    skip_whitespace(json);

    if (**json != '{') return;

    (*json)++;

    while (**json != '}') {
        skip_whitespace(json);
        if (**json == '"') {
            char* table_name = parse_string(json);

            skip_whitespace(json);

            if (**json == ':') {
                (*json)++;
                add_table_to_database(db, table_name);

                Table* table = &db->tables[db->table_count - 1];
                
                parse_columns_array(json, table);
            }
            free(table_name);
        }
        skip_whitespace(json);

        if (**json == ',') (*json)++;
    }

    (*json)++;
}

DataBase* parse_json(const char *json) {
    DataBase *db = malloc(sizeof(DataBase));

    skip_whitespace(&json);
    if (*json != '{') {
        free(db);
        return NULL;
    }

    json++;

    while (*json != '}') {
        skip_whitespace(&json);

        if (*json == '"') {
            char *key = parse_string(&json);
            skip_whitespace(&json);

            if (*json == ':') {
                json++;

                if (strcmp(key, "name") == 0) {
                    char *name_database = parse_string(&json);
                    db = create_database(name_database, db);
                    free(name_database);
                } else if (strcmp(key, "tuples_limit") == 0) {
                    db->tuples_limit = parse_number(&json);
                } else if (strcmp(key, "structure") == 0) {
                    parse_tables_object(&json, db);
                }

                free(key);
            } 
        }
        
        skip_whitespace(&json);

        if (*json == ',') {
            json++;
        }
    }

    return db;
}