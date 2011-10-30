#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

#include "getchildpids_test.h"

const char GLOBAL_SEPARATOR[] = "\n========================================\n";
const char LOCAL_SEPARATOR[] = "\n----------------------------------------\n";
const char INTERNAL_ERROR_PERROR_MSG[] = "ERROR: internal error in test function: ";

const char NOMINAL_CASE_LABEL[] = "In this test, no error should occur.";
const char INVALID_PPID_LABEL[] = "In this test, errno should be set to EINVAL.";
const char ARRAY_TOO_SHORT_LABEL[] = "In this test, errno should be set to EINVAL.";
const char INVALID_POINTER_LABEL[] = "In this test, errno should be set to EFAULT.";


static void launchTestInEnv ( const char* description, unsigned int realChildCount,
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
			perror(INTERNAL_ERROR_PERROR_MSG);
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
	launchTestInEnv ( NOMINAL_CASE_LABEL,
			childCount, getpid(), childCount );
}

static void testBadPpidCase ( int childCount, pid_t badPpid )
{
	launchTestInEnv ( INVALID_PPID_LABEL,
			childCount, badPpid, childCount );
}

static void testArrayTooShortCase ( int childCount, int arraySizeDifference )
{
	launchTestInEnv ( ARRAY_TOO_SHORT_LABEL,
			childCount, getpid(), childCount + arraySizeDifference );
}

/* TODO */
static void testInvalidPointerCase ( int childCount, pid_t badPpid )
{
	launchTestInEnv ( INVALID_POINTER_LABEL,
			childCount, badPpid, childCount );
}


int main ( void )
{
	/*
	 * TESTS WITH VALID PARAMETERS
	 */

	testNominalCase ( 0 );
	testNominalCase ( 1 );
	testNominalCase ( 10 );
	testNominalCase ( 100 );


	/*
	 * TESTS WITH INVALID PARAMETERS
	 */

	/* Incorrect pid */
	testBadPpidCase ( 0, -1 );
	testBadPpidCase ( 1, -1 );
	testBadPpidCase ( 10, -1 );
	testBadPpidCase ( 100, -1 );

	/* Array too short */
	testArrayTooShortCase ( 1, -1 );
	testArrayTooShortCase ( 10, -1 );
	testArrayTooShortCase ( 10, -5 );
	testArrayTooShortCase ( 10, -10 );
	testArrayTooShortCase ( 100, -1 );
	testArrayTooShortCase ( 100, -50 );
	testArrayTooShortCase ( 100, -100 );

	/* Invalid pointer */
	/* TODO */

	return EXIT_SUCCESS;
}

