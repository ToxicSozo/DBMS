package repl

import (
	"bufio"
	"fmt"
	"os"
	"strings"
)

type InputBuffer struct {
	Buffer       string
	BufferLength int
	InputLength  int
}

func NewInputBuffer() *InputBuffer {
	return &InputBuffer{
		Buffer:       "",
		BufferLength: 0,
		InputLength:  0,
	}
}

func PrintPrompt(databaseName string) {
	fmt.Printf("%s > ", databaseName)
}

func ReadInput(inputBuffer *InputBuffer) {
	reader := bufio.NewReader(os.Stdin)
	input, err := reader.ReadString('\n')
	if err != nil {
		fmt.Println("Error reading input")
		os.Exit(1)
	}

	input = strings.TrimSpace(input)

	inputBuffer.Buffer = input
	inputBuffer.BufferLength = len(input)
	inputBuffer.InputLength = len(input)
}

func CloseInputBuffer(inputBuffer *InputBuffer) {
	inputBuffer.Buffer = ""
	inputBuffer.BufferLength = 0
	inputBuffer.InputLength = 0
}
