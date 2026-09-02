#ifndef VFS_STDIO_H
#define VFS_STDIO_H

#include <stddef.h>

/* Minimal FILE* replacement backed by vfs.h instead of a real libc.
   Only supports what iniparser/userlib actually use: open, read-line,
   write, close. Not a general stdio implementation. */
typedef struct {
    void *vfs_file;   /* actual vfs_file_t* from your vfs.h */
    int   mode;       /* 0 = read, 1 = write */
} VFILE;

VFILE *vfs_fopen(const char *path, const char *mode);
int    vfs_fclose(VFILE *f);
int    vfs_fgetc(VFILE *f);
char  *vfs_fgets(char *buf, int size, VFILE *f);
int    vfs_fprintf(VFILE *f, const char *fmt, ...);

#endif