package json

import (
	"encoding/json"
	"fmt"
	"io/ioutil"

	"github.com/Hollywood-Kid/DBMS/internal/static/resources"
)

func parseJSON(filePath string) (resources.Schema, error) {
	data, err := ioutil.ReadFile(filePath)
	if err != nil {
		return resources.Schema{}, fmt.Errorf("ошибка при чтении файла: %v", err)
	}

	var schema resources.Schema
	err = json.Unmarshal(data, &schema)
	if err != nil {
		return resources.Schema{}, fmt.Errorf("ошибка при десериализации JSON: %v", err)
	}

	return schema, nil
}
