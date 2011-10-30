#ifndef _GETCHILDPIDS_TEST_H
#define _GETCHILDPIDS_TEST_H
#include <sys/types.h>

int getchildpids_test ( pid_t parent, int trueChildrenCount,
		pid_t trueChildren[] );

#endif /* _GETCHILDPIDS_TEST_H */
