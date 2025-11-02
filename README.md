# Go-based Relational Database with SQL Support

This project is a rewrite of the original C implementation in Go. It provides a lightweight
relational database that stores tables as CSV files and supports a small subset of SQL
statements for interacting with the data.

## Supported SQL

The interactive shell understands the following commands:

- `SELECT ... FROM ... [WHERE ...]` with support for `AND`, `OR`, and parenthesis in the
  `WHERE` clause. Use `*` to select all columns.
- `INSERT INTO ... (columns) VALUES (values)` to append rows to a table.
- `DELETE FROM ... [WHERE ...]` to remove rows.

String literals use single quotes and escape embedded quotes using `''`.

Additional meta commands are available inside the shell:

- `.tables` — list tables from the schema.
- `EXIT` or `QUIT` — leave the shell.

## Schema and storage

The database layout is described by a JSON schema file (default: `schema.json`). The schema
contains the database name, a tuples-per-file hint (currently unused), and a map of table names to
column definitions. For each table the engine automatically creates an auto-incrementing
primary key column named `id`.

Table data is stored in CSV files inside the configured data directory. The engine also keeps
per-table metadata files with the next primary key value.

## Getting started

1. **Install Go** (version 1.21 or newer).
2. **Run the shell**:

   ```bash
   go run ./cmd/dbms -schema schema.json -data data
   ```

   The command creates the data directory if needed and prepares the tables declared in the
   schema.

3. **Execute SQL statements** at the `db>` prompt. Each statement must end with a semicolon.

## Example session

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

## Project structure

```
cmd/dbms        Interactive shell entry point
internal/db     CSV storage engine and schema handling
internal/sql    SQL tokenizer, parser, and executor
schema.json     Default schema definition
```

Feel free to adapt the schema and extend the SQL support based on your needs.
