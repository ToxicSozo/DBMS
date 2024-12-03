package inputbuffer

import (
	"bufio"
	"fmt"
	"os"
	"strings"
)

type InputBuffer struct {
	buffer       string
	bufferLength int
	inputLength  int
}

func NewInputBuffer() *InputBuffer {
	return &InputBuffer{
		buffer:       "",
		bufferLength: 0,
		inputLength:  0,
	}
}

func PrintPrompt(databaseName string) {
	fmt.Printf("%s > ", databaseName)
}

func (ib *InputBuffer) ReadInput() error {
	reader := bufio.NewReader(os.Stdin)
	input, err := reader.ReadString('\n')
	if err != nil {
		return fmt.Errorf("error reading input: %w", err)
	}

	ib.buffer = strings.TrimSuffix(input, "\n")
	ib.inputLength = len(ib.buffer)
	ib.bufferLength = len(ib.buffer)

	return nil
}

func (ib *InputBuffer) GetBuffer() string {
	return ib.buffer
}

func (ib *InputBuffer) GetInputLength() int {
	return ib.inputLength
}

func (ib *InputBuffer) GetBufferLength() int {
	return ib.bufferLength
}
