/*
 * Example from Silberschatz, Galvin, and Gagne’s
 * Operating System Concepts, Seventh Edition
 */
#include <stdlib.h>
#include <linux/errno.h>
#include <sys/syscall.h>
#include <linux/unistd.h>

_syscall0(int, helloworld);

int main ( void )
{
	helloworld();
	return EXIT_SUCCESS;
}

