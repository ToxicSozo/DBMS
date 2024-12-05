package main

import (
	"fmt"
	"log"

	"github.com/Hollywood-Kid/DBMS/internal/inputbuffer"
	"github.com/Hollywood-Kid/DBMS/internal/meta"
)

func main() {
	schema, err := json.parseJSON("scheme.json")
	if err != nil {
		fmt.Println("Ошибка при парсинге JSON:", err)
		return
	}

	// Выводим полученную структуру
	fmt.Println("Schema Name:", schema.Name)
	fmt.Println("Tuples Limit:", schema.TuplesLimit)

	// Пример работы с хэш-таблицей
	tables := schema.Structure.Get("таблица1")
	fmt.Println("Columns in таблица1:", tables)

	databaseName := "MyDatabase"
	inputBuffer := inputbuffer.NewInputBuffer()

	for {
		inputbuffer.PrintPrompt(databaseName)

		err := inputBuffer.ReadInput()
		if err != nil {
			log.Fatal(err)
		}

		if inputBuffer.GetBuffer()[0] == '.' {
			result := meta.ExecuteMetaCommand(inputBuffer.GetBuffer())
			if result == meta.MetaCommandUnrecognizedCommand {
				fmt.Println("Unrecognized meta-command. Type .help for a list of available commands.")
			}
			continue
		}

	}
}
