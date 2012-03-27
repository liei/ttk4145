/* list_remove_element.c -- test the list_remove_element() function
 * 
 * Copyright (C) 2010 Nathan Phillip Brink <ohnobinki@ohnopublishing.net>
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
 */

#include <list.h>

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

static char *things[] =
  {
    "a", "blah", "dee", "dah", "ddeee", "dd", "if", "you", "can", "read", "this",
    ",", "you", "are", "crazy",
    (char *)NULL
  };
static int test_double_remove(list_t mylist, void *thing, int stage);
static size_t count_elements(list_t mylist, void *element);

int main(int argc, char *argv[])
{
  size_t counter;
  list_t mylist;
  int ret;
  void *tmp;

  mylist = list_init();

  for(counter = 0; things[counter]; counter ++)
    list_insert_after(mylist, things[counter], 0);

  if(test_double_remove(mylist, things[5], 0))
    return 1;

  /* test the removal of multiple elements */
  tmp = list_insert_after(mylist, things[6], 0);
  if(tmp != things[6])
    {
      fprintf(stderr, "%s:%d: list_insert_after() returned an incorrect value. Got %lu, expecting %lu\n",
	      __FILE__, __LINE__, (unsigned long)(intptr_t)tmp, (unsigned long)(intptr_t)things[6]);
      return 1;
    }

  ret = count_elements(mylist, things[6]);
  if(ret != 2)
    {
      fprintf(stderr, "%s:%d: count_elements() reports that there %d instances of an element. I was expecting there to be two instances\n",
	      __FILE__, __LINE__, ret);
      return 1;
    }

  if(test_double_remove(mylist, things[6], 1))
    return 1;

  list_free(mylist, LIST_NODEALLOC);

  return 0;
}

static int test_double_remove(list_t mylist, void *thing, int stage)
{
  int ret;

  ret = list_remove_element(mylist, thing);
  if(ret != LIST_OK)
    {
      fprintf(stderr, "%s:%d: stage=%d: Expected LIST_OK, got %d instead\n",
	      __FILE__, __LINE__, stage, ret);
      return 1;
    }

  ret = list_remove_element(mylist, thing);
  if(ret != LIST_EXTENT)
    {
      fprintf(stderr, "%s:%d: stage=%d: Expected LIST_EXTENT, got %d instead\n",
	      __FILE__, __LINE__, stage, ret);
      return 1;
    }
  return 0;
}

static int count_elements_traverse(void *element_to_match, void *element)
{
  return element_to_match != element;
}

static size_t count_elements(list_t mylist, void *element)
{
  size_t count;

  count = 0;
  list_mvfront(mylist);
  while(mylist
	&& list_traverse(mylist, element, &count_elements_traverse, LIST_FORW|LIST_CURR|LIST_ALTR) == LIST_OK)
    {
      /*
       * list_mvnext() returns NULL and doesn't change list->curr if
       * the end of the list is reached (as documented).
       */
      mylist = list_mvnext(mylist);
      count ++;
    }

  return count;
}
