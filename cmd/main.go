package main

import (
	"fmt"
	"log"

	"github.com/Hollywood-Kid/DBMS/internal/data_structures/hmap"
	"github.com/Hollywood-Kid/DBMS/internal/inputbuffer"
	"github.com/Hollywood-Kid/DBMS/internal/meta"
)

func main() {
	hm := hmap.NewHashMap()

	hm.Insert("key1", "value1")
	hm.Insert(42, "value2")
	hm.Insert(3.14, "value3")

	hm.Print()

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
