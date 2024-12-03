package resources

type Schema struct {
	Name        string              `json:"name"`
	TuplesLimit int                 `json:"tuples_limit"`
	Structure   map[string][]string `json:"structure"`
}
