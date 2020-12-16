#ifndef STR_HANDLER_H
#define STR_HANDLER_H

#include <stdlib.h>

struct strs_array_t {
    char** strs;
    size_t size;
};

void remove_chars_from_str(char* const* str, const char* chars_to_remove);

unsigned int split_str_into_lines(char* str, struct strs_array_t* lines_container);

unsigned int print_strs(const struct strs_array_t* strs_to_print, size_t strs_to_print_limit);

int cmp_strs(const void* str1, const void* str2);

#endif
