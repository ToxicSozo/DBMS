#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "../include/erproc.h"

#define SERVER_PORT 7432
#define SERVER_IP "127.0.0.1"
#define BUFFER_SIZE 256

int main() {
    // Создание сокета
    int fd = Socket(AF_INET, SOCK_STREAM, 0);

    // Инициализация адреса сервера
    struct sockaddr_in adr = {0};
    adr.sin_family = AF_INET;
    adr.sin_port = htons(SERVER_PORT);
    Inet_pton(AF_INET, SERVER_IP, &adr.sin_addr);

    // Подключение к серверу
    Connect(fd, (struct sockaddr *) &adr, sizeof adr);
    printf("Connected to server at %s:%d\n", SERVER_IP, SERVER_PORT);

    char buffer[BUFFER_SIZE];
    while (1) {
        // Чтение команды от пользователя
        printf("Enter a query (or type 'exit' to quit):\n");
        if (!fgets(buffer, BUFFER_SIZE, stdin)) {
            printf("Error reading input\n");
            break;
        }

        // Убираем символ новой строки, если он есть
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }

        // Проверяем, хочет ли пользователь выйти
        if (strcmp(buffer, "exit") == 0) {
            printf("Exiting...\n");
            break;
        }

        // Отправка запроса серверу
        if (write(fd, buffer, strlen(buffer)) == -1) {
            perror("write failed");
            break;
        }

        // Получение ответа от сервера
        ssize_t nread = read(fd, buffer, BUFFER_SIZE - 1);
        if (nread == -1) {
            perror("read failed");
            break;
        }
        if (nread == 0) {
            printf("Server disconnected\n");
            break;
        }

        // Завершаем строку для печати
        buffer[nread] = '\0';
        printf("Response from server:\n%s\n", buffer);
    }

    // Закрытие соединения
    close(fd);
    return 0;
}
