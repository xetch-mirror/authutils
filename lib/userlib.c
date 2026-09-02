#include "userlib.h"
#include <string.h>

static user_t g_users[USER_MAX_ACCOUNTS];
static int    g_current_uid = 0; /* uid 0 = root/admin, matches Unix convention */
static int    g_next_uid = 0;

void userlib_init(void) {
    memset(g_users, 0, sizeof(g_users));
    g_next_uid = 0;
    g_current_uid = 0;
}

int userlib_add(const char *name, const char *passhash, int is_admin) {
    for (int i = 0; i < USER_MAX_ACCOUNTS; i++) {
        if (!g_users[i].in_use) {
            g_users[i].uid = g_next_uid++;
            strncpy(g_users[i].name, name, USER_NAME_LEN - 1);
            strncpy(g_users[i].passhash, passhash, USER_HASH_LEN - 1);
            g_users[i].is_admin = is_admin;
            g_users[i].in_use = 1;
            return g_users[i].uid;
        }
    }
    return -1; /* table full */
}

user_t *userlib_find_by_name(const char *name) {
    for (int i = 0; i < USER_MAX_ACCOUNTS; i++) {
        if (g_users[i].in_use && strcmp(g_users[i].name, name) == 0) {
            return &g_users[i];
        }
    }
    return 0;
}

user_t *userlib_find_by_uid(int uid) {
    for (int i = 0; i < USER_MAX_ACCOUNTS; i++) {
        if (g_users[i].in_use && g_users[i].uid == uid) {
            return &g_users[i];
        }
    }
    return 0;
}

int userlib_current_uid(void) {
    return g_current_uid;
}

void userlib_set_current_uid(int uid) {
    g_current_uid = uid;
}