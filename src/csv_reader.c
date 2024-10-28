#include "../include/csv_reader.h"

void csv_reader(Table *table, char *db_name) {
    char filepath[1024];
    snprintf(filepath, sizeof(filepath), "%s/%s/1.csv", db_name, table->table_name);

    FILE *file = fopen(filepath, "r");

    char line[1024];

    fgets(line, sizeof(line), file);

    while (fgets(line, sizeof(line), file)) {

        line[strcspn(line, "\n")] = '\0';

        char *elements[table->column_count];
        char *token = strtok(line, ",");
        int idx = 0;

        while (token != NULL && idx < table->column_count) {
            elements[idx] = token;

            token = strtok(NULL, ",");
            idx++;
        }

        add_data_to_table(table, elements);
    }

    fclose(file);
}