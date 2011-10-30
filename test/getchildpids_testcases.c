#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

#include "getchildpids_test.h"

const char GLOBAL_SEPARATOR[] = "\n========================================\n";
const char LOCAL_SEPARATOR[] = "\n----------------------------------------\n";

const char INTERNAL_ERROR_PERROR_MSG[] = "ERROR: internal error in test function: ";
const char OOM_MSG[] = "ERROR: out of memory.";

const char NOMINAL_CASE_LABEL[] = "NOMINAL CASE\nIn this test, no error should occur.";
const char INVALID_PPID_LABEL[] = "INVALID PPID CASE\nIn this test, errno should be set to EINVAL.";
const char ARRAY_TOO_SHORT_LABEL[] = "ARRAY TOO SHORT CASE\nIn this test, errno should be set to EINVAL.";
const char INVALID_POINTER_LABEL[] = "INVALID POINTER CASE\nIn this test, errno should be set to EFAULT.";


static void launchTestInEnv ( const char* description, size_t actualChildrenCount,
		pid_t parentParameter, int sizeParameter, pid_t listParameter[] )
{
	pid_t *actualChildren = (pid_t*) malloc ( actualChildrenCount * sizeof(pid_t) );
	pid_t *forkedChild;
	pid_t *deletedChild;

	fputs ( GLOBAL_SEPARATOR, stderr );
	fputs ( description, stderr );
	fputs ( LOCAL_SEPARATOR, stderr );

	/* Fork every child */
	for ( forkedChild = actualChildren
			; forkedChild < (actualChildren + actualChildrenCount)
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

	if ( forkedChild == (actualChildren + actualChildrenCount) )
	{
		getchildpids_test ( actualChildrenCount, actualChildren,
				parentParameter, sizeParameter, listParameter );
	}

	/* Kill every child */
	for ( deletedChild = actualChildren ; deletedChild < forkedChild
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

	free ( actualChildren );
}

static void testNominalCase ( size_t childCount )
{
	pid_t *listParameter = (pid_t*) malloc(childCount * sizeof(pid_t));
	if ( NULL == listParameter )
	{
		fputs ( OOM_MSG, stderr );
		return;
	}

	launchTestInEnv ( NOMINAL_CASE_LABEL, childCount,
			getpid(), childCount, listParameter );

	free ( listParameter );
}

static void testBadPpidCase ( size_t childCount, pid_t badPpid )
{
	pid_t *listParameter = (pid_t*) malloc(childCount * sizeof(pid_t));
	if ( NULL == listParameter )
	{
		fputs ( OOM_MSG, stderr );
		return;
	}

	launchTestInEnv ( INVALID_PPID_LABEL, childCount,
			badPpid, childCount, listParameter );

	free ( listParameter );
}

static void testArrayTooShortCase ( size_t childCount, int arraySizeDifference )
{
	int sizeParameter = childCount + arraySizeDifference;
	pid_t *listParameter = (pid_t*) malloc(sizeParameter * sizeof(pid_t));
	if ( NULL == listParameter )
	{
		fputs ( OOM_MSG, stderr );
		return;
	}

	launchTestInEnv ( ARRAY_TOO_SHORT_LABEL, childCount,
			getpid(), sizeParameter, listParameter );

	free ( listParameter );
}

static void testInvalidPointerCase ( size_t childCount, pid_t *invalidPointer )
{
	launchTestInEnv ( INVALID_POINTER_LABEL, childCount,
			getpid(), childCount, invalidPointer );
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
	testInvalidPointerCase ( 1, NULL );
	testInvalidPointerCase ( 1, (pid_t*)&main );
	testInvalidPointerCase ( 10, NULL );
	testInvalidPointerCase ( 10, (pid_t*)&main );
	testInvalidPointerCase ( 100, NULL );
	testInvalidPointerCase ( 100, (pid_t*)&main );

	return EXIT_SUCCESS;
}

