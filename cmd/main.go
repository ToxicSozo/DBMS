package main

import (
	"fmt"
	"log"

	"github.com/Hollywood-Kid/DBMS/internal/inputbuffer"
	"github.com/Hollywood-Kid/DBMS/internal/lib/jsonparser"
	"github.com/Hollywood-Kid/DBMS/internal/lib/sqlparser"
	"github.com/Hollywood-Kid/DBMS/internal/meta"
)

func main() {
	schema, err := jsonparser.ParseJSON("/home/kid/Desktop/DBMS/scheme.json")

	if err != nil {
		fmt.Println("Ошибка при парсинге JSON:", err)
		return
	}

	tables := schema.Structure
	tables.Print()

	inputBuffer := inputbuffer.NewInputBuffer()

	for {
		inputbuffer.PrintPrompt(schema.Name)

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

		command, err := sqlparser.ParseSQL(inputBuffer.GetBuffer())
		if err != nil {
			log.Printf("Ошибка разбора: %v\n", err)
			continue
		}

		fmt.Printf("Разобранная команда: %+v\n", command)
	}
}
