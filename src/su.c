#include "userlib.h"
#include "pass.h"
#include <stdio.h>
#include <string.h>

/* Switch to another user's identity after verifying their password.
   Thin wrapper: does not duplicate sudo's own privilege-check logic,
   just changes current_uid once auth succeeds. */
int su_main(const char *target_name, const char *password) {
    user_t *u = userlib_find_by_name(target_name);
    if (!u) {
        printf("su: user '%s' not found\n", target_name);
        return 1;
    }

    if (!pass_verify(password, u->passhash)) {
        printf("su: authentication failure\n");
        return 1;
    }

    userlib_set_current_uid(u->uid);
    printf("su: switched to '%s'\n", u->name);
    return 0;
}