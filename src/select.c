#include "../include/select.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

int select_check_condition(Table *table, size_t row_index, char *condition) {
    char column_name[50];
    char value[50];
    char quoted_value[50];

    condition = strim_whitespace(condition);

    if (sscanf(condition, "%49[^ ] = '%49[^']'", column_name, quoted_value) == 2) {
        int col_index = get_column_index(table, column_name);
        if (col_index == -1) {
            printf("Column '%s' not found\n", column_name);
            return 0;
        }

        return strcmp(get_column_data(table, column_name, row_index), quoted_value) == 0;
    }

    if (sscanf(condition, "%49[^ ] = %49s", column_name, value) == 2) {
        int col_index = get_column_index(table, column_name);
        if (col_index == -1) {
            printf("Column '%s' not found\n", column_name);
            return 0;
        }

        return strcmp(get_column_data(table, column_name, row_index), value) == 0;
    }

    printf("Condition '%s' is invalid\n", condition);
    return 0;
}

int select_evaluate_conditions(Table *table, size_t row_index, char *conditions) {
    char *or_ptr = strstr(conditions, "OR");
    if (or_ptr != NULL) {
        char left_condition[256], right_condition[256];
        strncpy(left_condition, conditions, or_ptr - conditions);
        left_condition[or_ptr - conditions] = '\0';
        strcpy(right_condition, or_ptr + 2);

        strim_whitespace(left_condition);
        strim_whitespace(right_condition);

        return select_evaluate_conditions(table, row_index, left_condition) || 
               select_evaluate_conditions(table, row_index, right_condition);
    }

    char *and_ptr = strstr(conditions, "AND");
    if (and_ptr != NULL) {
        char left_condition[256], right_condition[256];
        strncpy(left_condition, conditions, and_ptr - conditions);
        left_condition[and_ptr - conditions] = '\0';
        strcpy(right_condition, and_ptr + 3);

        strim_whitespace(left_condition);
        strim_whitespace(right_condition);

        return select_evaluate_conditions(table, row_index, left_condition) && 
               select_evaluate_conditions(table, row_index, right_condition);
    }

    strim_whitespace(conditions);
    return select_check_condition(table, row_index, conditions);
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

    // Handle WHERE clause
    char *where_clause = strstr(buffer, "WHERE");
    if (where_clause) {
        where_clause += strlen("WHERE");
        where_clause = strim_whitespace(where_clause);

        for (size_t i = 0; i < column_count; i++) {
            Table *table = get_table(db, table_names[i]);
            if (!table) {
                printf("table %s not found\n", table_names[i]);
                return;
            }

            csv_reader(table, db->name);

            for (size_t row = 0; row < table->row_count; row++) {
                if (!select_evaluate_conditions(table, row, where_clause)) {
                    for (size_t col = 0; col < column_count; col++) {
                        free(format[col][row]);
                        format[col][row] = NULL;
                    }
                }
            }

            free_table_data(table);
        }
    }

    print_format(format, column_count, row_counts);
}