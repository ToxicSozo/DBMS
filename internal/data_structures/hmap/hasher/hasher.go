package hasher

import (
	"encoding/json"
	"fmt"
)

// djb2 хэш-функция для вычисления хэша из []byte.
func djb2(data []byte) uint64 {
	hash := uint64(5381)
	for _, b := range data {
		hash = ((hash << 5) + hash) + uint64(b) // hash * 33 + b
	}
	return hash
}

// toBytes преобразует ключ в []byte для хэширования.
func ToBytes(key interface{}) []byte {
	switch v := key.(type) {
	case string:
		return []byte(v)
	case int, int64, uint64:
		return []byte(fmt.Sprintf("%d", v))
	case float64:
		return []byte(fmt.Sprintf("%f", v))
	default:
		// Используем JSON-сериализацию для сложных типов
		data, _ := json.Marshal(v)
		return data
	}
}

// GetHash вычисляет хэш для заданного ключа.
func GetHash(key interface{}) uint64 {
	return djb2(ToBytes(key))
}
