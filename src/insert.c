#include "insert.h"

void insert(DataBase *db, char *buffer) {
    char *table_name = NULL;

    char *token = strtok(buffer, " ");
    if (token && strcmp(token, "INSERT") == 0) {
        token = strtok(NULL, " ");
        if (token && strcmp(token, "INTO") == 0) {
            table_name = strtok(NULL, " ");
        } else {
            printf("Invalid command syntax!\n");
            return;
        }
    } else {
        printf("Invalid command syntax!\n");
        return;
    }

    Table *table = get_table(db, table_name);
    if (!table) {
        printf("Table '%s' not found!\n", table_name);
        return;
    }

    token = strtok(NULL, " ");
    if (token && strcmp(token, "VALUES") == 0) {
        char *values[table->column_count];

        for (size_t i = 0; i < table->column_count; i++) {
            token = strtok(NULL, ", ");
            if (token) {
                values[i] = strdup(token);
            } else {
                printf("Not enough values provided for table '%s'.\n", table_name);
                return;
            }
        }

        if (strtok(NULL, " ") != NULL) {
            printf("Too many values provided!\n");
            return;
        }

        csv_reader(table);
        add_data_to_table(table, values);
        csv_write(table);

        for (size_t i = 0; i < table->column_count; i++) {
            free(values[i]);
        }
        free_table_data(table);
    } 
    
    else {
        printf("Invalid command syntax, VALUES keyword missing!\n");
    }
}