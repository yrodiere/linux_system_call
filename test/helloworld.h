#ifndef _HELLOWORLD_H
#define _HELLOWORLD_H
/*
#include <sys/syscall.h>
#include <linux/unistd.h>

 * This macro doesn't work:
 * _syscall0(int, helloworld);
 */

static int helloworld ( void )
{
	int res;
	__asm__ __volatile__ (
		"int	$0x80;"
		: "=a" (res)
		: "0" (337)
		);
	return res;
}

#endif /* _HELLOWORLD_H */

