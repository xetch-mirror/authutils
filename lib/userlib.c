#include "userlib.h"
#include "iniparser.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define USER_MAX 16

static user_t g_users[USER_MAX];
static char   g_passwords[USER_MAX][USER_PASS_LEN];
static int    g_user_count = 0;
static int    g_current_uid = 0;

void userlib_init(const char *ini_path) {
    g_user_count = 0;
    g_current_uid = 0;

    dictionary *ini = iniparser_load(ini_path);
    if (!ini) return; /* no user.ini yet — empty table */

    int nsec = iniparser_getnsec(ini);
    for (int i = 0; i < nsec && g_user_count < USER_MAX; i++) {
        const char *section = iniparser_getsecname(ini, i);

        char key[64];
        snprintf(key, sizeof(key), "%s:uid", section);
        int uid = iniparser_getint(ini, key, -1);

        snprintf(key, sizeof(key), "%s:password", section);
        const char *pw = iniparser_getstring(ini, key, "");

        snprintf(key, sizeof(key), "%s:admin", section);
        int admin = iniparser_getboolean(ini, key, 0);

        user_t *u = &g_users[g_user_count];
        u->uid = uid;
        strncpy(u->name, section, USER_NAME_LEN - 1);
        u->is_admin = admin;
        strncpy(g_passwords[g_user_count], pw, USER_PASS_LEN - 1);
        g_user_count++;
    }

    iniparser_freedict(ini);
}

user_t *userlib_find_by_name(const char *name) {
    for (int i = 0; i < g_user_count; i++) {
        if (strcmp(g_users[i].name, name) == 0) return &g_users[i];
    }
    return 0;
}

user_t *userlib_find_by_uid(int uid) {
    for (int i = 0; i < g_user_count; i++) {
        if (g_users[i].uid == uid) return &g_users[i];
    }
    return 0;
}

const char *userlib_get_password(const char *name) {
    for (int i = 0; i < g_user_count; i++) {
        if (strcmp(g_users[i].name, name) == 0) return g_passwords[i];
    }
    return 0;
}

int userlib_set_password(const char *name, const char *new_password) {
    for (int i = 0; i < g_user_count; i++) {
        if (strcmp(g_users[i].name, name) == 0) {
            strncpy(g_passwords[i], new_password, USER_PASS_LEN - 1);
            g_passwords[i][USER_PASS_LEN - 1] = '\0';
            return 1;
        }
    }
    return 0; /* user not found */
}

/* Serializes the in-memory table back to INI format. Overwrites the
   whole file — same shape as what userlib_init read in. */
int userlib_write(const char *ini_path) {
    FILE *fp = fopen(ini_path, "w");
    if (!fp) return 0;

    for (int i = 0; i < g_user_count; i++) {
        fprintf(fp, "[%s]\n", g_users[i].name);
        fprintf(fp, "uid = %d\n", g_users[i].uid);
        fprintf(fp, "password = %s\n", g_passwords[i]);
        fprintf(fp, "admin = %d\n", g_users[i].is_admin);
        fprintf(fp, "\n");
    }

    fclose(fp);
    return 1;
}

int userlib_current_uid(void) {
    return g_current_uid;
}

void userlib_set_current_uid(int uid) {
    g_current_uid = uid;
}