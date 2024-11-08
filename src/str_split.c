#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "../include/str_split.h"

char** str_split(char* a_str, const char* a_delims[], size_t delim_count)
{
    char** result = 0;
    size_t count = 0;
    char* tmp = a_str;
    char* last_comma = 0;
    size_t str_len = strlen(a_str);

    // Count the number of tokens
    while (*tmp)
    {
        for (size_t i = 0; i < delim_count; i++)
        {
            size_t delim_len = strlen(a_delims[i]);
            if (strncmp(tmp, a_delims[i], delim_len) == 0)
            {
                count++;
                last_comma = tmp;
                tmp += delim_len;
                break;
            }
        }
        tmp++;
    }

    count += last_comma < (a_str + str_len - 1);
    count++;

    result = malloc(sizeof(char*) * count);

    if (result)
    {
        size_t idx = 0;
        char* token_start = a_str;
        tmp = a_str;

        while (*tmp)
        {
            for (size_t i = 0; i < delim_count; i++)
            {
                size_t delim_len = strlen(a_delims[i]);
                if (strncmp(tmp, a_delims[i], delim_len) == 0)
                {
                    size_t token_len = tmp - token_start;
                    result[idx] = malloc(token_len + 1);
                    strncpy(result[idx], token_start, token_len);
                    result[idx][token_len] = '\0';
                    idx++;
                    tmp += delim_len;
                    token_start = tmp;
                    break;
                }
            }
            tmp++;
        }

        // Add the last token
        size_t token_len = tmp - token_start;
        result[idx] = malloc(token_len + 1);
        strncpy(result[idx], token_start, token_len);
        result[idx][token_len] = '\0';
        idx++;

        result[idx] = 0;
    }

    return result;
}

void free_split(char **split_res) {
    if (split_res)
    {
        for (int i = 0; split_res[i]; i++)
        {
            free(split_res[i]);
        }
        free(split_res);
    }
}