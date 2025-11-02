package main

import (
	"bufio"
	"flag"
	"fmt"
	"log"
	"os"
	"path/filepath"
	"strings"

	"github.com/ToxicSozo/DBMS/internal/db"
	sql "github.com/ToxicSozo/DBMS/internal/sql"
)

func main() {
	schemaPath := flag.String("schema", "schema.json", "Path to schema definition")
	dataDir := flag.String("data", "data", "Directory for table files")
	flag.Parse()

	schema, err := db.LoadSchema(*schemaPath)
	if err != nil {
		log.Fatalf("load schema: %v", err)
	}

	absDataDir := *dataDir
	if !filepath.IsAbs(absDataDir) {
		absDataDir = filepath.Join(filepath.Dir(*schemaPath), absDataDir)
	}

	database, err := db.NewDatabase(schema, absDataDir)
	if err != nil {
		log.Fatalf("init database: %v", err)
	}

	fmt.Printf("Schema %q loaded. Data directory: %s\n", schema.Name, absDataDir)
	fmt.Println("Type SQL statements terminated by ';'. Commands: .tables, EXIT, QUIT.")

	scanner := bufio.NewScanner(os.Stdin)
	scanner.Buffer(make([]byte, 0, 1024), 1024*1024)

	var buffer strings.Builder
	for {
		fmt.Print("db> ")
		if !scanner.Scan() {
			fmt.Println()
			break
		}
		line := scanner.Text()
		trimmed := strings.TrimSpace(line)
		if trimmed == "" {
			continue
		}

		upper := strings.ToUpper(trimmed)
		if upper == "EXIT" || upper == "QUIT" {
			break
		}

		if trimmed == ".tables" {
			tables := database.ListTables()
			if len(tables) == 0 {
				fmt.Println("No tables defined")
				continue
			}
			for _, tbl := range tables {
				fmt.Println(tbl)
			}
			continue
		}

		buffer.WriteString(line)
		if strings.HasSuffix(strings.TrimSpace(line), ";") {
			query := buffer.String()
			buffer.Reset()
			handleQuery(database, query)
		} else {
			buffer.WriteRune(' ')
		}
	}

	if err := scanner.Err(); err != nil {
		log.Printf("input error: %v", err)
	}
}

func handleQuery(database *db.Database, query string) {
	stmt, err := sql.Parse(query)
	if err != nil {
		fmt.Printf("Parse error: %v\n", err)
		return
	}

	result, err := sql.Execute(database, stmt)
	if err != nil {
		fmt.Printf("Execution error: %v\n", err)
		return
	}

	if len(result.Columns) > 0 {
		printTable(result.Columns, result.Rows)
	}
	if result.Message != "" {
		fmt.Println(result.Message)
	}
}

func printTable(columns []string, rows [][]string) {
	widths := make([]int, len(columns))
	for i, col := range columns {
		widths[i] = len(col)
	}
	for _, row := range rows {
		for i, value := range row {
			if len(value) > widths[i] {
				widths[i] = len(value)
			}
		}
	}

	format := make([]string, len(columns))
	for i, width := range widths {
		format[i] = fmt.Sprintf("%%-%ds", width)
	}

	for i, col := range columns {
		fmt.Printf(format[i], col)
		if i < len(columns)-1 {
			fmt.Print(" | ")
		}
	}
	fmt.Println()

	for i, width := range widths {
		fmt.Print(strings.Repeat("-", width))
		if i < len(widths)-1 {
			fmt.Print("-+-")
		}
	}
	fmt.Println()

	for _, row := range rows {
		for i, value := range row {
			fmt.Printf(format[i], value)
			if i < len(row)-1 {
				fmt.Print(" | ")
			}
		}
		fmt.Println()
	}
}
