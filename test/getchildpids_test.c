#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "getchildpids_test.h"
#include "getchildpids.h"

/* Format */
const char BEGIN_ARRAY = '{';
const char END_ARRAY = '}';

/* Error messages */
const char SYSCALL_FAILED_MSG[] = "ERROR: System call 'getchildpids' failed, returning error code [%d].\n";
const char SYSCALL_FAILED_PERROR_MSG[] = "Error description";
const char CHILDREN_DIFFER_MSG[] = "ERROR: The two lists are different.";

/* Information messages */
const char ACTUAL_CHILDREN_LABEL[] =  "Actual process children:       ";
const char SYSCALL_CHILDREN_LABEL[] = "Children according to syscall: ";
const char CHILDREN_SAME_MSG[] = "The two lists are the same.\n";

static int compare_pid_t ( const void * elem1, const void * elem2 )
{
	return *(pid_t*)elem1 - *(pid_t*)elem2;
}

static void print_array ( int size, pid_t list[], FILE *stream )
{
	if ( size < 0 )
		return;

	fputc ( BEGIN_ARRAY, stream );

	if (size > 0 )
	{
		pid_t *current = list;
		fprintf ( stream, "%d", *current );
		for ( ++current ; current < (list + size) ; ++current )
		{
			fprintf ( stream, ",%d", *current );
		}
	}

	fputc ( END_ARRAY, stream );
}

int getchildpids_test ( int actualChildrenCount, pid_t actualChildren[],
		pid_t parentParameter, int sizeParameter, pid_t listParameter[] ) {
	int syscallChildrenCount;

	syscallChildrenCount = getchildpids ( parentParameter, sizeParameter, listParameter );
	if ( syscallChildrenCount < 0 )
	{
		fprintf ( stderr, SYSCALL_FAILED_MSG, syscallChildrenCount );
		perror ( SYSCALL_FAILED_PERROR_MSG );
		return -1;
	}

	qsort ( actualChildren, actualChildrenCount, sizeof(pid_t),
			compare_pid_t );
	qsort ( listParameter, syscallChildrenCount, sizeof(pid_t),
			compare_pid_t );

	fputs ( ACTUAL_CHILDREN_LABEL, stderr );
	print_array ( actualChildrenCount, actualChildren, stderr );
	fputc ( '\n', stderr );
	fputs ( SYSCALL_CHILDREN_LABEL, stderr );
	print_array ( syscallChildrenCount, listParameter, stderr );
	fputc ( '\n', stderr );

	if ( syscallChildrenCount != actualChildrenCount
			|| 0 != memcmp ( actualChildren, listParameter, actualChildrenCount ) )
	{
		fputs ( CHILDREN_DIFFER_MSG, stderr );
		return 1;
	}
	else
	{
		fputs ( CHILDREN_SAME_MSG, stderr );
		return 0;
	}
}

