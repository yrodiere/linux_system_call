#include <linux/linkage.h> /* asmlinkage */
#include <linux/errno.h> /* EINVAL, EFAULT */
#include <asm/uaccess.h> /* put_user */
#include <linux/spinlock.h> /* read_lock, read_unlock */
#include <linux/sched.h> /* task_struct, tasklist_lock */
#include <linux/pid.h> /* find_task_by_vpid */
#include <linux/list.h> /* list_for_each_entry */

/* Get the pids of the children of a particular process.
 *
 * getchildpids() returns the pids of the children of the process whose pid is
 * <parent_pid> in <list>. The order is undefined.
 * The argument <size> should be set to the maximum number of items that can
 * be stored in the buffer pointed to by <list>.
 *
 * If no child process exists, <list> is not modified.
 *
 * If more than <size> children exist, an error results and <list>
 * might be modified (up to <size> elements).
 *
 * If no process is associated with <parent_pid>, an error results.
 *
 * On success, getchildpids() returns the number of retrieved children pids.
 * On error, -1 is returned, and errno is set appropriately.
 *
 * Errno values:
 *	- EFAULT	<list> has an invalid address.
 *	- EINVAL	<size> is less than the number of children, or there is no
 *		process associated with <parent_pid>.
 */
asmlinkage int sys_getchildpids ( pid_t parent_pid, int size, pid_t *list ) {
	struct task_struct *parent;
	struct task_struct *child;
	pid_t *current_out_pid;

	read_lock(&tasklist_lock);
	parent = find_task_by_vpid(parent_pid);

	if ( NULL == parent )
	{
		read_unlock(&tasklist_lock);
		return -EINVAL;
	}

	current_out_pid = list;
	list_for_each_entry ( child, &(parent->children), sibling )
	{
		if ( (list + size) <= current_out_pid )
		{
			read_unlock(&tasklist_lock);
			return -EINVAL;
		}

		if ( 0 != put_user ( child->pid, current_out_pid ) )
		{
			read_unlock(&tasklist_lock);
			return -EFAULT;
		}

		++current_out_pid;
	}

	read_unlock(&tasklist_lock);
	return (current_out_pid - list);
}

