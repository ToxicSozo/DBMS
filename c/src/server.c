#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "../include/erproc.h"
#include "../include/meta_commands.h"
#include "../include/statements.h"
#include "../include/database.h"
#include "../include/json_parser.h"
#include "../include/json_reader.h"
#include "../include/build_database_file_system.h"
#include "../include/sql_parsed_command.h"
#include "../include/sql_parser.h"

#define PORT 12345
#define BUFFER_SIZE 256

int main() {
    // Создание сокета
    int server = Socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in adr = {0};
    adr.sin_family = AF_INET;
    adr.sin_port = htons(PORT);
    adr.sin_addr.s_addr = INADDR_ANY;

    DataBase *database = parse_json(load_json_data("scheme.json"));
    build_database_file_system(database);

    // Привязка и прослушивание сокета
    Bind(server, (struct sockaddr *) &adr, sizeof adr);
    Listen(server, 5);

    printf("Server is listening on port %d...\n", PORT);

    while (1) {
        // Принятие соединений от клиента
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        int client_fd = Accept(server, (struct sockaddr *) &client_addr, &client_len);
        if (client_fd == -1) {
            perror("Accept failed");
            continue;  // Если не удалось принять соединение, продолжить слушать
        }

        printf("Client connected\n");

        char buffer[BUFFER_SIZE];

        while (1) {
            memset(buffer, 0, BUFFER_SIZE);
            ssize_t nread = read(client_fd, buffer, BUFFER_SIZE);
            if (nread == -1) {
                perror("read failed");
                close(client_fd);
                break;
            }
            if (nread == 0) {
                printf("Client disconnected\n");
                break;  // Выход из внутреннего цикла, если клиент отключился
            }

            // Обрезаем символ новой строки, если он есть
            if (buffer[nread - 1] == '\n') {
                buffer[nread - 1] = '\0';
            }

            printf("Received query: %s\n", buffer);

            Statement statement;
            switch (prepare_statement(buffer, &statement)) {
                case (PREPARE_SUCCESS):
                    break;
                case (PREPARE_UNRECOGNIZED_STATEMENT):
                    printf("Unrecognized statement '%s'.\n", buffer);
                    write(client_fd, "Error: Unrecognized statement\n", 30);
                    continue;
            }

            SQLParsedCommand *parsed_command = sql_parser(buffer, &statement);
            execute_statement(database, &statement, parsed_command);
            free_parsed_command(parsed_command);

            write(client_fd, "Executed.\n", 10);
        }

        close(client_fd);
    }

    close(server);

    return 0;
}
