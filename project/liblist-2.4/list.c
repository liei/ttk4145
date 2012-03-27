/* list.c -- a generic list package
 * 
 * Last edited: Tue Jul 28 15:37:24 1992 by bcs (Bradley C. Spatz) on wasp
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
 *    list_t list_init()
 *    list_t list_mvprev(list)
 *    list_t list_mvnext(list)
 *    char *list_insert_before(list, element, len)
 *    char *list_insert_after(list, element, len)
 *    char *list_remove_front(list)
 *    char *list_remove_curr(list)
 *    char *list_remove_rear(list)
 *    list_status_t list_remove_element(list, element);
 *    list_status_t list_traverse(list, data, func, opts);
 *    void list_free(list, dealloc)
 *
 *    char *list_front(list)
 *    char *list_curr(list)
 *    char *list_rear(list)
 *    list_t list_mvfront(list)
 *    list_t list_mvrear(list)
 *    size_t list_size(list)
 *    int list_empty(list)
 *
 * for
 *
 *    list_t list;
 *    void *element;
 *    size_t len;
 *    int func(void *data, void *curr)
 *    void dealloc(void *data)
 */

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "list.h"
#include "list_private.h"

const char *list_brag = PACKAGE_STRING "\n\
Copyright (C) 1992 Bradley C. Spatz\n\
Maintained by Nathan Phillip Brink <binki@ohnopub.net> 2010\n";

list_t list_init()
{
   list_t list;

   /* Allocate, initialize, and return a new list. */
   list = (list_t) malloc(sizeof(struct list));
   if (list == NULL)
      return(NULL);
   list->size = 0;
   list->front = NULL;
   list->rear = NULL;
   list->curr = NULL;
   return(list);
}


list_t list_mvprev(list_t list)
{
   /* Move to the previous link, if possible.  Note that the following
    * compound conditional expression *requires* a short-circuit evaluation.
    */
   if ((list->curr != NULL) && (list->curr->prev != NULL)) {
      list->curr = list->curr->prev;
      return(list);
   }
   else
      return(NULL);
}


list_t list_mvnext(list_t list)
{
   /* Move to the next link, if possible.  Note that the following
    * compound conditional expression *requires* a short-circuit evaluation.
    */
   if ((list->curr != NULL) && (list->curr->next != NULL)) {
      list->curr = list->curr->next;
      return(list);
   }
   else
      return(NULL);
}


list_t list_mvfront(list_t list)
{
   /* Move to the front of the list.*/
   list->curr = list->front;
   return(list);
}


list_t list_mvrear(list_t list)
{
   /* Move to the front of the list.*/
   list->curr = list->rear;
   return(list);
}


int list_empty(list_t list)
{
   /* Return 1 if the list is empty.  0 otherwise. */
   return((list->front == NULL) ? TRUE : FALSE);
}


void *list_front(list_t list)
{
   return((list->front == NULL) ? NULL : (list->front->data));
}


void *list_curr(list_t list)
{
   return((list->curr == NULL) ? NULL : (list->curr->data));
}


void *list_rear(list_t list)
{
   return((list->rear == NULL) ? NULL : (list->rear->data));
}


size_t list_size(list_t list)
{
   return(list->size);
}


static list_element_t list_create_element(void *data, size_t len)
{
   list_element_t new;

   /* Allocate storage for the new node and its data.  Return NULL if
    * unable to allocate.
    */
   new = (list_element_t) malloc(sizeof(struct list_element));
   if (new == NULL) {
      return(NULL);
   }

   /* Allocate storage for the data only if requested; i.e. if len > 0.
    * Then either copy the data or just the reference into the node.
    */
   if (len > 0) {
      new->data = (char *) malloc(len);
      if (new->data == NULL) {
         free(new);
         return(NULL);
      }
      (void) memcpy(new->data, data, len);
   }
   else {
      new->data = (char *) data;
   }

   return(new);
}


void *list_insert_before(list_t list, void *data, size_t len)
{
   list_element_t new;

   /* Allocate storage for the new element and its data.*/
   new = list_create_element(data, len);
   if (new == NULL)
      return(NULL);

   /* Now insert the element before the current, considering the cases:
    *    1) List is empty
    *    2) Inserting at front
    *    3) Otherwise
    * We handle them directly, in order.
    */
   if (list->front == NULL) {
      /* The list is empty.  Easy. */
      new->prev = new->next = NULL;
      list->front = list->rear = list->curr = new;
   }
   else if (list->curr->prev == NULL) {
      /* Inserting at the front. */
      new->prev = NULL;
      new->next = list->curr;
      list->curr->prev = new;
      list->front = new;
   }
   else {
      /* Otherwise. */
      new->prev = list->curr->prev;
      list->curr->prev->next = new;
      new->next = list->curr;
      list->curr->prev = new;
   }

   list->curr = new;
   list->size++;
   return(new->data);
}


void *list_insert_after(list_t list, void *data, size_t len)
{
   list_element_t new;

   /* Allocate storage for the new element and its data.*/
   new = list_create_element(data, len);
   if (new == NULL)
      return(NULL);

   /* Now insert the element after the current, considering the cases:
    *    1) List is empty
    *    2) Inserting at rear
    *    3) Otherwise
    * We handle them directly, in order.
    */
   if (list->front == NULL) {
      /* The list is empty.  Easy. */
      new->prev = new->next = NULL;
      list->front = list->rear = list->curr = new;
   }
   else if (list->curr->next == NULL) {
      /* Inserting at the rear. */
      new->next = NULL;
      new->prev = list->curr;
      list->curr->next = new;
      list->rear = new;
   }
   else {
      /* Otherwise. */
      new->next = list->curr->next;
      new->next->prev = new;
      new->prev = list->curr;
      list->curr->next = new;
   }

   list->curr = new;
   list->size++;
   return(new->data);
}


