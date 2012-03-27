/* list_internal.h -- private data structures for generic list package
 *
 * Nathan Phillip Brink 2010/04/26
 * 
 * Copyright (C) 2010, Nathan Phillip Brink <ohnobinki@ohnopublishing.net>
 * modification of material Copyright (C) 1992, Brandly C. Spatz, bcs@ufl.edu
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

#ifndef _LIBLIST_LIST_PRIVATE_H
#define _LIBLIST_LIST_PRIVATE_H

#include "list.h"

struct list_element;
/**
 * \internal
 * \brief Define a structure to describe each element in the list.
 */
typedef struct list_element *list_element_t;

struct list
{
  size_t size;
  list_element_t front;
  list_element_t rear;
  list_element_t curr;
};

struct list_element
{
  list_element_t prev;
  list_element_t next;
  void *data;
};



#endif /* _LIBLIST_LIST_PRIVATE_H */
