#include "sudoparser.h"
#include "userlib.h"

int sudoparser_check(void) {
    user_t *u = userlib_find_by_uid(userlib_current_uid());
    if (!u) return 0;
    return u->is_admin;
}