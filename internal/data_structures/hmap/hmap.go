package hmap

import (
	"fmt"

	"github.com/Hollywood-Kid/DBMS/internal/data_structures/hmap"
)

type HashMap struct {
	buckets []*Bucket
	size    int
	count   int
}

func NewHashMap() *HashMap {
	return &HashMap{
		buckets: make([]*Bucket, 1),
		size:    1,
		count:   0,
	}
}

// getBucketIndex вычисляет индекс бакета по ключу.
func (h *HashMap) getBucketIndex(key interface{}) int {
	hash := hmap.GetHash(key)

	return int(hash % uint64(len((h.buckets))))
}

// Insert вставляет новую пару ключ-значение в хэш-таблицу.
func (h *HashMap) Insert(key, value interface{}) {
	index := h.getBucketIndex(key)

	// Если бакет пустой, создаем новый
	if h.buckets[index] == nil {
		h.buckets[index] = NewBucket()
	}

	// Вставляем в бакет
	h.buckets[index].Insert(key, value)
	h.size++

	// Расширение таблицы, если превышен лимит
	if h.size > len(h.buckets)*8 {
		h.expand()
	}
}

// Find ищет значение по ключу в хэш-таблице.
func (h *HashMap) Find(key interface{}) (interface{}, bool) {
	index := h.getBucketIndex(key)

	// Если бакет пустой
	if h.buckets[index] == nil {
		return nil, false
	}

	// Ищем в бакете
	return h.buckets[index].Find(key)
}

// Remove удаляет пару ключ-значение из хэш-таблицы.
func (h *HashMap) Remove(key interface{}) bool {
	index := h.getBucketIndex(key)

	// Если бакет пустой
	if h.buckets[index] == nil {
		return false
	}

	// Удаляем из бакета
	if h.buckets[index].Remove(key) {
		h.size--
		return true
	}

	return false
}

// expand увеличивает размер хэш-таблицы.
func (h *HashMap) expand() {
	newBuckets := make([]*Bucket, len(h.buckets)*2)

	for _, bucket := range h.buckets {
		if bucket == nil {
			continue
		}
		for _, entry := range bucket.entries {
			newIndex := hmap.GetHash(entry.key) % uint64(len(newBuckets))
			if newBuckets[newIndex] == nil {
				newBuckets[newIndex] = NewBucket()
			}
			newBuckets[newIndex].Insert(entry.key, entry.value)
		}
	}

	h.buckets = newBuckets
}

// Print выводит содержимое хэш-таблицы.
func (h *HashMap) Print() {
	for i, bucket := range h.buckets {
		if bucket == nil || bucket.count == 0 {
			continue
		}
		fmt.Printf("Bucket %d:\n", i)
		for _, entry := range bucket.entries {
			fmt.Printf("  Key: %v, Value: %v\n", entry.key, entry.value)
		}
	}
}
