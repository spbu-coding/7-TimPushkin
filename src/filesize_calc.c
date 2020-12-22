#include <stdio.h>

#include "filesize_calc.h"


#if defined(unix) || defined(__unix__) || defined(__unix)

#include <sys/stat.h>

unsigned int calculate_filesize(FILE* const file, size_t* const filesize_container) {
    const int file_des = fileno(file);
    if (file_des == -1) {
        return 1;
    }
    struct stat file_stat;
    if (fstat(file_des, &file_stat)) {
        return 1;
    }
    *filesize_container = file_stat.st_size;
    return 0;
}

#else

unsigned int calculate_filesize(FILE* const file, size_t* const filesize_container) {
    const long initial_file_pos = ftell(file);
    if (initial_file_pos == -1) {
        return 1;
    }
    if (fseek(file, 0, SEEK_END)) {
        return 1;
    }
    const long filesize = ftell(file);
    if (filesize == -1) {
        return 1;
    }
    if (fseek(file, initial_file_pos, SEEK_SET)) {
        return 1;
    }
    *(filesize_container) = filesize;
    return 0;
}

#endif
