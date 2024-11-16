#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "../include/delete.h"
#include "../include/sql_parsed_command.h"
#include "../include/str_split.h"

bool evaluate_condition(Table *table, char *condition, List *row_data) {
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

void delete(DataBase *db, SQLParsedCommand *parsed_command) {
    Table *table = get_table(db, get_element_at(parsed_command->tables, 0));
    if (!table) {
        printf("Table '%s' notfound!\n", get_element_at(parsed_command->tables, 0));
        return;
    }

    if (parsed_command->condition) {
        List *and_condition = str_split(parsed_command->condition, " AND ");

        csv_reader(table, db->name);
        for (size_t i = 0; i < table->row_count; i++) {
            List *row_data = get_row_in_table(table, i);
            
            bool should_delete = false;

            List *or_condition;
            bool and_result = true;

            for (size_t j = 0; j < and_condition->size; j++) {
                or_condition = str_split((char*)get_element_at(and_condition, j), " OR ");

                bool or_result = false;
                for (size_t k = 0; k < or_condition->size; k++) {
                    or_result = evaluate_condition(table, (char*)get_element_at(or_condition, k), row_data);
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
                should_delete = true;
            }

            if (should_delete) {
                delete_row(table, i);
                i--;
            }

            free_list(row_data);
        }
        
        csv_write(table, db->name);
        free_table_data(table);
        free_list(and_condition);

    } 
    
    else {
        csv_reader(table, db->name);
        free_table_data(table);
        csv_write(table, db->name);
    }
}