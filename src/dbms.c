#include "../include/dbms.h"
#include "../include/csv_writer.h"

void insert(Table *table, char **data) {
    csv_reader(table);
    add_data_to_table(table, data);
    csv_write(table);

    free_table_data(table);
}