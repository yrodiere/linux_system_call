#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

#include "getchildpids_test.h"

const char GLOBAL_SEPARATOR[] = "\n========================================\n";
const char LOCAL_SEPARATOR[] = "\n----------------------------------------\n";

static void launchTestInEnv ( char* description, unsigned int realChildCount,
		pid_t syscallParameterParentPid, int syscallParameterChildCount )
{
	pid_t *children = (pid_t*) malloc ( realChildCount * sizeof(pid_t) );
	pid_t *forkedChild;
	pid_t *deletedChild;

	fputs ( GLOBAL_SEPARATOR, stderr );
	fputs ( description, stderr );
	fputs ( LOCAL_SEPARATOR, stderr );

	/* Fork every child */
	for ( forkedChild = children ; forkedChild < (children + realChildCount)
			; ++forkedChild )
	{
		pid_t result = fork();
		if ( result == 0 )
		{
			/* Child code */
			pause();
			exit(EXIT_SUCCESS);
		}

		/* Parent code */

		if ( result < 0 )
		{
			/* Exception: could not fork */
			perror("ERROR: internal error in test function: ");
			break;
		}

		*forkedChild = result;
	}

	if ( forkedChild == (children + realChildCount) )
	{
		getchildpids_test ( syscallParameterParentPid,
				syscallParameterChildCount, children );
	}

	/* Kill every child */
	for ( deletedChild = children ; deletedChild < forkedChild
			; ++deletedChild )
	{
		/* Errors are more or less ignored here,
		 * since we cannot do much to fix them
		 */
		if (0 == kill ( *deletedChild, SIGKILL ) )
		{
			waitpid ( *deletedChild, NULL, 0 );
		}
	}

	free ( children );
}

static void testNominalCase ( int childCount )
{
	launchTestInEnv ( "In this test, no error should occur.",
			childCount, getpid(), childCount );
}

int main ( void )
{
	/*
	 * TESTS WITH VALID PARAMETERS
	 */

	/* Test case 1: 0 child */
	testNominalCase ( 0 );

	/* Test case 2: 1 child */
	testNominalCase ( 1 );

	/* Test case 3: 10 children */
	testNominalCase ( 10 );

	/* Test case 4: 100 children */
	testNominalCase ( 100 );

	/*
	 * TESTS WITH INVALID PARAMETERS
	 */

	/* TODO */
	/* Incorrect pid */
	/*
	test ( 0, -1, 0 );
	test ( 1, -1, 1 );
	test ( 10, -1, 10 );
	*/

	return EXIT_SUCCESS;
}

