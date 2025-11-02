package sql

import (
	"fmt"

	"github.com/ToxicSozo/DBMS/internal/db"
)

// Result represents the outcome of executing a SQL statement.
type Result struct {
	Columns []string
	Rows    [][]string
	Message string
}

// Execute runs the statement against the database and returns a Result.
func Execute(database *db.Database, stmt Statement) (*Result, error) {
	switch s := stmt.(type) {
	case *SelectStatement:
		predicate := buildPredicate(s.Where)
		cols, rows, err := database.Select(s.Table, s.Columns, predicate)
		if err != nil {
			return nil, err
		}
		formatted := make([][]string, len(rows))
		for i, row := range rows {
			formatted[i] = make([]string, len(cols))
			for j, col := range cols {
				formatted[i][j] = row[col]
			}
		}
		return &Result{Columns: cols, Rows: formatted}, nil
	case *InsertStatement:
		id, err := database.Insert(s.Table, s.Columns, s.Values)
		if err != nil {
			return nil, err
		}
		return &Result{Message: fmt.Sprintf("Inserted row with id %d", id)}, nil
	case *DeleteStatement:
		predicate := buildPredicate(s.Where)
		count, err := database.Delete(s.Table, predicate)
		if err != nil {
			return nil, err
		}
		return &Result{Message: fmt.Sprintf("Deleted %d row(s)", count)}, nil
	default:
		return nil, fmt.Errorf("unsupported statement type %T", stmt)
	}
}

func buildPredicate(expr Expression) func(map[string]string) bool {
	if expr == nil {
		return nil
	}
	return func(row map[string]string) bool {
		return expr.Evaluate(row)
	}
}
