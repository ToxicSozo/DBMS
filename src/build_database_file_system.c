#include "../include/database.h"
#include "../include/build_database_file_system.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>

// Функция для создания директории
int create_directory(const char* path) {
    mode_t old_mask = umask(0);
    if (mkdir(path, 0777) == -1) {
        perror("Ошибка при создании директории");
        umask(old_mask);
        return -1;
    }
    umask(old_mask);
    return 0;
}

// Функция для проверки существования директории
int directory_exists(const char* path) {
    struct stat statbuf;
    return (stat(path, &statbuf) == 0 && S_ISDIR(statbuf.st_mode));
}

// Функция для создания файла 1.csv в директории и записи имен колонок
int create_csv_file(const char* path, Column *columns, size_t column_count) {
    char file_path[256];
    snprintf(file_path, sizeof(file_path), "%s/1.csv", path);

    FILE* file = fopen(file_path, "w");
    if (!file) {
        perror("Ошибка при создании файла");
        return -1;
    }

    // Запись имен колонок в файл
    for (size_t i = 0; i < column_count; i++) {
        if (columns[i].name) {
            fprintf(file, "%s", columns[i].name);
        } else {
            fprintf(file, "NULL");
        }
        if (i < column_count - 1) {
            fprintf(file, ",");
        }
    }
    fprintf(file, "\n");

    fclose(file);
    return 0;
}

// Функция для создания структуры директорий и файлов
void build_database_file_system(DataBase *schema) {
    // Проверка существования директории с названием схемы
    if (directory_exists(schema->name)) {
        printf("Файловая система базы данных '%s' уже существует.\n", schema->name);
        return;
    }

    // Создание директории с названием схемы
    if (create_directory(schema->name) == -1) {
        return;
    }

    // Создание поддиректорий для каждой таблицы и файла 1.csv в каждой поддиректории
    for (size_t i = 0; i < schema->table_count; i++) {
        char table_path[256];
        if (snprintf(table_path, sizeof(table_path), "%s/%s", schema->name, schema->tables[i].table_name) >= sizeof(table_path)) {
            fprintf(stderr, "Путь к таблице слишком длинный\n");
            continue;
        }

        if (create_directory(table_path) == -1) {
            continue;
        }

        if (create_csv_file(table_path, schema->tables[i].columns, schema->tables[i].column_count) == -1) {
            continue;
        }
    }
}

bool lock_table(const char *db_path, const char *table_name) {
    char lock_file[256];
    snprintf(lock_file, sizeof(lock_file), "%s/%s/%s_lock", db_path, table_name, table_name);

    int fd = open(lock_file, O_CREAT | O_EXCL, 0600); // Создать файл блокировки
    if (fd == -1) {
        if (errno == EEXIST) {
            printf("Таблица '%s' уже заблокирована другим клиентом.\n", table_name);
        } else {
            perror("Ошибка при создании файла блокировки");
        }
        return false;
    }

    close(fd);
    printf("Таблица '%s' успешно заблокирована. Файл: %s\n", table_name, lock_file);
    return true;
}

void unlock_table(const char *db_path, const char *table_name) {
    char lock_file[256];
    snprintf(lock_file, sizeof(lock_file), "%s/%s/%s_lock", db_path, table_name, table_name);

    if (unlink(lock_file) == -1) {
        perror("Ошибка при удалении файла блокировки");
    } else {
        printf("Таблица '%s' успешно разблокирована. Файл: %s\n", table_name, lock_file);
    }
}
