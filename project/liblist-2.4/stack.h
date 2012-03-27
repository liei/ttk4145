/* stack.h -- present stack abstraction with list primitives.
 *
 * Last edited: Tue Jul 28 15:33:54 1992 by bcs (Bradley C. Spatz) on wasp
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

#ifndef _LIBLIST_STACK_H
#define _LIBLIST_STACK_H

/**
 * \file
 * \brief Stack-style interface to lists.
 */

#include <list.h>

/* Present the stack datatypes in terms of list types (our point exactly). */
/**
 * \deprecated A stack handle.
 */
#define lstack_t list_t

/* Backwards compatibility */
/**
 * \deprecated API backwards-compatibility.
 */
#define STACK LIST
#define STACK_ELEMENT LIST_ELEMENT

/**
 * \file
 * Now map the stack functions onto the list primitives. The current
 * element pointer will always point to front of the list, which is
 * where we push and pop. With this model, we can map into the list
 * primitives directly.
 */

/**
 * \brief Allocate and initialize a stack.
 *
 * \return A new stack or NULL if there was an error.
 */
#define stack_init()                     list_init()

/**
 * \brief Push an element onto the top of the stack.
 *
 * \param stack The stack.
 * \param element The element.
 * \param len The number of bytes to allocate for a shallow copy of
 *   element if that is desired.  0 if it is not desired that a
 *   shallow copy be made of the pushed element.
 * \return A poiner to element or the new copy of element as stored in
 *   the stack.
 */
#define stack_push(stack, element, len)   list_insert_before(stack, element, len)

/**
 * \brief Pop an element off of the top of the stack.
 *
 * \param stack The stack.
 * \return The element which was removed from the top of the stack or
 *   NULL if the stack is empty.
 */
#define stack_pop(stack)                 list_remove_front(stack)

/**
 * \brief Peek at the element at the top of the stack.
 *
 * \param stack The stack.
 * \return The element at the top of the stack or NULL if the stack is
 *   empty.
 */
#define stack_top(stack)                 list_front(stack)

/**
 * \brief Retrive the number of elements in the stack.
 *
 * \param stack The stack.
 * \return The number of elements in the stack.
 */
#define stack_size(stack)                list_size(stack)

/**
 * \brief Check of the stack is empty.
 *
 * \param stack The stack.
 * \return 1 if the stack is empty. 0 if the stack has one or more
 *   items in it.
 */
#define stack_empty(stack)               list_empty(stack)

/**
 * \brief Free a stack
 *
 * \param stack The stack to free.
 * \param dealloc A function to apply to each element as it is removed
 *   from the stack in the process of freeing the stack.
 * \sa list_free()
 */
#define stack_free(stack, dealloc)       list_free(stack, dealloc)

/* Define the deallocation constants. */
/**
 * \brief Instruct stack_free() to deallocate each element.
 *
 * Only valid if the value of len in your stack_push() calls was
 * non-zero.
 *
 * \sa LIST_DEALLOC
 */
#define STACK_DEALLOC   LIST_DEALLOC

/**
 * \brief Instruct stack_free() to do nothing to each element.
 *
 * \sa stack_free()
 * \sa LIST_NODEALLOC
 */
#define STACK_NODEALLOC LIST_NODEALLOC

#endif /* _LIBLIST_STACK_H */
