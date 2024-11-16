#include "../include/select.h"
#include "../include/str_split.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void Select(DataBase *db, SQLParsedCommand *parsed_command) {
    List *format[parsed_command->columns->size];

    if(parsed_command->condition) {
        
    }

    else {
        for (size_t i = 0; i < parsed_command->columns->size; i++) {
            format[i] = new_list();

            List *table_and_column = str_split(get_element_at(parsed_command->columns, i), ".");

            if(elemin_list(parsed_command->tables, get_element_at(table_and_column, 0))) {
                Table *table = get_table(db, get_element_at(table_and_column, 0));
                
                csv_reader(table, db->name);
                    
                size_t idx = get_column_index(table, get_element_at(table_and_column, 1));

                for(size_t j = 0; j < table->row_count; j++) {
                    List *row_data = get_row_in_table(table, j);
                    
                    push_back(format[i], get_element_at(row_data, idx));

                    free_list(row_data);
                }

                free_table_data(table);
            }
            free_list(table_and_column);
        }
    }
}