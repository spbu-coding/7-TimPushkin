#include <stdio.h>
#include <string.h>

#include "str_handler.h"

void remove_chars_from_str(char* const* const str, const char* const chars_to_remove) {
    const size_t str_len = strlen(*str);
    size_t cur_char = 0, offset = 0;
    while (offset < str_len) {
        while (strchr(chars_to_remove, (*str)[offset]) != NULL) {
            offset++;
        }
        (*str)[cur_char++] = (*str)[offset++];
    }
    (*str)[cur_char] = '\0';
}

unsigned int split_str_into_lines(char* const str, struct strs_array_t* const lines_container) {
    lines_container->size = 0;
    for (size_t i = 0; str[i] != '\0'; i++) {
        if (str[i] == '\n') {
            lines_container->size++;
        }
    }
    lines_container->strs = (char**) malloc(lines_container->size * sizeof(char*));
    if (lines_container == NULL) {
        fprintf(stderr, "Failed to allocate memory for pointers to lines\n");
        return 1;
    }
    for (size_t char_ind = 0, line_beg = 0, line_num = 0; str[char_ind] != '\0'; char_ind++) {
        if (str[char_ind] == '\n') {
            str[char_ind] = '\0';
            lines_container->strs[line_num++] = &(str[line_beg]);
            line_beg = char_ind + 1;
        }
    }
    return 0;
}

unsigned int print_strs(const struct strs_array_t* const strs_to_print, const size_t strs_to_print_limit) {
    const size_t strs_to_print_num = (strs_to_print->size <= strs_to_print_limit) ? strs_to_print->size : strs_to_print_limit;
    for (size_t i = 0; i < strs_to_print_num; i++) {
        if (printf("%s\n", strs_to_print->strs[i]) < 0) {
            fprintf(stderr, "Failed to print one of the strings to stdout\n");
            return 1;
        }
    }
    return 0;
}

int cmp_strs(const void* const str1, const void* const str2) {
    return -strcmp(*((char* const*) str1), *((char* const*) str2));
}
