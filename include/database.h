#ifndef DATABASE_H 
#define DATABASE_H    

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Структура для хранения списка колонок
typedef struct {
    char** columns;
    size_t count;
} ColumnsList;

// Структура для хранения пары ключ-значение (таблица-колонки)
typedef struct {
    char* table_name;
    ColumnsList* columns;
} TableEntry;

// Структура для хранения информации о схеме
typedef struct {
    char* name;
    int tuples_limit;
    TableEntry *tables;
    size_t table_count;
} DataBase;

void free_database(DataBase* schema);

#endif