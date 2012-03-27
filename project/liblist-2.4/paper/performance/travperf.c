/* travperf.c -- performance test for list_traverse
 *
 * Bradley C. Spatz, University of Florida, bcs@ufl.edu
 * Last edited: Wed Sep 18 23:05:54 1991 by bcs (Bradley C. Spatz) on cutback
 */

#include <stdio.h>
#include <list.h>

#define ITERATIONS 100000
#define LIST_SIZE  5

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
   int iterations, i, j;
   int start, clicks;
   LIST *l;
   int null_func();

   /* Snag # iterations from the command line. */
   if (argc > 1) {
      iterations = atoi(argv[1]);
   }
   else {
      iterations = ITERATIONS;
   }

   /* Build the list. */
   if ((l = list_init()) == NULL) {
      fprintf(stderr, "%s: unable to allocate list descriptor.\n", argv[0]);
   }
   for (i=0; i<LIST_SIZE; i++) {
      list_insert_before(l, &i, sizeof(i));
   }

   start = jobclicks();

   /* Now iterate the following: traverse the list with a null function. */
   for (i=0; i<iterations; i++) {
#ifdef LIST_TRAVERSE
      list_traverse(l, (char *) NULL, null_func,
		    (LIST_FRNT | LIST_FORW | LIST_SAVE));
#else
      list_mvfront(l);
      do {
	 null_func((char *) NULL, (char *) list_curr(l));
      } while (list_mvnext(l) != NULL);
#endif
   }

   clicks = jobclicks() - start;
   printf("%d clicks (%.2f s)\n", clicks, (float) (clicks/CLICKSPERSEC));

   list_free(l, LIST_DEALLOC);
   exit(0);
}


int null_func(data, curr)
char *data, curr;
{
   return(1);
}
