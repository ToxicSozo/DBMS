package builfilesistem

import (
	"encoding/csv"
	"fmt"
	"os"
	"path/filepath"

	"github.com/Hollywood-Kid/DBMS/internal/static/resources"
)

func BuildFileSystem(schema resources.Schema) error {
	// Проверяем, существует ли корневая папка
	rootPath := schema.Name
	if _, err := os.Stat(rootPath); os.IsNotExist(err) {
		// Если папка не существует, создаем её
		if err := os.Mkdir(rootPath, 0755); err != nil {
			return fmt.Errorf("failed to create root directory: %w", err)
		}
	}

	// Проходим по всем бакетам в HashMap
	buckets := schema.Structure.GetBuckets()
	for i := 0; i < len(buckets); i++ {
		bucket := buckets[i]
		if bucket == nil {
			continue
		}
		entries := bucket.GetEntries()
		for _, entry := range entries {
			tableName := entry.GetKey().(string)

			// Создаем папку для таблицы
			tablePath := filepath.Join(rootPath, tableName)
			if _, err := os.Stat(tablePath); os.IsNotExist(err) {
				if err := os.Mkdir(tablePath, 0755); err != nil {
					return fmt.Errorf("failed to create table directory: %w", err)
				}
			}

			// Создаем файл 1.csv в папке таблицы
			csvFilePath := filepath.Join(tablePath, "1.csv")
			if _, err := os.Stat(csvFilePath); os.IsNotExist(err) {
				file, err := os.Create(csvFilePath)
				if err != nil {
					return fmt.Errorf("failed to create CSV file: %w", err)
				}
				defer file.Close()

				// Получаем имена колонок из Structure
				columns, ok := schema.Structure.Find(tableName)
				if !ok {
					return fmt.Errorf("columns not found for table: %s", tableName)
				}

				// Преобразуем имена колонок в срез строк
				columnNames := columns.([]string)

				// Записываем имена колонок в файл CSV
				writer := csv.NewWriter(file)
				if err := writer.Write(columnNames); err != nil {
					return fmt.Errorf("failed to write column names to CSV file: %w", err)
				}
				writer.Flush()

				if err := writer.Error(); err != nil {
					return fmt.Errorf("error flushing CSV writer: %w", err)
				}
			}
		}
	}

	return nil
}
