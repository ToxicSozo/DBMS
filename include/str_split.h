#ifndef STR_SPLIT_H
#define STR_SPLIT_H

#include <stddef.h>
#include "list.h"

List* str_split(const char* s, const char* delim);

#endif