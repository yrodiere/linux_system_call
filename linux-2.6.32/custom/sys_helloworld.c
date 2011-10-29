/*
 * Example from Silberschatz, Galvin, and Gagne’s
 * Operating System Concepts, Seventh Edition
 */
#include <linux/linkage.h>
#include <linux/kernel.h>

asmlinkage int sys_helloworld ( void ) {
	printk(KERN EMERG "hello world!");
	return 1;
}

