#include "userlib.h"
#include "sudoparser.h"
#include <stdio.h>

/* Runs a single command as uid 0 if permitted.
   No process model yet, so "running as root" just means
   temporarily flipping g_current_uid for the call. */
int sudo_run(const char *argv0, int (*cmd_main)(int, char **), int argc, char **argv) {
    if (!sudoparser_check()) {
        printf("sudo: %s is not in the sudoers list\n", argv0);
        return 1;
    }

    int saved_uid = userlib_current_uid();
    userlib_set_current_uid(0); /* elevate */

    int result = cmd_main(argc, argv);

    userlib_set_current_uid(saved_uid); /* always restore */
    return result;
}