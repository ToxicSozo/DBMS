#include "../include/csv_reader.h"

void csv_reader(DataBase *db) {
    FILE *file = fopen("Схема 1/таблица1/1.csv", "r");
    if (file == NULL) {
        perror("Ошибка открытия файла");
        return;
    }

    char line[1024];

    fgets(line, sizeof(line), file);

    while (fgets(line, sizeof(line), file)) {

        line[strcspn(line, "\n")] = '\0';

        char **elements = (char**)malloc(db->tables[0].column_count * sizeof(char*));
        if (elements == NULL) {
            perror("Ошибка выделения памяти");
            fclose(file);
            return;
        }

        char *token = strtok(line, ",");
        int idx = 0;

        while (token != NULL && idx < db->tables[0].column_count) {
            elements[idx] = strdup(token);
            if (elements[idx] == NULL) {
                perror("Ошибка выделения памяти");

                for (int i = 0; i < idx; i++) {
                    free(elements[i]);
                }
                free(elements);
                fclose(file);
                return;
            }
            token = strtok(NULL, ",");
            idx++;
        }

        add_data_to_table(&db->tables[0], elements);

        for (int i = 0; i < idx; i++) {
            free(elements[i]);
        }
        free(elements);
    }

    fclose(file);
}