#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "../include/delete.h"
#include "../include/sql_parsed_command.h"
#include "../include/str_split.h"

bool evaluate_condition(Table *table, char *condition, char **row_data) {
    char **col_and_var = str_split(condition, " ");

    if (!col_and_var || !col_and_var[0] || !col_and_var[1]) {
        return false;
    }

    size_t idx = get_column_index(table, col_and_var[0]);

    bool result = (idx != (size_t)-1 && strcmp(row_data[idx], col_and_var[1]) == 0);

    return result;
}

void delete(DataBase *db, SQLParsedCommand *parsed_command) {
    Table *table = get_table(db, parsed_command->command_data.insert_data.table_name);
    if (!table) {
        printf("Table '%s' not found!\n", parsed_command->command_data.insert_data.table_name);
        return;
    }

    if (parsed_command->command_data.delete_data.condition) {
        char **and_condition = str_split(parsed_command->command_data.delete_data.condition, " AND ");

        csv_reader(table, db->name);
        for (size_t i = 0; i < table->row_count; i++) {
            char **row_data = get_row_in_table(table, i);
            
            bool should_delete = false;

            char **or_condition;
            bool and_result = true;

            for (size_t j = 0; and_condition[j]; j++) {
                or_condition = str_split(and_condition[j], " OR ");

                bool or_result = false;
                for (size_t k = 0; or_condition[k]; k++) {
                    or_result = evaluate_condition(table, or_condition[k], row_data);
                    if (or_result) {
                        break;
                    }
                }

                if (!or_result) {
                    and_result = false;
                    break;
                }
            }

            if (and_result) {
                should_delete = true;
            }

            if (should_delete) {
                delete_row(table, i);
                i--;
            }

            for (size_t i = 0; row_data[i]; i++) {
                free(row_data[i]);
            }
            free(row_data);
        }
        
        csv_write(table, db->name);
        free_table_data(table);

    } 
    
    else {
        csv_reader(table, db->name);
        free_table_data(table);
        csv_write(table, db->name);
    }
}