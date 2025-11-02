package sql

import "fmt"

// Parse parses the provided SQL string into a Statement.
func Parse(input string) (Statement, error) {
	parser, err := newParser(input)
	if err != nil {
		return nil, err
	}

	if parser.cur.Type == TokenEOF {
		return nil, fmt.Errorf("empty statement")
	}

	switch parser.cur.Literal {
	case "SELECT":
		return parser.parseSelect()
	case "INSERT":
		return parser.parseInsert()
	case "DELETE":
		return parser.parseDelete()
	default:
		return nil, fmt.Errorf("unexpected token %q", parser.cur.Literal)
	}
}

type parser struct {
	tokenizer *Tokenizer
	cur       Token
}

func newParser(input string) (*parser, error) {
	p := &parser{tokenizer: NewTokenizer(input)}
	if err := p.advance(); err != nil {
		return nil, err
	}
	return p, nil
}

func (p *parser) advance() error {
	tok, err := p.tokenizer.Next()
	if err != nil {
		return err
	}
	p.cur = tok
	return nil
}

func (p *parser) expectLiteral(lit string) error {
	if p.cur.Literal != lit {
		return fmt.Errorf("expected %s, got %s", lit, p.cur.Literal)
	}
	return nil
}

func (p *parser) expectType(t TokenType) error {
	if p.cur.Type != t {
		return fmt.Errorf("unexpected token %s", p.cur.Literal)
	}
	return nil
}

func (p *parser) parseSelect() (Statement, error) {
	if err := p.advance(); err != nil {
		return nil, err
	}

	columns, err := p.parseColumnList()
	if err != nil {
		return nil, err
	}

	if err := p.expectLiteral("FROM"); err != nil {
		return nil, err
	}
	if err := p.advance(); err != nil {
		return nil, err
	}

	if err := p.expectType(TokenIdentifier); err != nil {
		return nil, err
	}
	table := p.cur.Literal

	if err := p.advance(); err != nil {
		return nil, err
	}

	var where Expression
	if p.cur.Literal == "WHERE" {
		if err := p.advance(); err != nil {
			return nil, err
		}
		where, err = p.parseExpression()
		if err != nil {
			return nil, err
		}
	}

	if p.cur.Type == TokenSemicolon {
		if err := p.advance(); err != nil {
			return nil, err
		}
	}

	if p.cur.Type != TokenEOF {
		return nil, fmt.Errorf("unexpected token %s", p.cur.Literal)
	}

	return &SelectStatement{Columns: columns, Table: table, Where: where}, nil
}

func (p *parser) parseInsert() (Statement, error) {
	if err := p.advance(); err != nil {
		return nil, err
	}

	if err := p.expectLiteral("INTO"); err != nil {
		return nil, err
	}
	if err := p.advance(); err != nil {
		return nil, err
	}

	if err := p.expectType(TokenIdentifier); err != nil {
		return nil, err
	}
	table := p.cur.Literal

	if err := p.advance(); err != nil {
		return nil, err
	}

	if err := p.expectType(TokenLParen); err != nil {
		return nil, err
	}
	if err := p.advance(); err != nil {
		return nil, err
	}

	columns, err := p.parseIdentifierList()
	if err != nil {
		return nil, err
	}

	if err := p.expectType(TokenRParen); err != nil {
		return nil, err
	}
	if err := p.advance(); err != nil {
		return nil, err
	}

	if err := p.expectLiteral("VALUES"); err != nil {
		return nil, err
	}
	if err := p.advance(); err != nil {
		return nil, err
	}

	if err := p.expectType(TokenLParen); err != nil {
		return nil, err
	}
	if err := p.advance(); err != nil {
		return nil, err
	}

	values, err := p.parseValueList()
	if err != nil {
		return nil, err
	}

	if p.cur.Type == TokenSemicolon {
		if err := p.advance(); err != nil {
			return nil, err
		}
	}

	if p.cur.Type != TokenEOF {
		return nil, fmt.Errorf("unexpected token %s", p.cur.Literal)
	}

	return &InsertStatement{Table: table, Columns: columns, Values: values}, nil
}

