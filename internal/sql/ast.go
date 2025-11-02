package sql

// Statement represents a parsed SQL statement.
type Statement interface {
	stmt()
}

// SelectStatement describes a SELECT query.
type SelectStatement struct {
	Columns []string
	Table   string
	Where   Expression
}

func (*SelectStatement) stmt() {}

// InsertStatement describes an INSERT command.
type InsertStatement struct {
	Table   string
	Columns []string
	Values  []string
}

func (*InsertStatement) stmt() {}

// DeleteStatement describes a DELETE command.
type DeleteStatement struct {
	Table string
	Where Expression
}

func (*DeleteStatement) stmt() {}

// Expression represents a boolean expression in a WHERE clause.
type Expression interface {
	Evaluate(row map[string]string) bool
	expr()
}

// ComparisonExpr represents a column equality comparison with a literal value.
type ComparisonExpr struct {
	Column string
	Value  string
}

func (*ComparisonExpr) expr() {}

// Evaluate checks if the row satisfies the comparison.
func (c *ComparisonExpr) Evaluate(row map[string]string) bool {
	return row[c.Column] == c.Value
}

// AndExpr represents logical AND between two expressions.
type AndExpr struct {
	Left  Expression
	Right Expression
}

func (*AndExpr) expr() {}

// Evaluate returns true if both operands are true.
func (a *AndExpr) Evaluate(row map[string]string) bool {
	return a.Left.Evaluate(row) && a.Right.Evaluate(row)
}

// OrExpr represents logical OR between two expressions.
type OrExpr struct {
	Left  Expression
	Right Expression
}

func (*OrExpr) expr() {}

// Evaluate returns true if any operand is true.
func (o *OrExpr) Evaluate(row map[string]string) bool {
	return o.Left.Evaluate(row) || o.Right.Evaluate(row)
}

// ParenExpr wraps an expression evaluated inside parentheses.
type ParenExpr struct {
	Inner Expression
}

func (*ParenExpr) expr() {}

// Evaluate delegates to the wrapped expression.
func (p *ParenExpr) Evaluate(row map[string]string) bool {
	if p.Inner == nil {
		return true
	}
	return p.Inner.Evaluate(row)
}