static void *list_remove_single(list_t list)
{
   char *data;

   /* The list has one element.  Easy. */
   data = list->curr->data;
   free(list->curr);
   list->front = list->rear = list->curr = NULL;
   list->size--;
   return (data);
}


void *list_remove_front(list_t list)
{
   list_element_t temp;
   char *data;

   /* Removing and return front element, or NULL if empty.  If curr
    * is the front, then curr becomes the next element.
    */
   if (list->front == NULL) {
      /* List is empty.  Easy. */
      return(NULL);
   }
   else if (list->front == list->rear) {
      /* List has only one element.  Easy. */
      data = list_remove_single(list);
   }
   else {
      /* List has more than one element.  Make sure to check if curr
       * points to the front.
       */
      data = list->front->data;
      list->front->next->prev = NULL;
      temp = list->front;
      list->front = temp->next;
      if (list->curr == temp)
	 list->curr = temp->next;
      free(temp);
      list->size--;
   }

   return(data);
}


void *list_remove_rear(list_t list)
{
   list_element_t temp;
   char *data;

   /* Removing and return rear element, or NULL if empty.  If curr
    * is the rear, then curr becomes the previous element.
    */
   if (list->front == NULL) {
      /* List is empty.  Easy. */
      return(NULL);
   }
   else if (list->front == list->rear) {
      /* List has only one element.  Easy. */
      data = list_remove_single(list);
   }
   else {
      /* List has more than one element.  Make sure to check if curr
       * points to the rear.
       */
      data = list->rear->data;
      list->rear->prev->next = NULL;
      temp = list->rear;
      list->rear = temp->prev;
      if (list->curr == temp)
	 list->curr = temp->prev;
      free(temp);
      list->size--;
   }

   return(data);
}


void *list_remove_curr(list_t  list)
{
   list_element_t temp;
   char *data;

   /* Remove the current element, returning a pointer to the data, or
    * NULL if the list is empty.  Set curr to the next element unless
    * curr is at the rear, in which case curr becomes the previous
    * element.
    */
   if (list->front == NULL) {
      /* The list is empty.  Easy. */
      return(NULL);
   }
   else if (list->front == list->rear) {
      /* The list has one element.  Easy. */
      data = list_remove_single(list);
   }
   else if (list->curr == list->front) {
      /* Removing front element.  Easy. */
      data = list_remove_front(list);
   }
   else if (list->curr == list->rear) {
      /* Removing from the rear.  Easy.*/
      data = list_remove_rear(list);
   }
   else {
      /* Otherwise.  Must be inside a 3-element or larger list. */
      data = list->curr->data;
      temp = list->curr;
      temp->next->prev = temp->prev;
      temp->prev->next = temp->next;
      list->curr = temp->next;
      free(temp);
      list->size--;
   }

   return(data);
}

static int _list_remove_element_traverse(void *element_to_find, void *element)
{
  if(element_to_find == element)
    return FALSE;
  return TRUE;
}

/* element refers to the data member of struct list_element */
list_status_t list_remove_element(list_t list, void *element)
{
  int hit;

  hit = 0;

  list_mvfront(list);
  while(list_traverse(list, element, &_list_remove_element_traverse, LIST_FORW|LIST_CURR|LIST_ALTR) == LIST_OK)
    {
      list_remove_curr(list);
      list_mvnext(list);
      hit = 1;
    }

  if(hit)
    return LIST_OK;
  return LIST_EXTENT;
}


list_status_t list_traverse(list_t list, void *data, list_traverse_func_t func, int opts)
{
   list_element_t lp;
   int status, rc;

   /* Traverse the list according to opts, calling func at each element,
    * until func returns 0 or the extent of the list is reached.  We return
    * 0 if the list is empty, 2 if we tried to go beyond the extent of the
    * list, and 1 otherwise.  We may or may not affect the current element
    * pointer.
    */
   if (list->front == NULL)
      return(LIST_EMPTY);
   
   /* Decide where to start. */
   if ((opts & LIST_CURR) == LIST_CURR) {
      lp = list->curr;
   }
   else if ((opts & LIST_REAR) == LIST_REAR) {
      lp = list->rear;
   }
   else {
      lp = list->front;
   }
   
   /* Now decide if to update the current element pointer. */
   if ((opts & LIST_ALTR) == LIST_ALTR)
      list->curr = lp;

   /* Now go until 0 is returned or we hit the extent of the list. */
   rc = LIST_OK;
   status = TRUE;
   while(status) {
      status = (*func)(data, lp->data);
      if (status) {
	 if ((((opts & LIST_BACK) == LIST_BACK) ? (lp->prev) : (lp->next))
	     == NULL) {
	    /* Tried to go beyond extent of list. */
	    status = FALSE;
	    rc = LIST_EXTENT;
	 }
	 else {
	    /* Decide where to go next. */
	    lp = (((opts & LIST_BACK) == LIST_BACK) ? (lp->prev) : (lp->next));

	    /* Now decide if to update the current element pointer. */
	    if ((opts & LIST_ALTR) == LIST_ALTR)
	       list->curr = lp;
	 }
      }
   }

   return(rc);
}


void list_free(list_t list, list_dealloc_func_t dealloc)
{
   char *data;

   /* Move to the front of the list.  Start removing elements from the
    * front.  Free up the data element by either applying the user-supplied
    * deallocation routine or free(3).  When we've gone through all the
    * elements, free the list descriptor.
    */
   list_mvfront(list);
   while (! list_empty(list))
     {
       data = list_remove_front(list);

       if(dealloc)
	 (*dealloc)(data);
     }

   free(list);
}

void list_free_free(void *data)
{
  free(data);
}