func (p *parser) parseDelete() (Statement, error) {
	if err := p.advance(); err != nil {
		return nil, err
	}

	if err := p.expectLiteral("FROM"); err != nil {
		return nil, err
	}
	if err := p.advance(); err != nil {
		return nil, err
	}

	if err := p.expectType(TokenIdentifier); err != nil {
		return nil, err
	}
	table := p.cur.Literal

	if err := p.advance(); err != nil {
		return nil, err
	}

	var where Expression
	if p.cur.Literal == "WHERE" {
		if err := p.advance(); err != nil {
			return nil, err
		}
		w, err := p.parseExpression()
		if err != nil {
			return nil, err
		}
		where = w
	}

	if p.cur.Type == TokenSemicolon {
		if err := p.advance(); err != nil {
			return nil, err
		}
	}

	if p.cur.Type != TokenEOF {
		return nil, fmt.Errorf("unexpected token %s", p.cur.Literal)
	}

	return &DeleteStatement{Table: table, Where: where}, nil
}

func (p *parser) parseColumnList() ([]string, error) {
	if p.cur.Type == TokenAsterisk {
		if err := p.advance(); err != nil {
			return nil, err
		}
		return []string{"*"}, nil
	}
	return p.parseIdentifierList()
}

func (p *parser) parseIdentifierList() ([]string, error) {
	var items []string
	for {
		if p.cur.Type != TokenIdentifier {
			return nil, fmt.Errorf("expected identifier, got %s", p.cur.Literal)
		}
		items = append(items, p.cur.Literal)
		if err := p.advance(); err != nil {
			return nil, err
		}
		if p.cur.Type != TokenComma {
			break
		}
		if err := p.advance(); err != nil {
			return nil, err
		}
	}
	if len(items) == 0 {
		return nil, fmt.Errorf("expected at least one identifier")
	}
	return items, nil
}

func (p *parser) parseValueList() ([]string, error) {
	var values []string
	for {
		switch p.cur.Type {
		case TokenString:
			values = append(values, p.cur.Literal)
		case TokenIdentifier:
			values = append(values, p.cur.Literal)
		default:
			return nil, fmt.Errorf("expected value, got %s", p.cur.Literal)
		}

		if err := p.advance(); err != nil {
			return nil, err
		}
		if p.cur.Type != TokenComma {
			break
		}
		if err := p.advance(); err != nil {
			return nil, err
		}
	}

	if err := p.expectType(TokenRParen); err != nil {
		return nil, err
	}
	if err := p.advance(); err != nil {
		return nil, err
	}

	return values, nil
}

func (p *parser) parseExpression() (Expression, error) {
	left, err := p.parseTerm()
	if err != nil {
		return nil, err
	}

	for p.cur.Literal == "OR" {
		if err := p.advance(); err != nil {
			return nil, err
		}
		right, err := p.parseTerm()
		if err != nil {
			return nil, err
		}
		left = &OrExpr{Left: left, Right: right}
	}
	return left, nil
}

func (p *parser) parseTerm() (Expression, error) {
	left, err := p.parseFactor()
	if err != nil {
		return nil, err
	}

	for p.cur.Literal == "AND" {
		if err := p.advance(); err != nil {
			return nil, err
		}
		right, err := p.parseFactor()
		if err != nil {
			return nil, err
		}
		left = &AndExpr{Left: left, Right: right}
	}
	return left, nil
}

func (p *parser) parseFactor() (Expression, error) {
	switch p.cur.Type {
	case TokenLParen:
		if err := p.advance(); err != nil {
			return nil, err
		}
		expr, err := p.parseExpression()
		if err != nil {
			return nil, err
		}
		if err := p.expectType(TokenRParen); err != nil {
			return nil, err
		}
		if err := p.advance(); err != nil {
			return nil, err
		}
		return &ParenExpr{Inner: expr}, nil
	case TokenIdentifier:
		column := p.cur.Literal
		if err := p.advance(); err != nil {
			return nil, err
		}
		if err := p.expectType(TokenEqual); err != nil {
			return nil, err
		}
		if err := p.advance(); err != nil {
			return nil, err
		}
		if p.cur.Type != TokenString && p.cur.Type != TokenIdentifier {
			return nil, fmt.Errorf("expected literal value, got %s", p.cur.Literal)
		}
		value := p.cur.Literal
		if err := p.advance(); err != nil {
			return nil, err
		}
		return &ComparisonExpr{Column: column, Value: value}, nil
	default:
		return nil, fmt.Errorf("unexpected token %s in expression", p.cur.Literal)
	}
}
