#include "../include/dbms.h"
#include "../include/csv_writer.h"

void insert(Table *table, char **data) {
    csv_reader(table);
    printf("1");
    add_data_to_table(table, data);
    printf("2");
    csv_write(table);
    printf("3");
}