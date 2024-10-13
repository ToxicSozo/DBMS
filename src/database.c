#include "../include/database.h"

void free_database(DataBase* schema) {
    if (schema) {
        free(schema->name);
        for (size_t i = 0; i < schema->table_count; i++) {
            free(schema->tables[i].table_name);
            for (size_t j = 0; j < schema->tables[i].columns->count; j++) {
                free(schema->tables[i].columns->columns[j]);
            }
            free(schema->tables[i].columns->columns);
            free(schema->tables[i].columns);
        }
        free(schema->tables);
        free(schema);
    }
}