#include "userlib.h"
#include "pass.h"
#include <stdio.h>

#define USER_INI_PATH "user.ini"

/* Changes a password: verify old one via pass, update in-memory table,
   then serialize the whole table back to user.ini. */
int passwd_main(const char *target_name, const char *old_password, const char *new_password) {
    if (!pass_check(target_name, old_password)) {
        return 1; /* pass_check already printed "pass is incorrect" */
    }

    if (!userlib_set_password(target_name, new_password)) {
        printf("passwd: user '%s' not found\n", target_name);
        return 1;
    }

    if (!userlib_write(USER_INI_PATH)) {
        printf("passwd: failed to write user.ini\n");
        return 1;
    }

    printf("passwd: password updated for '%s'\n", target_name);
    return 0;
}