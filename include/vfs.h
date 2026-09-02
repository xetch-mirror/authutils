#ifndef VFS_H
#define VFS_H

#include <stdint.h>

#define VFS_NAME_MAX   64
#define VFS_MAX_PATH   256
#define VFS_MAX_NODES  128
#define VFS_MAX_FILES  128

// RETURN
#define VFS_OK          0
#define VFS_ERROR      -1
#define VFS_ENOENT     -2 
#define VFS_ENOMEM     -3 

// File/Node Type Flags
#define VFS_FILE       (1 << 0)  // 0x01
#define VFS_DIRECTORY  (1 << 1)  // 0x02
#define VFS_BLOCKDEV   (1 << 2)  // 0x04

static inline int vfs_strcmp(const char *s1, const char *s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(const unsigned char *)s1 - *(const unsigned char *)s2;
}

struct vfs_node;

typedef struct {
    int (*read)(struct vfs_node *node, uint32_t offset, void *buffer, uint32_t size);
    int (*write)(struct vfs_node *node, uint32_t offset, const void *buffer, uint32_t size);
} vfs_ops_t;

typedef struct vfs_node {
    char name[VFS_MAX_PATH];
    uint32_t size;
    uint32_t flags;          // REPLACED
    uint32_t inode_id;       
    void *priv_data;         
    vfs_ops_t *ops;          
} vfs_node_t;

typedef struct {
    vfs_node_t *node;
    uint32_t offset;
    uint32_t flags;
} vfs_file_t;

// Function 
int vfs_mkdir(const char *path);
int vfs_exists(const char *path);
vfs_node_t *vfs_lookup(const char *path);
int vfs_register_node(vfs_node_t node);
void vfs_init(void);
void vfs_list_dir(const char *path);

// OPERATION
static inline int vfs_open(const char *path, vfs_file_t *out_file) {
    vfs_node_t *node = vfs_lookup(path);
    if (!node) return VFS_ENOENT;

    out_file->node = node;
    out_file->offset = 0;
    out_file->flags = 0;
    return VFS_OK;
}

static inline int vfs_read(vfs_file_t *file, void *buffer, uint32_t size) {
    if (!file || !file->node || !file->node->ops || !file->node->ops->read) return VFS_ERROR;
    if (file->offset >= file->node->size) return 0; 
    if (file->offset + size > file->node->size) size = file->node->size - file->offset;

    int bytes_read = file->node->ops->read(file->node, file->offset, buffer, size);
    if (bytes_read > 0) file->offset += bytes_read;
    return bytes_read;
}

static inline int vfs_write(vfs_file_t *file, const void *buffer, uint32_t size) {
    if (!file || !file->node || !file->node->ops || !file->node->ops->write) return VFS_ERROR;

    int bytes_written = file->node->ops->write(file->node, file->offset, buffer, size);
    if (bytes_written > 0) {
        file->offset += bytes_written;
        if (file->offset > file->node->size) file->node->size = file->offset;
    }
    return bytes_written;
}

#endif