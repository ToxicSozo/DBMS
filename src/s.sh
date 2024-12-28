#!/bin/bash

# Компиляция проекта
gcc server.c erproc.c sql_parser.c sql_parsed_command.c list.c str_split.c statements.c json_parser.c json_reader.c database.c build_database_file_system.c insert.c csv_reader.c csv_writer.c delete.c select.c -o server

# Проверка успешности компиляции
if [ $? -eq 0 ]; then
    echo "Компиляция завершена успешно. Исполняемый файл: server"
else
    echo "Ошибка при компиляции"
fi