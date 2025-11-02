package db

import (
	"encoding/csv"
	"errors"
	"fmt"
	"io"
	"os"
	"path/filepath"
	"sort"
	"strconv"
)

const (
	metaExt = ".meta"
	dataExt = ".csv"
)

// Database works with CSV-based table storage described by the Schema.
type Database struct {
	schema  Schema
	dataDir string
}

// NewDatabase creates a database using the given schema and data directory. The directory is
// created if it does not exist. Each table is initialized with a header row when needed.
func NewDatabase(schema Schema, dataDir string) (*Database, error) {
	if err := os.MkdirAll(dataDir, 0o755); err != nil {
		return nil, fmt.Errorf("create data directory: %w", err)
	}

	db := &Database{schema: schema, dataDir: dataDir}
	if err := db.ensureTables(); err != nil {
		return nil, err
	}
	return db, nil
}

func (d *Database) ensureTables() error {
	for table, cols := range d.schema.Structure {
		if err := d.ensureTableFile(table, cols); err != nil {
			return err
		}
	}
	return nil
}

func (d *Database) ensureTableFile(table string, columns []string) error {
	path := d.tablePath(table)
	if _, err := os.Stat(path); errors.Is(err, os.ErrNotExist) {
		f, err := os.Create(path)
		if err != nil {
			return fmt.Errorf("create table %q: %w", table, err)
		}
		defer f.Close()

		w := csv.NewWriter(f)
		header := append([]string{"id"}, columns...)
		if err := w.Write(header); err != nil {
			return fmt.Errorf("write header: %w", err)
		}
		w.Flush()
		if err := w.Error(); err != nil {
			return err
		}
	}
	return nil
}

func (d *Database) tablePath(table string) string {
	file := fmt.Sprintf("%s%s", table, dataExt)
	return filepath.Join(d.dataDir, file)
}

func (d *Database) metaPath(table string) string {
	file := fmt.Sprintf("%s%s", table, metaExt)
	return filepath.Join(d.dataDir, file)
}

// Insert adds a new row to the specified table.
func (d *Database) Insert(table string, columns, values []string) (int64, error) {
	tableCols, err := d.schema.Columns(table)
	if err != nil {
		return 0, err
	}

	if len(columns) != len(values) {
		return 0, fmt.Errorf("columns count (%d) does not match values count (%d)", len(columns), len(values))
	}

	colIndex := make(map[string]int, len(tableCols))
	for i, c := range tableCols {
		colIndex[c] = i
	}

	record := make([]string, len(tableCols))
	for i, col := range columns {
		idx, ok := colIndex[col]
		if !ok {
			return 0, fmt.Errorf("unknown column %q for table %q", col, table)
		}
		record[idx] = values[i]
	}

	id, err := d.nextID(table)
	if err != nil {
		return 0, err
	}

	path := d.tablePath(table)
	f, err := os.OpenFile(path, os.O_APPEND|os.O_WRONLY, 0)
	if err != nil {
		return 0, fmt.Errorf("open table: %w", err)
	}
	defer f.Close()

	row := append([]string{strconv.FormatInt(id, 10)}, record...)
	w := csv.NewWriter(f)
	if err := w.Write(row); err != nil {
		return 0, fmt.Errorf("write row: %w", err)
	}
	w.Flush()
	if err := w.Error(); err != nil {
		return 0, err
	}

	if err := d.storeNextID(table, id+1); err != nil {
		return 0, err
	}

	return id, nil
}

