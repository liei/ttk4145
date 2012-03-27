/* queuetest.c -- test program for generic queue package.
 * 
 * Last edited: Tue Jul 28 15:38:58 1992 by bcs (Bradley C. Spatz) on wasp
 *
 * Copyright (C) 1992, Bradley C. Spatz, bcs@ufl.edu
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

static char brag[] = "$$Version: queuetest " PACKAGE_VERSION " Copyright (C) 1992 Bradley C. Spatz";

#include <stdio.h>
#include <stdlib.h>
#include "../queue.h"

main()
{
   QUEUE *q;
   char cmd;
   int val, *pval;

   /* Print some instructions. */
   printf("This program demonstrates the various queue manipulation\n");
   printf("routines supplied by this package.  Although this program\n");
   printf("manipulates a queue of integers, data of any type and size\n");
   printf("(and not just integers) are supported by the routines.  See\n");
   printf("the man page for more information.\n\n");

   /* Allocate a new queue. */
   q = q_init();
   print_queue(q);

   /* Get some commands. */
   printf("(e)nqueue, (d)equeue, (f)ront, (s)ize, (c)heck, (q)uit: ");
   while (scanf("%1s", &cmd) != EOF) {
      switch (cmd) {
	 case 'e':
	    printf("Value (int) to enqueue: ");
	    if (scanf("%d", &val))
	       /* We ignore the return code here, butin practice, we may
		* fail (with a return code of 0).
		*/
	       q_enqueue(q, &val, sizeof(val));
	    break;
	 case 'd':
	    pval = (int *) q_dequeue(q);
	    if (pval != NULL)
	       printf("%d dequeued.\n", *pval);
	    else
	       printf("Queue is empty!\n");
	    break;
	 case 'f':
	    pval = (int *) q_front(q);
	    if (pval == NULL) 
	       printf("Queue is empty!\n");
	    else
	       printf("%d at front.\n", *pval);
	    break;
	 case 's':
	    val = q_size(q);
	    printf("Queue has %d element%s.\n", val, ((val == 1) ? "" : "s"));
	    break;
	 case 'c':
	    printf("Queue is%s empty.\n", (q_empty(q) ? "" : " not"));
	    break;
	 case 'q':
	    q_free(q, QUEUE_DEALLOC);
	    exit(0);
	    break;
         default:
	    printf("'%c' not a recognized command!\n", cmd);
	    break;
      }
      print_queue(q);
      printf("\n(e)nqueue, (d)equeue, (f)ront, (s)ize, (c)heck, (q)uit: ");
   }

   exit(0);
}


/* Routine to print the integer stored at each node.  In this example,
 * we ignore the first parameter, which might be useful if we were
 * searching the list or something.  We must return 0 or 1, so we always
 * return 1.
 */
int print_element(input, curr)
char *input;
char *curr;
{
   printf(" %d", *(int *) curr);
   return(TRUE);
}


/* Routine to print a queue of integers. */
print_queue(queue)
QUEUE *queue;
{
   printf("Queue: ");
   if (q_empty(queue))
      printf("(empty)\n");
   else {
      list_traverse(queue, (char *) 0, print_element,
		    (LIST_FRNT | LIST_FORW | LIST_SAVE));
      printf("\n");
   }
}
