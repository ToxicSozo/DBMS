package main

import (
	"fmt"
	"log"

	"github.com/Hollywood-Kid/DBMS/internal/inputbuffer"
	"github.com/Hollywood-Kid/DBMS/internal/meta"
)

func main() {
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
