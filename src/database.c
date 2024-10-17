#include "../include/database.h"

DataBase* create_database(const char *database_name, DataBase *db) {
    db->name = strdup(database_name);
    db->table_count = 0;
    db->tables = NULL;
    db->tuples_limit = 0;

    return db;
}

void add_table_to_database(DataBase *db, const char *table_name) {
    db->table_count++;
    db->tables = realloc(db->tables, db->table_count * sizeof(Table));
    db->tables[db->table_count - 1].table_name = strdup(table_name);
    db->tables[db->table_count - 1].columns = NULL;
    db->tables[db->table_count - 1].column_count = 0;
}

void add_column_to_table(Table *table, const char *column_name) {
    table->column_count++;
    table->columns = realloc(table->columns, table->column_count * sizeof(Column));
    table->columns[table->column_count - 1].name = strdup(column_name);
    table->columns[table->column_count - 1].data = NULL;
}

void add_data_to_table(Table *table, char **data) {
    if (table == NULL || data == NULL) {
        return;
    }
    
    for (size_t i = 0; i < table->column_count; i++) {
        DataNode *new_node = malloc(sizeof(DataNode));

        new_node->data = strdup(data[i]);
        new_node->next = NULL;

        if (table->columns[i].data == NULL) {
            table->columns[i].data = new_node;
        } 
        
        else {
            DataNode *current = table->columns[i].data;
            while (current->next != NULL) {
                current = current->next;
            }
            current->next = new_node;
        }
    }
}