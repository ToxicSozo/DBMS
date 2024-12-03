package meta

import (
	"fmt"
	"os"
	"strings"
)

type MetaCommandResult int

const (
	MetaCommandSuccess MetaCommandResult = iota
	MetaCommandUnrecognizedCommand
)

var metaCommands = map[string]func(){
	".exit": func() {
		fmt.Println("Exiting the program.")
		os.Exit(0)
	},
	".help": func() {
		fmt.Println("Available meta-commands:")
		fmt.Println(".exit - Exit the program.")
		fmt.Println(".help - Show this help message.")
	},
}

func ExecuteMetaCommand(input string) MetaCommandResult {
	command := strings.TrimSpace(input)

	if action, exists := metaCommands[command]; exists {
		action()
		return MetaCommandSuccess
	}

	fmt.Printf("Unrecognized command '%s'\n", command)
	return MetaCommandUnrecognizedCommand
}
