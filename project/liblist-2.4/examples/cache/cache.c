/* cache.c -- routines to implement a generic, list(3)-based cache package.
 *
 * Last edited: Tue Jul 28 15:41:47 1992 by bcs (Bradley C. Spatz) on wasp
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
 *
 *
 * We define the following routines here:
 *
 *    cache_t cache_init(max_elements)
 *    char *cache_enter(cache, data, bytes, removed)
 *    char *cache_check(cache, data, match)
 *    void cache_free(cache, dealloc)
 *
 * for
 *
 *    cache_t cache;
 *    char *data;
 *    int max_elements, max_size, bytes;
 *    char **removed;
 *    int match(data, curr)
 *       char *data;
 *       char *curr;
 *    void dealloc(data)
 *       char *data;
 *
 * We base this package on the list(3) package.
 *
 * Keep a data structure that is essentially a list.  We'll add new
 * elements to the front of the list and remove old elements from the end
 * of the list.  We'll optimize searches for MRU (Most Recently Used) and
 * we'll dispose of elements when we need space by using LRU.  Finally,
 * we'll always promote the element on a hit to the front of the list.
 * 
 * We'll allow the user to control the size of the cache in terms of
 * cache elements.  This limit will be determined at cache creation.
 */

static char brag[] = "$$Version: cache " PACKAGE_VERSION " Copyright (C) 1992 Bradley C. Spatz";

#include <stdio.h>
#include <stdlib.h>
#include "cache.h"

struct cache {
   int max_elements;
   LIST *list;
};

cache_t cache_init(int max_elements)
{
   cache_t new_cache;

   /* Allocate, initialize, and return a new cache.   Return NULL if
    * the malloc or list initialization fails.
    */
   if ((new_cache = (cache_t) malloc(sizeof(struct cache))) == NULL) {
      return(NULL);
   }
   new_cache->max_elements = max_elements;
   if ((new_cache->list = list_init()) == NULL) {
      /* The list creation fragged, so release the cache descriptor. */
      free(new_cache);
      return(NULL);
   }

   return(new_cache);
}


void *cache_enter(cache_t cache, void *data, int bytes, void **removed)
{
   char *new_element;

   /* Add a new element to the front of our list.  This is easy, because
    * we're using the list(3) package; our intentions exactly.
    * Try and add the new element.  If that succeeds, then check for a
    * full cache.  If full, remove the element at the rear of the list.
    * We return a pointer to the newly inserted element or NULL if the
    * insert failed.  We also return a pointer to the removed element
    * if we did indeed remove one.
    */
   *removed = NULL;
   new_element = list_insert_before(cache->list, data, bytes);
   if (new_element != NULL) {
      if (list_size(cache->list) > cache->max_elements) {
	 *removed = (char *) list_remove_rear(cache->list);
      }
   }

   return(new_element);
}


void *cache_check(cache_t cache, void *data, cache_match_func_t match)
{
   char *found;

   /* Check for an empty cache. */
   if (list_size(cache->list) == 0) {
      return(NULL);
   }

   /* Ok.  Search the list for the element, starting from the front
    * of our list.  If the traversal finds the element, then promote it to
    * the front if it's not already there, and return a pointer to the element.
    * Otherwise, return NULL.  In either case, make sure to reset the
    * current element pointer back to the front of the list.
    */
   if (list_traverse(cache->list, data, match,
		     (LIST_FRNT | LIST_FORW | LIST_ALTR)) == LIST_OK) {
      /* We found what we're looking for. */
      if (list_curr(cache->list) != list_front(cache->list)) {
	 fprintf(stderr, "cache_check: moving found to front.\n");
	 found = (char *) list_remove_curr(cache->list);
	 list_mvfront(cache->list);
	 list_insert_before(cache->list, found, 0);
	 return(found);
      }
      else {
	 return(list_front(cache->list));
      }
   }
   else {
      /* We did not find the element. */
      list_mvfront(cache->list);
      return(NULL);
   }
}


void cache_free(cache_t cache, cache_dealloc_func_t dealloc)
{
   /* First free up the list, and then the cache descriptor. */
   list_free(cache->list, dealloc);
   free(cache);
}
