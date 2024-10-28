#include "../include/delete.h"

#include <stdio.h>
#include <string.h>

// Функция для удаления лишних пробелов
char *trim_whitespace(char *str) {
    while (*str == ' ') str++;  // Убираем начальные пробелы
    char *end = str + strlen(str) - 1;
    while (end > str && *end == ' ') end--;  // Убираем конечные пробелы
    *(end + 1) = '\0';  // Завершаем строку
    return str;
}

// Функция для проверки одного условия
int check_condition(Table *table, size_t row_index, char *condition) {
    char column_name[50];
    char value[50];
    char quoted_value[50];

    condition = trim_whitespace(condition);

    if (sscanf(condition, "%49[^ ] = '%49[^']'", column_name, quoted_value) == 2) {
        int col_index = get_column_index(table, column_name);
        if (col_index == -1) {
            printf("Column '%s' not found\n", column_name);
            return 0;
        }

        return strcmp(table->columns[col_index].data[row_index].data, quoted_value) == 0;
    }

    if (sscanf(condition, "%49[^ ] = %49s", column_name, value) == 2) {
        int col_index = get_column_index(table, column_name);
        if (col_index == -1) {
            printf("Column '%s' not found\n", column_name);
            return 0;
        }

        return strcmp(table->columns[col_index].data[row_index].data, value) == 0;
    }

    printf("Condition '%s' is invalid\n", condition);
    return 0;
}

int evaluate_conditions(Table *table, size_t row_index, char *conditions) {
    char *or_ptr = strstr(conditions, "OR");
    if (or_ptr != NULL) {
        char left_condition[256], right_condition[256];
        strncpy(left_condition, conditions, or_ptr - conditions);
        left_condition[or_ptr - conditions] = '\0';
        strcpy(right_condition, or_ptr + 2);

        trim_whitespace(left_condition);
        trim_whitespace(right_condition);

        return evaluate_conditions(table, row_index, left_condition) || 
               evaluate_conditions(table, row_index, right_condition);
    }

    char *and_ptr = strstr(conditions, "AND");
    if (and_ptr != NULL) {
        char left_condition[256], right_condition[256];
        strncpy(left_condition, conditions, and_ptr - conditions);
        left_condition[and_ptr - conditions] = '\0';
        strcpy(right_condition, and_ptr + 3);

        trim_whitespace(left_condition);
        trim_whitespace(right_condition);

        return evaluate_conditions(table, row_index, left_condition) && 
               evaluate_conditions(table, row_index, right_condition);
    }

    trim_whitespace(conditions);
    return check_condition(table, row_index, conditions);
}

void delete(DataBase *db, char *buffer) {
    char *table_name = NULL;

    char *token = strtok(buffer, " ");
    if (token && strcmp(token, "DELETE") == 0) {
        token = strtok(NULL, " ");
        if (token && strcmp(token, "FROM") == 0) {
            table_name = strtok(NULL, " ");

            if (table_name == NULL) {
                printf("Invalid syntax: missing table name!\n");
                return;
            }

            Table *table = get_table(db, table_name);
            if (!table) {
                printf("Table '%s' not found!\n", table_name);
                return;
            }

            token = strtok(NULL, " ");
            if (token && strcmp(token, "WHERE") == 0) {
                char *condition = strtok(NULL, "");
                if (condition) {
                    csv_reader(table, db->name);
                    for (size_t i = 0; i < table->row_count; i++) {
                        if (evaluate_conditions(table, i, condition)) {
                            printf("Deleting row %zu from table '%s'\n", i, table->table_name);
                            delete_row(table, i);
                            i--;
                        }
                    }
                    csv_write(table, db->name);
                    free_table_data(table);
                } 
                
                else {
                    printf("Invalid syntax: missing condition after WHERE!\n");
                    return;
                }
            } 
            
            else if (!token) {
                csv_reader(table, db->name);
                free_table_data(table);
                csv_write(table, db->name);
            }
        } 
        
        else {
            printf("Invalid command syntax: missing FROM!\n");
        }
    } 
    
    else {
        printf("Invalid command syntax!\n");
    }
}