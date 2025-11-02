package sql

import (
	"fmt"
	"strings"
	"unicode"
)

// Tokenizer converts an input string into a stream of tokens.
type Tokenizer struct {
	input []rune
	pos   int
}

// NewTokenizer creates a tokenizer for a SQL input string.
func NewTokenizer(input string) *Tokenizer {
	return &Tokenizer{input: []rune(input)}
}

// Next returns the next token in the stream.
func (t *Tokenizer) Next() (Token, error) {
	t.skipWhitespace()
	if t.pos >= len(t.input) {
		return Token{Type: TokenEOF}, nil
	}

	ch := t.input[t.pos]
	switch {
	case ch == ',':
		t.pos++
		return Token{Type: TokenComma, Literal: ","}, nil
	case ch == '(':
		t.pos++
		return Token{Type: TokenLParen, Literal: "("}, nil
	case ch == ')':
		t.pos++
		return Token{Type: TokenRParen, Literal: ")"}, nil
	case ch == '=':
		t.pos++
		return Token{Type: TokenEqual, Literal: "="}, nil
	case ch == '*':
		t.pos++
		return Token{Type: TokenAsterisk, Literal: "*"}, nil
	case ch == ';':
		t.pos++
		return Token{Type: TokenSemicolon, Literal: ";"}, nil
	case ch == '\'':
		return t.scanString()
	case unicode.IsLetter(ch) || ch == '_' || unicode.IsDigit(ch):
		return t.scanIdentifier()
	default:
		return Token{}, fmt.Errorf("unexpected character %q", ch)
	}
}

func (t *Tokenizer) scanString() (Token, error) {
	t.pos++ // skip opening quote
	var sb strings.Builder
	for t.pos < len(t.input) {
		ch := t.input[t.pos]
		if ch == '\'' {
			if t.pos+1 < len(t.input) && t.input[t.pos+1] == '\'' {
				sb.WriteRune('\'')
				t.pos += 2
				continue
			}
			t.pos++
			return Token{Type: TokenString, Literal: sb.String()}, nil
		}
		sb.WriteRune(ch)
		t.pos++
	}
	return Token{}, fmt.Errorf("unterminated string literal")
}

func (t *Tokenizer) scanIdentifier() (Token, error) {
	start := t.pos
	for t.pos < len(t.input) {
		ch := t.input[t.pos]
		if unicode.IsLetter(ch) || unicode.IsDigit(ch) || ch == '_' || ch == '.' {
			t.pos++
			continue
		}
		break
	}
	literal := string(t.input[start:t.pos])
	upper := normalizeIdent(literal)
	if _, ok := keywords[upper]; ok {
		return Token{Type: TokenIdentifier, Literal: upper}, nil
	}
	return Token{Type: TokenIdentifier, Literal: literal}, nil
}

func (t *Tokenizer) skipWhitespace() {
	for t.pos < len(t.input) {
		if unicode.IsSpace(t.input[t.pos]) {
			t.pos++
			continue
		}
		break
	}
}
