#include <glib.h>
#include <stdio.h>
#include <string.h>
#include "../include/insert.h"

void insert(DataBase *db, char *buffer) {
    char *table_name = NULL;

    gchar **tokens = g_strsplit(buffer, " ", -1);
    if (tokens[0] && strcmp(tokens[0], "INSERT") == 0) {
        if (tokens[1] && strcmp(tokens[1], "INTO") == 0) {
            table_name = g_strdup(tokens[2]);
        } 
        
        else {
            printf("Invalid command syntax!\n");
            g_strfreev(tokens);
            return;
        }
    } 
    
    else {
        printf("Invalid command syntax!\n");
        g_strfreev(tokens);
        return;
    }

    Table *table = get_table(db, table_name);
    if (!table) {
        printf("Table '%s' not found!\n", table_name);
        g_free(table_name);
        g_strfreev(tokens);
        return;
    }

    if (tokens[3] && strcmp(tokens[3], "VALUES") == 0) {
        GPtrArray *values = g_ptr_array_new();

        for (gchar **token = &tokens[4]; *token; token++) {
            g_ptr_array_add(values, g_strdup(*token));
        }

        if (values->len != table->column_count) {
            printf("Incorrect number of values provided for table '%s'.\n", table_name);
            g_ptr_array_free(values, TRUE);
            g_free(table_name);
            g_strfreev(tokens);
            return;
        }

        csv_reader(table, db->name);
        add_data_to_table(table, (char **)values->pdata);
        csv_write(table, db->name);

        for (guint i = 0; i < values->len; i++) {
            g_free(values->pdata[i]);
        }
        g_ptr_array_free(values, TRUE);

        free_table_data(table);
    } 
    
    else {
        printf("Invalid command syntax, VALUES keyword missing!\n");
    }

    g_free(table_name);
    g_strfreev(tokens);
}