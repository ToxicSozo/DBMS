#include "../include/database.h"
#include "../include/database_directory.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

// Функция для создания директории
int create_directory(const char* path) {
    if (mkdir(path, 0777) == -1) {
        perror("Ошибка при создании директории");
        return -1;
    }
    return 0;
}

// Функция для создания файла 1.csv в директории и записи имен колонок
int create_csv_file(const char* path, ColumnsList* columns) {
    char file_path[256];
    snprintf(file_path, sizeof(file_path), "%s/1.csv", path);

    FILE* file = fopen(file_path, "w");
    if (!file) {
        perror("Ошибка при создании файла");
        return -1;
    }

    // Запись имен колонок в файл
    for (size_t i = 0; i < columns->count; i++) {
        fprintf(file, "%s", columns->columns[i]);
        if (i < columns->count - 1) {
            fprintf(file, ",");
        }
    }
    fprintf(file, "\n");

    fclose(file);
    return 0;
}

// Функция для создания структуры директорий и файлов
void build_database_file_system(DataBase* schema) {
    // Создание директории с названием схемы
    if (create_directory(schema->name) == -1) {
        return;
    }

    // Создание поддиректорий для каждой таблицы и файла 1.csv в каждой поддиректории
    for (size_t i = 0; i < schema->table_count; i++) {
        char table_path[256];
        snprintf(table_path, sizeof(table_path), "%s/%s", schema->name, schema->tables[i].table_name);

        if (create_directory(table_path) == -1) {
            continue;
        }

        if (create_csv_file(table_path, schema->tables[i].columns) == -1) {
            continue;
        }
    }
}