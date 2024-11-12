#include "../include/csv_reader.h"
#include "../include/str_split.h"

void csv_reader(Table *table, char *db_name) {
    char filepath[1024];
    snprintf(filepath, sizeof(filepath), "%s/%s/1.csv", db_name, table->table_name);

    FILE *file = fopen(filepath, "r");

    char line[1024];

    fgets(line, sizeof(line), file);

    while (fgets(line, sizeof(line), file)) {

        line[strcspn(line, "\n")] = '\0';

        List *values = str_split(line, ",");
        add_data_to_table(table, values);

        free_list(values);
    }

    fclose(file);
}