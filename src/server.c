#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "../include/erproc.h"
#include "../include/meta_commands.h"
#include "../include/statements.h"
#include "../include/database.h"
#include "../include/json_parser.h"
#include "../include/json_reader.h"
#include "../include/build_database_file_system.h"
#include "../include/sql_parsed_command.h"
#include "../include/sql_parser.h"

#define PORT 7432
#define BUFFER_SIZE 256

// Мьютекс для синхронизации доступа к базе данных
pthread_mutex_t db_mutex;

// Общий ресурс — база данных
DataBase *database;

void *handle_client(void *arg) {
    int client_fd = *(int *)arg;
    free(arg);  // Освобождаем память, выделенную для client_fd
    char buffer[BUFFER_SIZE];

    printf("Клиент %d подключился и ожидает команду...\n", client_fd);

    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        ssize_t nread = read(client_fd, buffer, BUFFER_SIZE);
        if (nread == -1) {
            perror("Ошибка чтения");
            break;
        }
        if (nread == 0) {
            printf("Клиент %d отключился\n", client_fd);
            break;
        }

        // Обрезаем символ новой строки, если он есть
        if (buffer[nread - 1] == '\n') {
            buffer[nread - 1] = '\0';
        }

        printf("Клиент %d отправил запрос: %s\n", client_fd, buffer);

        // Синхронизируем доступ к базе данных с помощью мьютекса
        printf("Клиент %d ожидает выполнения команды...\n", client_fd);
        pthread_mutex_lock(&db_mutex);
        printf("Клиент %d выполняет команду...\n", client_fd);

        // Обрабатываем запрос клиента (например, SQL-запрос)
        Statement statement;
        if (prepare_statement(buffer, &statement) != PREPARE_SUCCESS) {
            write(client_fd, "Ошибка: Нераспознанная команда\n", 30);
        } else {
            SQLParsedCommand *parsed_command = sql_parser(buffer, &statement);
            execute_statement(database, &statement, parsed_command);
            free_parsed_command(parsed_command);
            write(client_fd, "Выполнено.\n", 10);
        }

        printf("Клиент %d завершил выполнение команды.\n", client_fd);
        pthread_mutex_unlock(&db_mutex);  // Освобождаем мьютекс
        printf("Клиент %d освободил блокировку, готов к следующей команде.\n", client_fd);
    }

    close(client_fd);
    return NULL;
}

int main() {
    // Инициализируем мьютекс
    pthread_mutex_init(&db_mutex, NULL);

    // Загружаем базу данных
    database = parse_json(load_json_data("scheme.json"));
    build_database_file_system(database);

    // Создаем серверный сокет
    int server_fd = Socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_addr = {0};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    Bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    Listen(server_fd, 100);

    printf("Сервер слушает порт %d...\n", PORT);

    while (1) {
        // Принимаем новое соединение
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        int *client_fd = malloc(sizeof(int));
        if (!client_fd) {
            perror("Ошибка выделения памяти");
            continue;
        }
        *client_fd = Accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
        printf("Клиент подключился\n");

        // Создаем новый поток для клиента
        pthread_t client_thread;
        if (pthread_create(&client_thread, NULL, handle_client, client_fd) != 0) {
            perror("Ошибка создания потока");
            close(*client_fd);
            free(client_fd);
            continue;
        }

        // Отсоединяем поток, чтобы он автоматически очищался после завершения
        pthread_detach(client_thread);
    }

    // Освобождаем ресурсы
    close(server_fd);
    pthread_mutex_destroy(&db_mutex);

    return 0;
}