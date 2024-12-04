package hmap

import (
	"reflect"
)

// Entry представляет одну запись в бакете.
type Entry struct {
	key   interface{}
	value interface{}
}

// Bucket представляет бакет, который хранит до 8 элементов.
type Bucket struct {
	entries []Entry
	count   int
}

// NewBucket создает новый бакет.
func NewBucket() *Bucket {
	return &Bucket{
		entries: make([]Entry, 0, 8),
		count:   0,
	}
}

// Find ищет значение по ключу в бакете.
func (b *Bucket) Find(key interface{}) (interface{}, bool) {
	for i := 0; i < b.count; i++ {
		if reflect.DeepEqual(b.entries[i].key, key) {
			return b.entries[i].value, true
		}
	}
	return nil, false
}

// Insert вставляет новую пару ключ-значение в бакет.
func (b *Bucket) Insert(key, value interface{}) {
	// Обновляем значение, если ключ уже существует
	for i := 0; i < b.count; i++ {
		if reflect.DeepEqual(b.entries[i].key, key) {
			b.entries[i].value = value
			return
		}
	}

	// Если места достаточно, просто добавляем
	if b.count < 8 {
		b.entries = append(b.entries, Entry{key, value})
		b.count++
	}
}

// Remove удаляет ключ из бакета.
func (b *Bucket) Remove(key interface{}) bool {
	for i := 0; i < b.count; i++ {
		if reflect.DeepEqual(b.entries[i].key, key) {
			b.entries = append(b.entries[:i], b.entries[i+1:]...)
			b.count--
			return true
		}
	}
	return false
}
