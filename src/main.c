#include "input_buffer.h"
#include "meta_commands.h"
#include "statements.h"
#include "database.h"
#include "json_parser.h"
#include "json_reader.h"
#include "build_database_file_system.h"
#include <stdbool.h>

int main(int argc, char* argv[]) {
    DataBase *database = parse_json(load_json_data("scheme.json"));
    build_database_file_system(database);

    InputBuffer *input_buffer = new_input_buffer();
    while (true) {
        print_prompt(database->name);
        read_input(input_buffer);

        if (input_buffer->buffer[0] == '.') {
            switch (do_meta_command(input_buffer)) {
                case (META_COMMAND_SUCCESS):
                    continue;
                case (META_COMMAND_UNRECOGNIZED_COMMAND):
                    printf("Unrecognized command '%s'\n", input_buffer->buffer);
                    continue;
            }
        }

        Statement statement;
        switch (prepare_statement(input_buffer, &statement)) {
            case (PREPARE_SUCCESS):
                break;
            case (PREPARE_UNRECOGNIZED_STATEMENT):
                printf("Unrecognized keyword at start of '%s'.\n", input_buffer->buffer);
                continue;
        }

        execute_statement(database, &statement, input_buffer);
        printf("Executed.\n");
    }

    close_input_buffer(input_buffer);
}