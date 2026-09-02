#ifndef SUDOPARSER_H
#define SUDOPARSER_H

/* returns 1 if the current user is permitted to run sudo, 0 otherwise */
int sudoparser_check(void);

#endif