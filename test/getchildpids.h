#ifndef _GETCHILDPIDS_H
#define _GETCHILDPIDS_H
/*
#include <sys/syscall.h>
#include <linux/unistd.h>

 * This macro doesn't work:
 * _syscall0(int, helloworld);
 */

static int getchildpids ( pid_t parent_pid, int size, pid_t list[] )
{
	int res;
	__asm__ __volatile__ (
		"int	$0x80;"
		: "=a" (res)
		: "0" (338), "b" ((long)parent_pid), "c" ((long)size), "d" ((long)list)
		);
	return res;
}

#endif /* _GETCHILDPIDS_H */

