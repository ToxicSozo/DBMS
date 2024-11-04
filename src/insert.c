#include <stdio.h>
#include <string.h>

#include "../include/insert.h"
#include "../include/str_split.h"

void insert(DataBase *db, char *buffer) {
    char *table_name = NULL;

    char **tokens = str_split(buffer, ' ');
    if (tokens[0] && strcmp(tokens[0], "INSERT") == 0) {
        if (tokens[1] && strcmp(tokens[1], "INTO") == 0) {
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

    if (tokens[3] && strcmp(tokens[3], "VALUES") == 0) {
        int val_count = 0;
        
        for (char **token = &tokens[4]; *token; token++) {
            val_count++;
        }

        if (val_count != table->column_count) {
            printf("Incorrect number of values provided for table '%s'.\n", table_name);
            free(table_name);
            free_split(tokens);
            return;
        }

        char **values = malloc((val_count + 1) * sizeof(char *));

        int i = 0;
        for (char **token = &tokens[4]; *token; token++) {
            values[i++] = strdup(*token);
        }
        values[i] = NULL;

        csv_reader(table, db->name);
        add_data_to_table(table, values);
        csv_write(table, db->name);

        for (int j = 0; j < val_count; j++) {
            free(values[j]);
        }
        free(values);
        
        free_table_data(table);
    } 
    
    else {
        printf("Invalid command syntax, VALUES keyword missing!\n");
    }

    free(table_name);
    free_split(tokens);
}