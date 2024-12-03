#include "../include/csv_writer.h"

void csv_write(Table *table, char *db_name) {
    char filepath[1024];
    snprintf(filepath, sizeof(filepath), "%s/%s/1.csv", db_name, table->table_name);

    FILE *file = fopen(filepath, "w");

    size_t i, j, k;

    for(i = 0; i < table->column_count; i++) {
        if(i > 0) fputc(',', file);
        fputs(table->columns[i].name, file);
    }
    fputc('\n', file);

    DataNode *current_nodes[table->column_count];
    for (j = 0; j < table->column_count; j++) {
        current_nodes[j] = table->columns[j].data;
    }

    for (i = 0; i < table->row_count; i++) {
        for (j = 0; j < table->column_count; j++) {
            if (current_nodes[j]) {
                if (j > 0) fputc(',', file);
                fputs(current_nodes[j]->data, file);
                
                if (j == table->column_count - 1 && current_nodes[j]) fputc('\n', file);

                current_nodes[j] = current_nodes[j]->next;
            }
        }
    }

    fclose(file);
}