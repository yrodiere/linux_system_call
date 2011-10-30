#ifndef _HELLOWORLD_H
#define _HELLOWORLD_H

#include <errno.h>

static int helloworld ( void )
{
	int res;

	__asm__ __volatile__ (
		"int	$0x80;"
		: "=a" (res)
		: "0" (337)
		);

	if ( res < 0 )
	{
		errno = -res;
		return -1;
	}
	else
		return res;
}

#endif /* _HELLOWORLD_H */

