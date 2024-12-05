package resources

import (
	"encoding/json"

	"github.com/Hollywood-Kid/DBMS/internal/data_structures/hmap"
)

type Schema struct {
	Name        string        `json:"name"`
	TuplesLimit int           `json:"tuples_limit"`
	Structure   *hmap.HashMap `json:"structure"`
}

func (s *Schema) UnmarshalJSON(data []byte) error {
	// Структура, которая будет использоваться для десериализации JSON в промежуточный формат
	type Alias Schema

	// Промежуточная структура для десериализации
	aux := &struct {
		Structure map[string][]string `json:"structure"`
		*Alias
	}{
		Alias: (*Alias)(s),
	}

	// Десериализация JSON в промежуточную структуру
	if err := json.Unmarshal(data, &aux); err != nil {
		return err
	}

	// Создаем хэш-таблицу
	s.Structure = hmap.NewHashMap()

	// Переносим данные из обычной карты в хэш-таблицу
	for key, value := range aux.Structure {
		s.Structure.Insert(key, value)
	}

	return nil
}
