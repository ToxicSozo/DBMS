#include <stdio.h>
#include <string.h>

#include "../include/delete.h"
#include "../include/sql_parsed_command.h"
#include "../include/str_split.h"

void delete(DataBase *db, SQLParsedCommand *parsed_command) {
    Table *table = get_table(db, parsed_command->command_data.insert_data.table_name);
    if (!table) {
        printf("Table '%s' notfound!\n", parsed_command->command_data.insert_data.table_name);
        return;
    }

    if(parsed_command->command_data.delete_data.condition) {
        const char *delim[] = {" AND ", " OR "};

        char **condition_tokens = str_split(parsed_command->command_data.delete_data.condition, 
                                            delim, 2);
        
        csv_reader(table, db->name);
        for (size_t i = 0; i < table->row_count; i++) {
            char **row_data = get_row_in_table(table, i);

            for(size_t j = 0; condition_tokens[j]; j++) {
                const char *delim[] = {" = "};
                char **col_and_var = str_split(condition_tokens[j], delim, 1);

                size_t idx = get_column_index(table, col_and_var[0]);

                if(idx != -1 && strcmp(row_data[idx], col_and_var[1]) == 0) delete_row(table, i);

                free_split(col_and_var);
            }

            for(size_t k = 0; row_data[k]; k++) {
                free(row_data[k]);
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