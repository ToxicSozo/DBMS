#include "../include/json_reader.h"
#include "../include/json_parser.h"
#include "../include/database.h"
#include "../include/database_directory.h"
#include "../include/csv_reader.h"
#include "../include/dbms.h"
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>

void print_data_node(DataNode *node) {
    while (node != NULL) {
        printf("    Data: %s\n", node->data);
        node = node->next;
    }
}

void print_table(Table *table) {
    printf("  Table: %s\n", table->table_name);
    for (size_t i = 0; i < table->column_count; i++) {
        printf("    Column: %s\n", table->columns[i].name);
        print_data_node(table->columns[i].data);
    }
}

void print_database(DataBase *db) {
    if (db == NULL) {
        printf("Database is NULL\n");
        return;
    }

    printf("Database Name: %s\n", db->name);
    printf("Tuples Limit: %d\n", db->tuples_limit);
    printf("Tables:\n");
    for (size_t i = 0; i < db->table_count; i++) {
        print_table(&db->tables[i]);
    }
}
 
int main() {
    DataBase *db = parse_json(load_json_data("scheme.json"));
    build_database_file_system(db);

    Table *table = &db->tables[0];

    char *values[] = {"value1", "value2", "value3", "value4"};
    insert(table, values);

    return 0;
}