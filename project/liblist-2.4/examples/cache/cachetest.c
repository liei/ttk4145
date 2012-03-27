/* cachetest.c -- test program for generic cache package
 *
 * Last edited: Tue Jul 28 15:42:57 1992 by bcs (Bradley C. Spatz) on wasp
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

static char brag[] = "$$Version: cachetest " PACKAGE_VERSION " Copyright (C) 1992 Bradley C. Spatz";

#include <stdio.h>
#include <stdlib.h>
#include "cache.h"

#define MAX_ELEMENTS 3

int match();

main()
{
   CACHE *cache;
   char cmd[2];
   int val, *pval;

   /* Print some instructions. */
   printf("This program demonstrates the various cache manipulation\n");
   printf("routines supplied by this package.  Although this program\n");
   printf("manipulates a cache of integers, data of any type and size\n");
   printf("(and not just integers) are supported by the routines.  See\n");
   printf("the man page for more information.\n\n");
   printf("We illustrate a cache with a maximum size of 3 entries.\n\n");

   /* Allocate a new cache. */
   cache = cache_init(MAX_ELEMENTS);
   print_cache(cache);

   /* Get some commands. */
   printf("\n(e)nter new element, (c)heck for element; (q)uit: ");
   while (scanf("%1s", cmd) != EOF) {
      switch (cmd[0]) {
         case 'e':
	    printf("Value (int) to enter: ");
	    if (scanf("%d", &val)) {
	       /* We ignore the return code here, but in practice, 
		* we may fail (with a return code of NULL).
		*/
	       cache_enter(cache, &val, sizeof(val), &pval);
	       if (pval != NULL) {
		  printf("%d was removed to make room.\n", *pval);
	       }
	    }
            break;
	 case 'c':
	    printf("Value (int) to check for: ");
	    if (scanf("%d", &val)) {
	       pval = (int *) cache_check(cache, (char *) &val, match);
	       if (pval != NULL) {
		  printf("%d found!\n", *pval);
	       }
	       else {
		  printf("Not found.\n");
	       }
	    }
	    break;
	 case 'q':
	    cache_free(cache, CACHE_DEALLOC);
	    exit(0);
	    break;
         default:
	    printf("'%s' not a recognized command!\n", cmd);
	    break;
      }
      print_cache(cache);
      printf("\n(e)nter new element, (c)heck for element; (q)uit: ");
   }

   exit(0);
}


/* Provide a routine to return FALSE (0) if the current element in the cache,
 * curr, is equal to the element we are searching for, data.  We return
 * FALSE, instead of TRUE, because we will use the list_traverse function.
 * That function continues if the user-supplied function is TRUE.  We want
 * to stop when we've found our element.
 */
int match(data, curr)
char *data;
char *curr;
{
   return(((*(int *) data) == (*(int *) curr)) ? FALSE : TRUE);
}


/* The following routines make know that the cache is implemented with
 * the list(3) package.  It makes knowledge of the CACHE structure as
 * well.  We do this here for illustration only.
 */
int print_element(input, curr)
char *input;
char *curr;
{
   printf(" %d", *(int *) curr);
   return(TRUE);
}

print_cache(cache)
CACHE *cache;
{
   printf("Cache:");
   if (list_empty(cache->list))
      printf(" (empty).\n");
   else {
      list_traverse(cache->list, (char *) 0, print_element,
		    (LIST_FRNT | LIST_FORW | LIST_SAVE));
      printf(".\n");
   }
}
