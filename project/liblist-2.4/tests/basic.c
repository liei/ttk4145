/* basic.c -- basic list functionality test
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
#include <stdlib.h>
#include <string.h>

static char *alphabets[] =
  {
    "a", "b", "c", "dee", "eee", "eph", "jee", "aecsh", "eye", "jay", "kay",
    "ell", "em", "en", "oh", "pee",
    (char *)NULL
  };

int main(int argc, char *argv[])
{
  size_t counter;

  LIST *mylist;
  char *astring;

  mylist = list_init();

  /**
     malloc() (via strdup()) and free()
   */

  for(counter = 0; alphabets[counter]; counter ++)
    list_insert_before(mylist, strdup(alphabets[counter]), 0);

  for(counter --; counter > 5; counter --)
    {
      astring = list_remove_front(mylist);
      if(strcmp(astring, alphabets[counter]))
	{
	  fprintf(stderr, "%s:%d: Expected ``%s'', got ``%s''\n", __FILE__, __LINE__,
		  alphabets[counter], astring);
	  return 1;
	}
      free(astring);
    }

  list_free(mylist, LIST_DEALLOC);

  /**
     static
  */
  mylist = list_init();

  for(counter = 0; alphabets[counter]; counter ++)
    list_insert_before(mylist, alphabets[counter], 0);

  for(counter --; counter > 5; counter --)
    {
      astring = list_remove_front(mylist);
      if(strcmp(astring, alphabets[counter]))
	{
	  fprintf(stderr, "%s:%d: Expected ``%s'', got ``%s''\n", __FILE__, __LINE__,
		  alphabets[counter], astring);
	  return 1;
	}
    }

  list_free(mylist, LIST_NODEALLOC);


  return 0;
}
