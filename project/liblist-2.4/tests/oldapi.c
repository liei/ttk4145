/*
  oldapi.c -- tests to make sure that we stay compliant with the old API

  Copyright (C) 2010 Nathan Phillip Brink <ohnobinki@ohnopublishing.net>

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
 
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
 
  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

/*
  The point of this file is to make sure that the current API (not ABI,
  we've thrown that out long ago) is backwards compatible with the original
  API. This essentially means the following:
  - old macros, rather than current typedefs, still work
  - all functions described in the original API spec are callable
  - the library acts in the same manner as the original (if it is possible
    to check this)

  The first two goals are compiletime and used to catch mistakes when
  editing the header files.
 */

#include <list.h>
#include <stack.h>
#include <queue.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int check_list();
int check_stack();
int check_queue();

int main(int argc, char *argv[])
{
  int ret;

  ret = check_list();
  if(ret)
    {
      fprintf(stderr, "LIST fails: %d\n", ret);
      return 1;
    }

  ret = check_stack();
  if(ret)
    {
      fprintf(stderr, "STACK fails: %d\n", ret);
      return 2;
    }

  ret = check_queue();
  if(ret)
    {
      fprintf(stderr, "QUEUE fails: %d\n", ret);
      return 3;
    }

  return 0;
}

static char* datas[] = 
  { "Hello,",
    "I",
    "am",
    "Bob.",
    (char *)NULL };

int list_traverse_print(void *data, void *node_data)
{
  char *thestring = (char *)node_data;

  printf("%s\n", thestring);

  return TRUE;
}

int check_list()
{
  LIST *list, *listreturn;

  char *dataptr;

  size_t counter;

  int travreturn;
  
  list = list_init();
  if(!list)
    return 1;

  /* test if list is functional without anything in it (expect NULL return) */
  listreturn = list_mvprev(list);
  if(listreturn)
    return 2;

  /* play with some data */
  for(counter = 0; datas[counter]; counter ++)
    {
      dataptr = list_insert_after(list, datas[counter], strlen(datas[counter]) + 1);
      if(strcmp(dataptr, datas[counter]))
	{
	  fprintf(stderr, "got %s, expecting %s\n", dataptr, datas[counter]);
	  return 3;
	}
      
    }

  for(counter = 0; datas[counter]; counter ++)
    {
      dataptr = list_insert_before(list, datas[counter], strlen(datas[counter]) + 1);
      if(strcmp(dataptr, datas[counter]))
	{
	  fprintf(stderr, "got %s, expecting %s\n", dataptr, datas[counter]);
	  return 4;
	}
    }

  /* test traversal */
  travreturn = list_traverse(list, (void *)NULL, &list_traverse_print, 0);
  if(travreturn != LIST_EXTENT)
    return 5;

  /* The defaults for list traversal are to not change the current
     element of the list. Thus, list_curr should return the last
     inserted piece of data which is at datas[counter - 1] */
  dataptr = (char *)list_curr(list);
  if(strcmp(dataptr, datas[counter - 1]))
    {
      fprintf(stderr, "get %s, expecting %s\n", dataptr, datas[counter - 1]);
      return 6;
    }

  list_free(list, LIST_DEALLOC);

  return 0;
}


int check_stack()
{
  STACK *stack;
  char *dataptr;

  size_t counter;

  stack = stack_init();
  if(!stack)
    return 1;

  for(counter = 0; datas[counter]; counter ++)
    {
      dataptr = (char *)stack_push(stack, datas[counter], strlen(datas[counter]) + 1);
      if(strcmp(dataptr, datas[counter]))
	{
	  fprintf(stderr, "got %s, expecting %s\n", dataptr, datas[counter]);
	  return 2;
	}
      dataptr = (char *)stack_top(stack);
      if(strcmp(dataptr, datas[counter]))
	{
	  fprintf(stderr, "got %s, expecting %s\n", dataptr, datas[counter]);
	  return 3;
	}
    }

  if(stack_size(stack) != counter)
    {
      fprintf(stderr, "stack size is %zu, expected %zu\n", stack_size(stack), counter);
      return 4;
    }

  while(!stack_empty(stack))
    {
      dataptr = (char *)stack_pop(stack);
      if(!dataptr)
	{
	  fprintf(stderr, "stack is not empty but stack_pop() returns NULL\n");
	  return 5;
	}

      printf("%s\n", dataptr);
      free(dataptr);
    }

  dataptr = (char *)stack_pop(stack);
  if(dataptr)
    {
      fprintf(stderr, "stack is empty but I still got %s\n", dataptr);
      return 6;
    }

  if(stack_size(stack) != 0)
    {
      fprintf(stderr, "stack is empty but has a size of %zu\n", stack_size(stack));
      return 7;
    }

  stack_free(stack, STACK_DEALLOC);

  return 0;
}

int check_queue()
{
  QUEUE *queue;
  char *item;

  size_t counter;

  queue = q_init();
  if(!queue)
    {
      fprintf(stderr, "unable to initialize queue\n");
      return 0;
    }

  for(counter = 0; datas[counter]; counter ++)
    q_enqueue(queue, datas[counter], strlen(datas[counter]) + 1);

  item = (char *)q_front(queue);
  if(!item)
    {
      fprintf(stderr, "got NULL when expecting %s\n", datas[counter]);
      return 1;
    }
  if(strcmp(item, datas[0]))
    {
      fprintf(stderr, "q_front() returned %s, expecting %s\n", item, datas[0]);
      return 2;
    }

  for(counter = 0; datas[counter]; counter ++)
    {
      item = (char *)q_dequeue(queue);
      if(!item || strcmp(item, datas[counter]))
	{
	  fprintf(stderr, "got %s, expecting %s\n", item, datas[counter]);
	  return 3;
	}
      free(item);
    }
  
  item = (char *)q_dequeue(queue);
  if(item)
    {
      fprintf(stderr, "got %s when expecting NULL\n", item);
      return 4;
    }

  q_free(queue, QUEUE_NODEALLOC);

  return 0;
}
