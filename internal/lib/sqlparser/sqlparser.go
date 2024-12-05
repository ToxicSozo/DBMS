package sqlparser

import (
	"errors"
	"strings"
)

type SQLCommandType int

const (
	InsertCommand SQLCommandType = iota
	SelectCommand
	DeleteCommand
)

type SQLParsedCommand struct {
	Type      SQLCommandType
	Tables    []string
	Columns   []string
	Values    []string
	Condition string
}

func ParseSQL(input string) (*SQLParsedCommand, error) {
	tokens := strings.Fields(input)
	if len(tokens) == 0 {
		return nil, errors.New("empty input")
	}

	commandType, err := detectCommandType(tokens[0])
	if err != nil {
		return nil, err
	}

	parsedCommand := &SQLParsedCommand{Type: commandType}

	switch commandType {
	case InsertCommand:
		return parseInsert(tokens, parsedCommand)
	case SelectCommand:
		return parseSelect(tokens, parsedCommand)
	case DeleteCommand:
		return parseDelete(tokens, parsedCommand)
	}

	return nil, errors.New("unsupported command type")
}

func detectCommandType(command string) (SQLCommandType, error) {
	switch strings.ToUpper(command) {
	case "INSERT":
		return InsertCommand, nil
	case "SELECT":
		return SelectCommand, nil
	case "DELETE":
		return DeleteCommand, nil
	default:
		return -1, errors.New("unrecognized command type")
	}
}

func parseInsert(tokens []string, parsedCommand *SQLParsedCommand) (*SQLParsedCommand, error) {
	if len(tokens) < 4 || strings.ToUpper(tokens[1]) != "INTO" || strings.ToUpper(tokens[3]) != "VALUES" {
		return nil, errors.New("invalid INSERT command syntax")
	}

	parsedCommand.Tables = append(parsedCommand.Tables, tokens[2])
	values := strings.Trim(tokens[4], "()")
	parsedCommand.Values = splitAndTrim(values, ",")

	return parsedCommand, nil
}

func parseSelect(tokens []string, parsedCommand *SQLParsedCommand) (*SQLParsedCommand, error) {
	fromIndex := findKeywordIndex(tokens, "FROM")
	if fromIndex == -1 || fromIndex == 1 {
		return nil, errors.New("invalid SELECT command syntax")
	}

	parsedCommand.Columns = splitAndTrim(strings.Join(tokens[1:fromIndex], " "), ",")
	whereIndex := findKeywordIndex(tokens, "WHERE")

	if whereIndex == -1 {
		parsedCommand.Tables = tokens[fromIndex+1:]
	} else {
		parsedCommand.Tables = tokens[fromIndex+1 : whereIndex]
		parsedCommand.Condition = strings.Join(tokens[whereIndex+1:], " ")
	}

	return parsedCommand, nil
}

func parseDelete(tokens []string, parsedCommand *SQLParsedCommand) (*SQLParsedCommand, error) {
	if len(tokens) < 3 || strings.ToUpper(tokens[1]) != "FROM" {
		return nil, errors.New("invalid DELETE command syntax")
	}

	parsedCommand.Tables = append(parsedCommand.Tables, tokens[2])
	if len(tokens) > 3 && strings.ToUpper(tokens[3]) == "WHERE" {
		parsedCommand.Condition = strings.Join(tokens[4:], " ")
	}

	return parsedCommand, nil
}

func findKeywordIndex(tokens []string, keyword string) int {
	for i, token := range tokens {
		if strings.ToUpper(token) == keyword {
			return i
		}
	}
	return -1
}

func splitAndTrim(input, delimiter string) []string {
	parts := strings.Split(input, delimiter)
	for i, part := range parts {
		parts[i] = strings.TrimSpace(part)
	}
	return parts
}
