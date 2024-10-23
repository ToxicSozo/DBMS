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
    char quoted_value[50];  // Для значений в кавычках

    condition = trim_whitespace(condition);  // Убираем пробелы из условия

    // Проверка на наличие значения в кавычках
    if (sscanf(condition, "%49[^ ] = '%49[^']'", column_name, quoted_value) == 2) {
        int col_index = get_column_index(table, column_name);
        if (col_index == -1) {
            printf("Column '%s' not found\n", column_name);
            return 0;
        }

        // Сравниваем значение из таблицы с тем, что внутри кавычек
        return strcmp(table->columns[col_index].data[row_index].data, quoted_value) == 0;
    }

    // Если значения нет в кавычках, проверяем без них
    if (sscanf(condition, "%49[^ ] = %49s", column_name, value) == 2) {
        int col_index = get_column_index(table, column_name);
        if (col_index == -1) {
            printf("Column '%s' not found\n", column_name);
            return 0;
        }

        return strcmp(table->columns[col_index].data[row_index].data, value) == 0;
    }

    printf("Condition '%s' is invalid\n", condition);  // Отладочный вывод
    return 0;
}

// Рекурсивная функция для оценки всех условий с учётом AND/OR
int evaluate_conditions(Table *table, size_t row_index, char *conditions) {
    char *or_ptr = strstr(conditions, "OR");
    if (or_ptr != NULL) {
        // Разделяем по оператору OR, проверяем каждую часть отдельно
        char left_condition[256], right_condition[256];
        strncpy(left_condition, conditions, or_ptr - conditions);  // Левая часть до OR
        left_condition[or_ptr - conditions] = '\0';
        strcpy(right_condition, or_ptr + 2);  // Правая часть после OR

        // Убираем пробелы из обеих частей
        trim_whitespace(left_condition);
        trim_whitespace(right_condition);

        // Оцениваем: если хоть одно условие истинно, возвращаем 1
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

// Основная функция удаления строк с условиями
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
                    csv_reader(table);
                    for (size_t i = 0; i < table->row_count; i++) {
                        if (evaluate_conditions(table, i, condition)) {
                            printf("Deleting row %zu from table '%s'\n", i, table->table_name);
                            delete_row(table, i);
                            i--;  // Уменьшаем индекс, так как строка удалена
                        }
                    }
                    csv_write(table);
                    free_table_data(table);
                } else {
                    printf("Invalid syntax: missing condition after WHERE!\n");
                    return;
                }
            } else if (!token) {
                csv_reader(table);
                free_table_data(table);
                csv_write(table);
            }
        } else {
            printf("Invalid command syntax: missing FROM!\n");
        }
    } else {
        printf("Invalid command syntax!\n");
    }
}