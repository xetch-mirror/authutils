#include "vfs_stdio.h"
#include "vfs.h"      /* your real vfs.h */
#include <stdio.h>    /* only for vsnprintf's formatting logic */
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#define WRITE_BUF_SIZE 512

VFILE *vfs_fopen(const char *path, const char *mode) {
    VFILE *f = malloc(sizeof(VFILE));
    if (!f) return 0;

    int write_mode = (mode[0] == 'w');
    f->mode = write_mode;

    /* TODO: match your real vfs_open() signature/flags exactly —
       this assumes something like vfs_open(path, flags) -> vfs_file_t* */
    int flags = write_mode ? (VFS_FILE /* | VFS_CREATE, if you have it */) : VFS_FILE;
    f->vfs_file = vfs_open(path, flags);

    if (!f->vfs_file) {
        free(f);
        return 0;
    }
    return f;
}

int vfs_fclose(VFILE *f) {
    if (!f) return -1;
    /* TODO: call your real vfs_close(f->vfs_file) if one exists */
    free(f);
    return 0;
}

int vfs_fgetc(VFILE *f) {
    unsigned char c;
    int n = vfs_read(f->vfs_file, &c, 1); /* TODO: match real signature */
    if (n != 1) return -1; /* EOF */
    return (int)c;
}

char *vfs_fgets(char *buf, int size, VFILE *f) {
    int i = 0;
    while (i < size - 1) {
        int c = vfs_fgetc(f);
        if (c < 0) break;
        buf[i++] = (char)c;
        if (c == '\n') break;
    }
    if (i == 0) return 0; /* nothing read = EOF */
    buf[i] = '\0';
    return buf;
}

int vfs_fprintf(VFILE *f, const char *fmt, ...) {
    char tmp[WRITE_BUF_SIZE];
    va_list args;
    va_start(args, fmt);
    int len = vsnprintf(tmp, sizeof(tmp), fmt, args);
    va_end(args);

    if (len <= 0) return 0;

    /* TODO: match your real vfs_write() signature */
    return vfs_write(f->vfs_file, tmp, len);
}