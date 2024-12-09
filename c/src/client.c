#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "../include/erproc.h"

#define SERVER_PORT 12345
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
    
    // Отправка запроса серверу (например, SQL-запроса)
    char *request = "INSERT INTO таблица1 VALUES (1 2 3 4)";
    write(fd, request, strlen(request));
    
    // Получение ответа от сервера
    char buf[BUFFER_SIZE];
    ssize_t nread = read(fd, buf, BUFFER_SIZE);
    if (nread == -1) {
        perror("read failed");
        close(fd);
        exit(EXIT_FAILURE);
    }
    if (nread == 0) {
        printf("EOF occurred\n");
    }

    // Печать ответа от сервера
    buf[nread] = '\0';  // Добавляем нулевой символ в конец строки
    printf("Response from server:\n%s\n", buf);
    
    // Закрытие соединения
    close(fd);
    return 0;
}