package json

import (
	"encoding/json"
	"fmt"
	"io/ioutil"
)

func parseJSON(filePath string) (Schema, error) {
	// Чтение файла
	data, err := ioutil.ReadFile(filePath)
	if err != nil {
		return Schema{}, fmt.Errorf("ошибка при чтении файла: %v", err)
	}

	// Десериализация JSON в структуру
	var schema Schema
	err = json.Unmarshal(data, &schema)
	if err != nil {
		return Schema{}, fmt.Errorf("ошибка при десериализации JSON: %v", err)
	}

	return schema, nil
}
