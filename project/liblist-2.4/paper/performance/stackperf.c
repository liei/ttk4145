/* stackperf.c  --  stack performance test for list(3) library
 *
 * Bradley C. Spatz, University of Florida, bcs@ufl.edu
 * Last edited: Wed Sep 18 23:27:28 1991 by bcs (Bradley C. Spatz) on cutback
 */

#include <stdio.h>
#ifdef DSTACK
#include <dstack.h>
#else
#include <stack.h>
#endif

#define ITERATIONS  100000


/* Objects associated with CPU times -- may have to change
 * CLICKSPERSEC is defined as HZ on many systems
 *	Typical values of CLICKSPERSEC: Vax=60  Cray=105296000
 *	For large values, also change %5d in macro loop1, 99999 in minclicks;
 */

#define CLICKSPERSEC 60

#include <sys/types.h>
#include <sys/times.h>

int jobclicks()
{
	struct	tms buffer;

	times(&buffer);
	return (int) (buffer.tms_utime + buffer.tms_stime);
}


main(argc, argv)
int argc;
char *argv[];
{
   STACK *s;
   int iterations, i, *val;
   int start, clicks;


   if (argc > 1) {
      iterations = atoi(argv[1]);
   }
   else {
      iterations = ITERATIONS;
   }

   if ((s = stack_init()) == NULL) {
      fprintf(stderr, "%s: unable to allocate stack descriptor.\n", argv[0]);
      exit(1);
   }

   start = jobclicks();

   /* Perform the test. */
   for (i=0; i<iterations; i++) {
#ifndef NO_MALLOC
      stack_push(s, &i, sizeof(i));
#else
      stack_push(s, &i, 0);
#endif
      val = (int *) stack_pop(s);
      free((char *) val);
   }

   clicks = jobclicks() - start;
   printf("%d clicks (%.2f s)\n", clicks, (float) (clicks/CLICKSPERSEC));

#ifdef DSTACK
   stack_free(s);
#else
   stack_free(s, STACK_DEALLOC);
#endif
   exit(0);
}
