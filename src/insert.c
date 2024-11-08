#include <stdio.h>

#include "../include/insert.h"
#include "../include/sql_parsed_command.h"
#include "../include/str_split.h"


void insert(DataBase *db, SQLParsedCommand *parsed_command) {
    Table *table = get_table(db, parsed_command->command_data.insert_data.table_name);
    if (!table) {
        printf("Table '%s' notfound!\n", parsed_command->command_data.insert_data.table_name);
        return;
    }

    if (parsed_command->command_data.insert_data.value_count != table->column_count) {
        printf("Incorrect number of values provided for table '%s'.\n", parsed_command->command_data.insert_data.table_name);
        return;
    }

    csv_reader(table, db->name);
    add_data_to_table(table, parsed_command->command_data.insert_data.values);
    csv_write(table, db->name);

    free_table_data(table);
}