#include "../include/dbms.h"
#include "../include/csv_writer.h"

void insert(Table *table, char **data) {
    csv_reader(table);
    add_data_to_table(table, data);
    csv_write(table);

    free_table_data(table);
}

void delete(Table *table) {
    csv_reader(table);
    free_table_data(table);
    csv_write(table);
}

void delete_where(Table *table, const char *column_name, const char *value) {
    csv_reader(table);

    int idx = get_column_index(table, column_name);

    if (idx == -1) {
        printf("Столбец '%s' не найден в таблице '%s'\n", column_name, table->table_name);
        return;
    }

    for (size_t i = 0; i < table->row_count; i++) {
        DataNode *current = table->columns[idx].data;
        
        for (size_t k = 0; k < i && current != NULL; k++) {
            current = current->next;
        }

        if (current != NULL && strcmp(current->data, value) == 0) {
            delete_row(table, i);

            i--;
        }
    }
        
    csv_write(table);

    free_table_data(table);
}