#include <stdio.h>

#include "../include/insert.h"
#include "../include/sql_parsed_command.h"
#include "../include/str_split.h"


void insert(DataBase *db, SQLParsedCommand *parsed_command) {
    Table *table = get_table(db, get_element_at(parsed_command->tables, 0));
    if (!table) {
        printf("Table '%s' notfound!\n", get_element_at(parsed_command->tables, 0));
        return;
    }
    
    csv_reader(table, db->name);
    add_data_to_table(table, parsed_command->values);
    csv_write(table, db->name);

    free_table_data(table);
}