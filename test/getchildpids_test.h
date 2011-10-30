#ifndef _GETCHILDPIDS_TEST_H
#define _GETCHILDPIDS_TEST_H
#include <sys/types.h>

int getchildpids_test ( int actualChildrenCount, pid_t actualChildren[],
		pid_t parentParameter, int sizeParameter, pid_t listParameter[] );

#endif /* _GETCHILDPIDS_TEST_H */
