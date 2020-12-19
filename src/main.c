#include <stdlib.h>
#include <stdio.h>

#include "str_handler.h"

#define close_file_after_err(file) if (fclose(file) != 0) fprintf(stderr, "Also failed to close the file\n")

#define STRS_TO_PRINT_NUM 100
#define PUNCTUATION_MARKS ".,;:!?"

unsigned int read_file(const char* const filename, char** const file_contents_container) {
    FILE* file_to_read = fopen(filename, "r");
    if (file_to_read == NULL) {
        fprintf(stderr, "Failed to open %s\n", filename);
        return 1;
    }
    if (fseek(file_to_read, 0, SEEK_END)) {
        fprintf(stderr, "Failed to read %s (fseek failed)\n", filename);
        close_file_after_err(file_to_read);
        return 1;
    }
    const long file_size = ftell(file_to_read);
    if (file_size == -1) {
        fprintf(stderr, "Failed to read %s (ftell failed)\n", filename);
        close_file_after_err(file_to_read);
        return 1;
    }
    if (fseek(file_to_read, 0, SEEK_SET)) {
        fprintf(stderr, "Failed to read %s (fseek failed)\n", filename);
        close_file_after_err(file_to_read);
        return 1;
    }
    *file_contents_container = (char*) malloc((file_size + 2) * sizeof(char));
    if (*file_contents_container == NULL) {
        fprintf(stderr, "Failed to allocate memory for contents of %s\n", filename);
        close_file_after_err(file_to_read);
        return 1;
    }
    if (file_size > 0 && fread(*file_contents_container, file_size, 1, file_to_read) != 1) {
        fprintf(stderr, "Failed to read %s (fread failed)\n", filename);
        free(*file_contents_container);
        close_file_after_err(file_to_read);
        return 1;
    }
    if (fclose(file_to_read) != 0) {
        fprintf(stderr, "Failed to close %s after reading\n", filename);
        free(*file_contents_container);
        return 1;
    }
    if (file_size > 0 && (*file_contents_container)[file_size - 1] != '\n') {
        (*file_contents_container)[file_size] = '\n';
        (*file_contents_container)[file_size + 1] = '\0';
    } else {
        (*file_contents_container)[file_size] = '\0';
    }
    return 0;
}

int main(const int argc, const char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Unexpected amount of arguments\n");
        return 1;
    }
    char* file_contents;
    if (read_file(argv[1], &file_contents)) {
        return 1;
    }
    remove_chars_from_str(&file_contents, PUNCTUATION_MARKS);
    struct strs_array_t strs_to_sort;
    if (split_str_into_lines(file_contents, &strs_to_sort)) {
        free(file_contents);
        return 1;
    }
    qsort(strs_to_sort.strs, strs_to_sort.size, sizeof(char *), cmp_strs_des);
    if (print_strs(&strs_to_sort, STRS_TO_PRINT_NUM)) {
        free(file_contents);
        free(strs_to_sort.strs);
        return 1;
    }
    free(file_contents);
    free(strs_to_sort.strs);
    return 0;
}
