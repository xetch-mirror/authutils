#ifndef USERLIB_H
#define USERLIB_H

#define USER_MAX_ACCOUNTS 16
#define USER_NAME_LEN     32
#define USER_HASH_LEN     32

typedef struct {
    int  uid;
    char name[USER_NAME_LEN];
    char passhash[USER_HASH_LEN];
    int  is_admin;
    int  in_use;
} user_t;

void   userlib_init(void);
int    userlib_add(const char *name, const char *passhash, int is_admin);
user_t *userlib_find_by_name(const char *name);
user_t *userlib_find_by_uid(int uid);
int    userlib_current_uid(void);
void   userlib_set_current_uid(int uid);

#endif