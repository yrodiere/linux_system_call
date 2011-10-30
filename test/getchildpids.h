#ifndef _GETCHILDPIDS_H
#define _GETCHILDPIDS_H

#include <errno.h>

static int getchildpids ( pid_t parent_pid, int size, pid_t list[] )
{
	int res;

	__asm__ __volatile__ (
		"int	$0x80;"
		: "=a" (res)
		: "0" (338), "b" ((long)parent_pid), "c" ((long)size), "d" ((long)list)
		);

	if ( res < 0 )
	{
		errno = -res;
		return -1;
	}
	else
		return res;
}

#endif /* _GETCHILDPIDS_H */