// Select returns selected column names and rows matching the provided predicate. When columns is
// empty or contains "*", all columns including the primary key are returned in order.
func (d *Database) Select(table string, columns []string, predicate func(map[string]string) bool) ([]string, []map[string]string, error) {
	tableCols, err := d.schema.Columns(table)
	if err != nil {
		return nil, nil, err
	}

	wantAll := len(columns) == 0 || (len(columns) == 1 && columns[0] == "*")
	selected := make([]string, 0, len(columns))
	if wantAll {
		selected = append([]string{"id"}, tableCols...)
	} else {
		colSet := make(map[string]struct{}, len(tableCols)+1)
		colSet["id"] = struct{}{}
		for _, c := range tableCols {
			colSet[c] = struct{}{}
		}

		for _, c := range columns {
			if _, ok := colSet[c]; !ok {
				return nil, nil, fmt.Errorf("unknown column %q for table %q", c, table)
			}
			selected = append(selected, c)
		}
	}

	path := d.tablePath(table)
	f, err := os.Open(path)
	if err != nil {
		return nil, nil, fmt.Errorf("open table: %w", err)
	}
	defer f.Close()

	r := csv.NewReader(f)
	header, err := r.Read()
	if err != nil {
		return nil, nil, fmt.Errorf("read header: %w", err)
	}

	index := make(map[string]int, len(header))
	for i, name := range header {
		index[name] = i
	}

	var rows []map[string]string
	for {
		rec, err := r.Read()
		if err != nil {
			if errors.Is(err, io.EOF) {
				break
			}
			return nil, nil, fmt.Errorf("read row: %w", err)
		}

		row := make(map[string]string, len(header))
		for name, idx := range index {
			if idx < len(rec) {
				row[name] = rec[idx]
			}
		}

		if predicate != nil && !predicate(row) {
			continue
		}

		out := make(map[string]string, len(selected))
		for _, c := range selected {
			out[c] = row[c]
		}
		rows = append(rows, out)
	}

	colsCopy := make([]string, len(selected))
	copy(colsCopy, selected)
	return colsCopy, rows, nil
}

// Delete removes rows that match the provided predicate and returns the number of deleted rows.
func (d *Database) Delete(table string, predicate func(map[string]string) bool) (int, error) {
	path := d.tablePath(table)
	f, err := os.Open(path)
	if err != nil {
		return 0, fmt.Errorf("open table: %w", err)
	}
	r := csv.NewReader(f)
	header, err := r.Read()
	if err != nil {
		f.Close()
		return 0, fmt.Errorf("read header: %w", err)
	}

	var kept [][]string
	var deleted int
	for {
		rec, err := r.Read()
		if err != nil {
			if errors.Is(err, io.EOF) {
				break
			}
			f.Close()
			return 0, fmt.Errorf("read row: %w", err)
		}

		row := make(map[string]string, len(header))
		for i, name := range header {
			if i < len(rec) {
				row[name] = rec[i]
			}
		}

		if predicate != nil && predicate(row) {
			deleted++
			continue
		}
		kept = append(kept, rec)
	}
	f.Close()

	tmpPath := path + ".tmp"
	out, err := os.Create(tmpPath)
	if err != nil {
		return 0, fmt.Errorf("create tmp file: %w", err)
	}

	w := csv.NewWriter(out)
	if err := w.Write(header); err != nil {
		out.Close()
		return 0, fmt.Errorf("write header: %w", err)
	}
	for _, rec := range kept {
		if err := w.Write(rec); err != nil {
			out.Close()
			return 0, fmt.Errorf("write row: %w", err)
		}
	}
	w.Flush()
	if err := w.Error(); err != nil {
		out.Close()
		return 0, err
	}
	if err := out.Close(); err != nil {
		return 0, err
	}

	if err := os.Rename(tmpPath, path); err != nil {
		return 0, fmt.Errorf("replace table file: %w", err)
	}

	return deleted, nil
}

func (d *Database) nextID(table string) (int64, error) {
	metaPath := d.metaPath(table)
	data, err := os.ReadFile(metaPath)
	if err != nil {
		if errors.Is(err, os.ErrNotExist) {
			if err := d.storeNextID(table, 1); err != nil {
				return 0, err
			}
			return 1, nil
		}
		return 0, fmt.Errorf("read meta: %w", err)
	}

	value, err := strconv.ParseInt(string(data), 10, 64)
	if err != nil {
		return 0, fmt.Errorf("parse meta value: %w", err)
	}
	return value, nil
}

func (d *Database) storeNextID(table string, id int64) error {
	metaPath := d.metaPath(table)
	tmp := metaPath + ".tmp"
	if err := os.WriteFile(tmp, []byte(strconv.FormatInt(id, 10)), 0o644); err != nil {
		return fmt.Errorf("write meta: %w", err)
	}
	return os.Rename(tmp, metaPath)
}

// ListTables returns all table names sorted alphabetically.
func (d *Database) ListTables() []string {
	tables := make([]string, 0, len(d.schema.Structure))
	for table := range d.schema.Structure {
		tables = append(tables, table)
	}
	sort.Strings(tables)
	return tables
}
