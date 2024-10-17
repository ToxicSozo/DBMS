#include "../include/csv_writer.h"

void csv_write(Table *table) {
    FILE *file = fopen("Схема 1/таблица1/1.csv", "w");

    if (file == NULL) {
        perror("Failed to open file");
        return;
    }

    // Запись заголовков столбцов
    for(size_t i = 0; i < table->column_count; i++) {
        if(i > 0) fprintf(file, ",");
        fprintf(file, "%s", table->columns[i].name);
    }
    fprintf(file, "\n");

    // Запись данных строк
    for(size_t i = 0; i < table->row_count; i++) {
        for(size_t j = 0; j < table->column_count; j++) {
            DataNode *current = table->columns[j].data;
            
            // Перемещаемся к нужной строке
            for(size_t k = 0; k < i && current != NULL; k++) {
                current = current->next;
            }

            if(j > 0) fprintf(file, ",");

            if (current != NULL) {
                fprintf(file, "%s", current->data);
            } else {
                fprintf(file, "");
            }
        }
        fprintf(file, "\n");
    }

    fclose(file);
}