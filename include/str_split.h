#ifndef STR_SPLIT_H
#define STR_SPLIT_H

char** str_split(char* a_str, const char* a_delims[], size_t delim_count);
void free_split(char **split_res);

#endif 