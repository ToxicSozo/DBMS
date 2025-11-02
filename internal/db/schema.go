package db

import (
	"encoding/json"
	"fmt"
	"os"
)

// Schema describes database layout loaded from a JSON file.
type Schema struct {
	Name        string              `json:"name"`
	TuplesLimit int                 `json:"tuples_limit"`
	Structure   map[string][]string `json:"structure"`
}

// LoadSchema reads schema definition from the provided file path.
func LoadSchema(path string) (Schema, error) {
	data, err := os.ReadFile(path)
	if err != nil {
		return Schema{}, fmt.Errorf("read schema: %w", err)
	}

	var schema Schema
	if err := json.Unmarshal(data, &schema); err != nil {
		return Schema{}, fmt.Errorf("parse schema: %w", err)
	}

	if len(schema.Structure) == 0 {
		return Schema{}, fmt.Errorf("schema structure is empty")
	}

	for table, cols := range schema.Structure {
		if len(cols) == 0 {
			return Schema{}, fmt.Errorf("table %q has no columns defined", table)
		}
	}

	return schema, nil
}

// Columns returns a copy of the column list for the provided table name.
func (s Schema) Columns(table string) ([]string, error) {
	cols, ok := s.Structure[table]
	if !ok {
		return nil, fmt.Errorf("unknown table %q", table)
	}

	out := make([]string, len(cols))
	copy(out, cols)
	return out, nil
}
