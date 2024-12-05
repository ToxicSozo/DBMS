package database

import (
	"errors"
	"fmt"

	"github.com/Hollywood-Kid/DBMS/internal/data_structures/hmap"
)

// Column представляет колонку таблицы.
type Column struct {
	Name string
	Data []string
}

// Table представляет таблицу с колонками и строками.
type Table struct {
	Name      string
	Columns   *hmap.HashMap  // Колонки хранятся в хэш-таблице
	RowCount  int            // Общее количество строк
	ColumnMap map[string]int // Для быстрого поиска индекса колонки
}

// DataBase представляет базу данных с таблицами.
type DataBase struct {
	Name        string
	TuplesLimit int
	Tables      *hmap.HashMap // Таблицы также хранятся в хэш-таблице
}

// NewDatabase создает новую базу данных.
func NewDatabase(name string, tuplesLimit int) *DataBase {
	return &DataBase{
		Name:        name,
		TuplesLimit: tuplesLimit,
		Tables:      hmap.NewHashMap(),
	}
}

// AddTable добавляет новую таблицу в базу данных.
func (db *DataBase) AddTable(tableName string) error {
	if _, exists := db.Tables.Find(tableName); exists {
		return errors.New("table already exists")
	}

	table := &Table{
		Name:      tableName,
		Columns:   hmap.NewHashMap(),
		RowCount:  0,
		ColumnMap: make(map[string]int),
	}
	db.Tables.Insert(tableName, table)
	return nil
}

// AddColumn добавляет новую колонку в таблицу.
func (db *DataBase) AddColumn(tableName, columnName string) error {
	tableRaw, exists := db.Tables.Find(tableName)
	if !exists {
		return errors.New("table does not exist")
	}
	table := tableRaw.(*Table)

	if _, exists := table.Columns.Find(columnName); exists {
		return errors.New("column already exists")
	}

	column := &Column{
		Name: columnName,
		Data: []string{},
	}
	table.Columns.Insert(columnName, column)
	table.ColumnMap[columnName] = len(table.ColumnMap)
	return nil
}

// AddRow добавляет строку в таблицу.
func (db *DataBase) AddRow(tableName string, rowData map[string]string) error {
	tableRaw, exists := db.Tables.Find(tableName)
	if !exists {
		return errors.New("table does not exist")
	}
	table := tableRaw.(*Table)

	if len(rowData) != len(table.ColumnMap) {
		return errors.New("row data does not match column count")
	}

	// Добавляем данные в каждую колонку
	for columnName, value := range rowData {
		columnRaw, exists := table.Columns.Find(columnName)
		if !exists {
			return fmt.Errorf("column %s does not exist", columnName)
		}
		column := columnRaw.(*Column)
		column.Data = append(column.Data, value)
	}

	table.RowCount++
	return nil
}

// DeleteRow удаляет строку из таблицы.
func (db *DataBase) DeleteRow(tableName string, rowIndex int) error {
	tableRaw, exists := db.Tables.Find(tableName)
	if !exists {
		return errors.New("table does not exist")
	}
	table := tableRaw.(*Table)

	if rowIndex < 0 || rowIndex >= table.RowCount {
		return errors.New("row index out of bounds")
	}

	// Удаляем данные из каждой колонки
	table.Columns.ForEach(func(key, value interface{}) {
		column := value.(*Column)
		if rowIndex < len(column.Data) {
			column.Data = append(column.Data[:rowIndex], column.Data[rowIndex+1:]...)
		}
	})

	table.RowCount--
	return nil
}

// PrintTable выводит содержимое таблицы.
func (db *DataBase) PrintTable(tableName string) error {
	tableRaw, exists := db.Tables.Find(tableName)
	if !exists {
		return errors.New("table does not exist")
	}
	table := tableRaw.(*Table)

	// Выводим имена колонок
	table.Columns.ForEach(func(key, _ interface{}) {
		fmt.Printf("%s\t", key)
	})
	fmt.Println()

	// Выводим строки таблицы
	for i := 0; i < table.RowCount; i++ {
		table.Columns.ForEach(func(_, value interface{}) {
			column := value.(*Column)
			if i < len(column.Data) {
				fmt.Printf("%s\t", column.Data[i])
			} else {
				fmt.Print("NULL\t")
			}
		})
		fmt.Println()
	}

	return nil
}

// PrintDatabase выводит содержимое всей базы данных.
func (db *DataBase) PrintDatabase() {
	fmt.Printf("Database: %s\n", db.Name)
	fmt.Println("====================================")

	db.Tables.ForEach(func(key, value interface{}) {
		table := value.(*Table)
		fmt.Printf("Table: %s\n", table.Name)

		// Выводим заголовки колонок
		table.Columns.ForEach(func(columnKey, _ interface{}) {
			fmt.Printf("%s\t", columnKey)
		})
		fmt.Println()

		// Выводим строки таблицы
		for i := 0; i < table.RowCount; i++ {
			table.Columns.ForEach(func(_, columnValue interface{}) {
				column := columnValue.(*Column)
				if i < len(column.Data) {
					fmt.Printf("%s\t", column.Data[i])
				} else {
					fmt.Print("NULL\t")
				}
			})
			fmt.Println()
		}
		fmt.Println("------------------------------------")
	})
}
