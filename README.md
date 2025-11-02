# Go-based Relational Database with SQL Support / Реляционная база данных на Go

This README is available in both English and Russian to make the project easier to use for a wider
audience.

## English

### Overview

This project is a rewrite of the original C implementation in Go. It provides a lightweight
relational database that stores tables as CSV files and supports a small subset of SQL
statements for interacting with the data.

### Supported SQL

The interactive shell understands the following commands:

- `SELECT ... FROM ... [WHERE ...]` with support for `AND`, `OR`, and parenthesis in the
  `WHERE` clause. Use `*` to select all columns.
- `INSERT INTO ... (columns) VALUES (values)` to append rows to a table.
- `DELETE FROM ... [WHERE ...]` to remove rows.

String literals use single quotes and escape embedded quotes using `''`.

Additional meta commands are available inside the shell:

- `.tables` — list tables from the schema.
- `EXIT` or `QUIT` — leave the shell.

### Schema and storage

The database layout is described by a JSON schema file (default: `schema.json`). The schema
contains the database name, a tuples-per-file hint (currently unused), and a map of table names to
column definitions. For each table the engine automatically creates an auto-incrementing
primary key column named `id`.

Table data is stored in CSV files inside the configured data directory. The engine also keeps
per-table metadata files with the next primary key value.

### Getting started

1. **Install Go** (version 1.21 or newer).
2. **Run the shell**:

   ```bash
   go run ./cmd/dbms -schema schema.json -data data
   ```

   The command creates the data directory if needed and prepares the tables declared in the
   schema.

3. **Execute SQL statements** at the `db>` prompt. Each statement must end with a semicolon.

### Example session

```
$ go run ./cmd/dbms
Schema "Example Schema" loaded. Data directory: /path/to/data
Type SQL statements terminated by ';'. Commands: .tables, EXIT, QUIT.
db> .tables
orders
users
db> INSERT INTO users (name, email) VALUES ('Alice', 'alice@example.com');
Inserted row with id 1
db> SELECT * FROM users;
id | name  | email
--+-------+---------------------
1 | Alice | alice@example.com
db> DELETE FROM users WHERE name = 'Alice';
Deleted 1 row(s)
db> EXIT
```

### Project structure

```
cmd/dbms        Interactive shell entry point
internal/db     CSV storage engine and schema handling
internal/sql    SQL tokenizer, parser, and executor
schema.json     Default schema definition
```

Feel free to adapt the schema and extend the SQL support based on your needs.

## Русский

### Обзор

Этот проект представляет собой переписанную на Go версию исходной реализации на C. Он
предоставляет лёгкую реляционную базу данных, которая хранит таблицы в виде CSV-файлов и
поддерживает небольшой поднабор SQL-команд для работы с данными.

### Поддерживаемый SQL

Интерактивная оболочка понимает следующие команды:

- `SELECT ... FROM ... [WHERE ...]` с поддержкой `AND`, `OR` и скобок в выражении `WHERE`.
  Чтобы выбрать все столбцы, используйте `*`.
- `INSERT INTO ... (columns) VALUES (values)` для добавления строк в таблицу.
- `DELETE FROM ... [WHERE ...]` для удаления строк.

Строковые литералы записываются в одинарных кавычках. Встроенные кавычки экранируются как `''`.

Дополнительные мета-команды оболочки:

- `.tables` — выводит список таблиц из схемы.
- `EXIT` или `QUIT` — завершает работу оболочки.

### Схема и хранение данных

Структура базы задаётся JSON-файлом схемы (по умолчанию `schema.json`). В схеме указаны имя
базы данных, рекомендация по количеству кортежей в файле (пока не используется), а также
отображение имён таблиц на определение их столбцов. Для каждой таблицы движок автоматически
создаёт автоинкрементный первичный ключ `id`.

Данные таблиц сохраняются в CSV-файлах внутри настроенной директории данных. Для каждой таблицы
также хранится вспомогательный файл с информацией о следующем значении первичного ключа.

### Быстрый старт

1. **Установите Go** версии 1.21 или новее.
2. **Запустите оболочку** командой:

   ```bash
   go run ./cmd/dbms -schema schema.json -data data
   ```

   Команда при необходимости создаст директорию данных и подготовит таблицы, определённые в
   схеме.

3. **Вводите SQL-команды** в приглашении `db>`. Каждое выражение должно заканчиваться точкой с
   запятой.

### Пример сессии

```
$ go run ./cmd/dbms
Schema "Example Schema" loaded. Data directory: /path/to/data
Type SQL statements terminated by ';'. Commands: .tables, EXIT, QUIT.
db> .tables
orders
users
db> INSERT INTO users (name, email) VALUES ('Alice', 'alice@example.com');
Inserted row with id 1
db> SELECT * FROM users;
id | name  | email
--+-------+---------------------
1 | Alice | alice@example.com
db> DELETE FROM users WHERE name = 'Alice';
Deleted 1 row(s)
db> EXIT
```

### Структура проекта

```
cmd/dbms        Точка входа интерактивной оболочки
internal/db     Движок хранения CSV и обработка схемы
internal/sql    Токенизатор, парсер и исполнитель SQL
schema.json     Схема базы данных по умолчанию
```

При необходимости адаптируйте схему и расширяйте поддержку SQL под свои задачи.
