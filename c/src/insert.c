#include <stdio.h>
#include <unistd.h>

#include "../include/insert.h"
#include "../include/sql_parsed_command.h"
#include "../include/build_database_file_system.h"
#include "../include/str_split.h"


void insert(DataBase *db, SQLParsedCommand *parsed_command) {
    const char *table_name = get_element_at(parsed_command->tables, 0);

    if (!lock_table(db->name, table_name)) {
        return; // Если таблица заблокирована, завершаем выполнение
    }

    printf("Ожидание в заблокированном состоянии (5 секунд)...\n");
    sleep(5); // Задержка для проверки блокировки

    Table *table = get_table(db, table_name);
    if (!table) {
        printf("Table '%s' not found!\n", table_name);
        unlock_table(db->name, table_name);
        return;
    }

    csv_reader(table, db->name);
    add_data_to_table(table, parsed_command->values);
    csv_write(table, db->name);

    free_table_data(table);
    unlock_table(db->name, table_name); // Снимаем блокировку таблицы
}
