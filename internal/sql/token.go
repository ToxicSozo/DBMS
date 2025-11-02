package sql

import "strings"

// TokenType represents the type of lexical token.
type TokenType int

const (
	TokenEOF TokenType = iota
	TokenIdentifier
	TokenString
	TokenComma
	TokenLParen
	TokenRParen
	TokenEqual
	TokenAsterisk
	TokenSemicolon
)

// Token describes a lexical token with its literal representation.
type Token struct {
	Type    TokenType
	Literal string
}

var keywords = map[string]TokenType{
	"SELECT": TokenIdentifier,
	"FROM":   TokenIdentifier,
	"WHERE":  TokenIdentifier,
	"INSERT": TokenIdentifier,
	"INTO":   TokenIdentifier,
	"VALUES": TokenIdentifier,
	"DELETE": TokenIdentifier,
	"AND":    TokenIdentifier,
	"OR":     TokenIdentifier,
	"EXIT":   TokenIdentifier,
	"QUIT":   TokenIdentifier,
}

func normalizeIdent(s string) string {
	return strings.ToUpper(s)
}
