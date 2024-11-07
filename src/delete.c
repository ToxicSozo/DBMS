#include <stdio.h>
#include <string.h>

#include "../include/delete.h"
#include "../include/str_split.h"

void delete(DataBase *db, char *buffer) {
    char *table_name = NULL;
    
    char **tokens = str_split(buffer, ' ');
    if (tokens[0] && strcmp(tokens[0], "DELETE") == 0) {
        if (tokens[1] && strcmp(tokens[1], "FROM") == 0) {
            table_name = strdup(tokens[2]);
        }
        
        else {
            printf("Invalid command syntax!\n");
            free_split(tokens);
            return;
        }

    }
    
    else {
        printf("Invalid command syntax!\n");
        free_split(tokens);
        return;
    }

    Table *table = get_table(db, table_name);
    if (!table) {
        printf("Table '%s' not found!\n", table_name);
        
        free(table_name);
        free_split(tokens);

        return;
    }

    if (tokens[3] && strcmp(tokens[3], "WHERE") == 0) {
        
    }

    else if (!tokens[3]) {
        csv_reader(table, db->name);
        free_table_data(table);
        csv_write(table, db->name);
    }

    free(table_name);
    free_split(tokens);
}