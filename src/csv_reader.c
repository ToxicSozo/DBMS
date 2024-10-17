#include "../include/csv_reader.h"

void csv_reader(Table *table) {
    FILE *file = fopen("Схема 1/таблица1/1.csv", "r");

    char line[1024];

    fgets(line, sizeof(line), file);

    while (fgets(line, sizeof(line), file)) {

        line[strcspn(line, "\n")] = '\0';

        char **elements = (char**)malloc(table->column_count * sizeof(char*));

        if (elements == NULL) {
            perror("Ошибка выделения памяти");
            fclose(file);
            return;
        }

        char *token = strtok(line, ",");
        int idx = 0;

        while (token != NULL && idx < table->column_count) {
            elements[idx] = strdup(token);

            token = strtok(NULL, ",");
            idx++;
        }

        add_data_to_table(table, elements);

        for (int i = 0; i < idx; i++) {
            free(elements[i]);
        }
        free(elements);
    }
    fclose(file);
}