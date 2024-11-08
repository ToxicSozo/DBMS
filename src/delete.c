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
        const char *delim[] = {"AND", "OR"};

        char **condition_tokens = str_split(parsed_command->command_data.delete_data.condition, 
                                            delim, 2);

        for(size_t i = 0; condition_tokens[i]; i++) {
            printf("%s\n", condition_tokens[i]);
        }

        csv_reader(table, db->name);
        for (size_t i = 0; i < table->row_count; i++) {
            char **row_data = get_row_in_table(table, i);

        }
    }

    else {
        csv_reader(table, db->name);
        free_table_data(table);
        csv_write(table, db->name);
    }
}