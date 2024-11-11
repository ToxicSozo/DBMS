#ifndef STR_SPLIT_H
#define STR_SPLIT_H

#include <stddef.h>

char** str_split( const char *s, const char *delim );
char** strsplit_count( const char* s, const char* delim, size_t* nb );
void free_split(char **split_res);

#endif 