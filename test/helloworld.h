#ifndef _HELLOWORLD_H
#define _HELLOWORLD_H
/*
#include <sys/syscall.h>
#include <linux/unistd.h>

 * This macro doesn't work:
 * _syscall0(int, helloworld);
 */

int helloworld ( void )
{
	__asm__ (
		"movl	$337,%eax\n\t"
		"int	$0x80\n\t"
		);
}

#endif /* _HELLOWORLD_H */

