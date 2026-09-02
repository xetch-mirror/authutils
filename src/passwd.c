#include "userlib.h"
#include "pass.h"
#include <stdio.h>
#include <string.h>

/* Changes the password for the current user (or any user, if current
   user is admin). Minimal — no old-password re-entry confirmation yet. */
int passwd_main(const char *target_name, const char *new_password) {
    user_t *caller = userlib_find_by_uid(userlib_current_uid());
    user_t *target = userlib_find_by_name(target_name);

    if (!target) {
        printf("passwd: user '%s' not found\n", target_name);
        return 1;
    }

    if (caller->uid != target->uid && !caller->is_admin) {
        printf("passwd: permission denied\n");
        return 1;
    }

    pass_hash(new_password, target->passhash, sizeof(target->passhash));
    printf("passwd: password updated for '%s'\n", target->name);
    return 0;
}