#!/bin/bash

# Файл-сигнал для синхронизации
SYNC_FILE="/tmp/sync_file"

# Удаляем файл-сигнал, если он существует
rm -f "$SYNC_FILE"

# Функция для запуска клиента
run_client() {
    local id=$1
    local delay=$2

    # Ждём, пока файл-сигнал не появится
    while [ ! -f "$SYNC_FILE" ]; do
        sleep 0.1
    done

    # Отправляем команду после появления файла-сигнала
    echo "DELETE FROM таблица1 WHERE колонка1 = 'User1'"
    sleep 2

    echo "DELETE FROM таблица1 WHERE колонка1 = 'User2'"
    sleep 2

    echo "DELETE FROM таблица1 WHERE колонка1 = 'User3'"
    sleep 2

    echo "DELETE FROM таблица1 WHERE колонка1 = 'User99' OR колонка4 = 'Phone17'"
    sleep 2

    echo "DELETE FROM таблица1 WHERE колонка1 = 'User5' AND колонка4 = 'Address100'"
    sleep 2

    echo "DELETE FROM таблица1 WHERE колонка1 = 'User5' AND колонка4 = 'Address5'"
    sleep 2

    echo "DELETE FROM таблица1 WHERE колонка1 = 'User99' OR колонка4 = 'Phone18' AND колонка1 = 'User20' OR колонка4 = 'Phone1000'"
    sleep 5

    echo "DELETE FROM таблица1"

    sleep "$delay"

    echo "exit"
}

# Запускаем клиента с несколькими вызовами команд
echo "Запускаем клиента..."

# Создаём файл-сигнал, чтобы клиент начал выполнение
echo "Запускаем клиента..."
touch "$SYNC_FILE"

# Запускаем клиента и отправляем команды
run_client 1 1 | ./client

# Удаляем файл-сигнал после завершения теста
rm -f "$SYNC_FILE"