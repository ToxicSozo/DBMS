#include "../include/select.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stdio.h>

void print_format(char ***format, size_t column_count, size_t *row_counts) {
    for (size_t row = 0; row < row_counts[0]; row++) {
        for (size_t col = 0; col < column_count; col++) {
            printf("%s\t", format[col][row] ? format[col][row] : "NULL");
        }
        printf("\n");
    }
}

char *strim_whitespace(char *str) {
    while (*str == ' ') str++;
    char *end = str + strlen(str) - 1;
    while (end > str && *end == ' ') end--;
    *(end + 1) = '\0';
    return str;
}

char* get_column_data(Table *table, const char *column_name, size_t row_index) {
    int col_index = get_column_index(table, column_name);
    if (col_index == -1) {
        printf("Column '%s' not found in table '%s'!\n", column_name, table->table_name);
        return NULL;
    }

    Column *column = &table->columns[col_index];
    DataNode *current_node = column->data;

    for (size_t i = 0; i < row_index; i++) {
        if (current_node == NULL) {
            printf("Row index out of bounds for column '%s'!\n", column_name);
            return NULL;
        }
        current_node = current_node->next;
    }

    if (current_node)  return current_node->data;

    printf("No data found for row index %zu in column '%s'!\n", row_index, column_name);
    return NULL;
}

void Select(DataBase *db, char *buffer) {
    char **table_names = NULL;
    char **column_names = NULL;

    size_t column_count = 0;
    size_t table_count = 0;

    char *token = strtok(buffer, " ");
    if (token && strcmp(token, "SELECT") == 0) {
        char *columns_part = strtok(NULL, "FROM");
        if (columns_part) {
            char *column_token = strtok(columns_part, ",");

            while (column_token) {
                column_token = strim_whitespace(column_token);

                char *dot_pos = strchr(column_token, '.');
                if (dot_pos) {
                    *dot_pos = '\0';
                    char *table_name = column_token;
                    char *column_name = dot_pos + 1;

                    table_names = realloc(table_names, (table_count + 1) * sizeof(char *));
                    table_names[table_count] = strdup(table_name);
                    table_count++;

                    column_names = realloc(column_names, (column_count + 1) * sizeof(char *));
                    column_names[column_count] = strdup(column_name);
                    column_count++;
                } 
                
                else {
                    printf("Invalid column format: %s\n", column_token);
                }

                column_token = strtok(NULL, ",");
            }
        }
    }

    char ***format = malloc(column_count * sizeof(char **));
    size_t *row_counts = malloc(column_count * sizeof(size_t));

    for (size_t i = 0; i < column_count; i++) {
        Table *table = get_table(db, table_names[i]);
        if (!table) {
            printf("table %s not found\n", table_names[i]);
            return;
        }

        csv_reader(table, db->name);
        row_counts[i] = table->row_count;

        format[i] = malloc(table->row_count * sizeof(char *));
        
        for (size_t row = 0; row < table->row_count; row++) {
            format[i][row] = strdup(get_column_data(table, column_names[i], row));
        }

        free_table_data(table);
    }

    print_format(format, column_count, row_counts);
}
