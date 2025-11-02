#include "../include/select.h"
#include "../include/str_split.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool evaluate_where_condition(Table *table, char *condition, List *row_data) {
    List *col_and_var = str_split(condition, " = ");

    if (col_and_var->size < 2) {
        free_list(col_and_var);
        return false;
    }

    size_t idx = get_column_index(table, (char*)get_element_at(col_and_var, 0));

    bool result = (idx != (size_t)-1 && strcmp((char*)get_element_at(row_data, idx), (char*)get_element_at(col_and_var, 1)) == 0);

    free_list(col_and_var);
    return result;
}

void Select(DataBase *db, SQLParsedCommand *parsed_command) {
    List *format[parsed_command->columns->size];

    for (size_t i = 0; i < parsed_command->columns->size; i++) {
        format[i] = new_list();
    }

    if (parsed_command->condition) {
        List *and_condition = str_split(parsed_command->condition, " AND ");

        for (size_t i = 0; i < parsed_command->columns->size; i++) {
            List *table_and_column = str_split(get_element_at(parsed_command->columns, i), ".");

            if (elemin_list(parsed_command->tables, get_element_at(table_and_column, 0))) {
                Table *table = get_table(db, get_element_at(table_and_column, 0));

                csv_reader(table, db->name);

                size_t idx = get_column_index(table, get_element_at(table_and_column, 1));

                for (size_t j = 0; j < table->row_count; j++) {
                    List *row_data = get_row_in_table(table, j);

                    bool should_include = false;

                    List *or_condition;
                    bool and_result = true;

                    for (size_t k = 0; k < and_condition->size; k++) {
                        or_condition = str_split((char*)get_element_at(and_condition, k), " OR ");

                        bool or_result = false;
                        for (size_t l = 0; l < or_condition->size; l++) {
                            or_result = evaluate_where_condition(table, (char*)get_element_at(or_condition, l), row_data);
                            if (or_result) {
                                break;
                            }
                        }

                        if (!or_result) {
                            and_result = false;
                            break;
                        }

                        free_list(or_condition);
                    }

                    if (and_result) {
                        should_include = true;
                    }

                    if (should_include) {
                        push_back(format[i], get_element_at(row_data, idx));
                    }

                    free_list(row_data);
                }

                free_table_data(table);
            }
            free_list(table_and_column);
        }

        free_list(and_condition);
    } else {
        for (size_t i = 0; i < parsed_command->columns->size; i++) {
            List *table_and_column = str_split(get_element_at(parsed_command->columns, i), ".");

            if (elemin_list(parsed_command->tables, get_element_at(table_and_column, 0))) {
                Table *table = get_table(db, get_element_at(table_and_column, 0));

                csv_reader(table, db->name);

                size_t idx = get_column_index(table, get_element_at(table_and_column, 1));

                for (size_t j = 0; j < table->row_count; j++) {
                    List *row_data = get_row_in_table(table, j);

                    push_back(format[i], get_element_at(row_data, idx));

                    free_list(row_data);
                }

                free_table_data(table);
            }
            free_list(table_and_column);
        }
    }

    for (size_t j = 0; j < format[0]->size; j++) {
        for (size_t i = 0; i < parsed_command->columns->size; i++) {
            printf("%s ", (char*)get_element_at(format[i], j));
        }
        printf("\n");
    }

    for (size_t i = 0; i < parsed_command->columns->size; i++) {
        free_list(format[i]);
    }
}