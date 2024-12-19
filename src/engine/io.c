#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdbool.h>

#include "io.h"
#include "util.h"

// 20 Mib limit
#define IO_READ_CHUNK_SIZE 20 * 1024 * 1024 // 20971520
#define IO_READ_ERROR_GENERAL "Error reading file: %s. err: %d\n"
#define IO_READ_ERROR_MEMORY "Not enough memory to read file: %s\n"

File io_file_read(const char *path) {
    File file = { .is_valid = false };

    FILE *fp = fopen(path, "rb");
    if (ferror(fp)) {
        ERROR_RETURN(file, IO_READ_ERROR_GENERAL, path, errno);
    }

    char *data = NULL;
    char *tmp;
    size_t used = 0;
    size_t len = 0;
    size_t n;

    while (true) {
        if (used + IO_READ_CHUNK_SIZE + 1 > len) { // +1 or *2
            len = used + IO_READ_CHUNK_SIZE + 1;

            if (len <= used) {
                free(data);
                ERROR_RETURN(file, "Input file is too large: %s\n", path);
            }

            tmp = realloc(data, len);
            if (!tmp) {
                free(data);
                ERROR_RETURN(file, IO_READ_ERROR_MEMORY, path);
            }
            data = tmp;
        }

        n = fread(data + used, 1, IO_READ_CHUNK_SIZE, fp);
        if (n == 0)
            break;
        
        used += n;
    }

    if (ferror(fp)) {
        free(data);
        ERROR_RETURN(file, IO_READ_ERROR_GENERAL, path, errno);
    }

    tmp = realloc(data, used + 1);
    if (!tmp) {
        free(data);
        ERROR_RETURN(file, IO_READ_ERROR_MEMORY, path);
    }

    data = tmp;
    data[used] = 0;

    file.data = data;
    file.len = len;
    file.is_valid = true;

    return file;
}

int io_file_write(void *buffer, size_t len, const char *path) {
    return 0;
}
