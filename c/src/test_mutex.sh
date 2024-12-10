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
    (
        echo "INSERT INTO таблица1 VALUES 'User$id' $((id * 10)) 'Address$id' 'Phone$id'"
        sleep "$delay"
        echo "exit"
    ) | ./client
    if [ $? -ne 0 ]; then
        echo "Client $id failed"
    else
        echo "Client $id finished successfully"
    fi
}

# Запускаем больше клиентов с данными о пользователях
for i in {1..20}; do
    echo "Starting client $i..."
    run_client "$i" 1 &
done

# Даём всем клиентам время для подключения
sleep 2

# Создаём файл-сигнал, чтобы клиенты начали выполнение
echo "Releasing clients..."
touch "$SYNC_FILE"

# Ждём завершения всех клиентов
wait
echo "All clients finished"

# Удаляем файл-сигнал после завершения теста
rm -f "$SYNC_FILE"