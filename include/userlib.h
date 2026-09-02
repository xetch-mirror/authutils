#ifndef USERLIB_H
#define USERLIB_H

#define USER_NAME_LEN 32
#define USER_PASS_LEN 32

typedef struct {
    int  uid;
    char name[USER_NAME_LEN];
    int  is_admin;
} user_t;

void userlib_init(const char *ini_path);
user_t *userlib_find_by_name(const char *name);
user_t *userlib_find_by_uid(int uid);
const char *userlib_get_password(const char *name);
int  userlib_set_password(const char *name, const char *new_password);
int  userlib_write(const char *ini_path);
int  userlib_current_uid(void);
void userlib_set_current_uid(int uid);

#endif