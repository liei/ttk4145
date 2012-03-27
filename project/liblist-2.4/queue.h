/* queue.h -- present queue abstraction with list primitives.
 *
 * Last edited: Tue Jul 28 15:34:15 1992 by bcs (Bradley C. Spatz) on wasp
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

#ifndef _LIBLIST_QUEUE_H
#define _LIBLIST_QUEUE_H

/**
 * \file
 * \brief Queue-style interface to lists.
 */

#include <list.h>

/* define queue objects in terms of list */
/**
 * \brief A queue handle.
 */
#define queue_t list_t

/* backwards compatibility */
/**
 * \deprecated API backwards-compatibility.
 */
#define QUEUE LIST
#define QUEUE_ELEMENT LIST_ELEMENT

/**
 * \file
 * Map the queue functions onto the list primitives.  The current
 * element pointer will always point to the end of the list, which is
 * where we add new elements.  We remove elements from the front.
 * With this model, we map onto the list primitives directly.
 */

/**
 * \brief Allocate and initialize a queue.
 *
 * \return A new queue or NULL if there was an error.
 */
#define q_init()                        list_init()

/**
 * \brief Enqueue an element at the rear of the queue.
 *
 * \param queue The queue.
 * \param element The element to enqueue.
 * \param len The number of bytes to allocate and copy out of element
 *   if a shallow copy is desired. 0 if q_enqueue should just store
 *   the element pointer without making a shallow copy.
 * \return A pointer to element or the shallow copy of element.
 */
#define q_enqueue(queue, element, len)   list_insert_after(queue, element, len)

/**
 * \brief Dequeue an element from the front of the queue.
 *
 * \param queue The queue.
 * \return The element at the front of the queue or NULL if the queue
 *   is empty.
 */
#define q_dequeue(queue)                list_remove_front(queue)    

/**
 * \brief Peek at the front item of a queue.
 *
 * \param queue The queue.
 * \return The element at the front of the queue or NULL if the queue
 *   is empty.
 */
#define q_front(queue)                  list_front(queue)

/**
 * \brief Retrieve the number of elements in the queue.
 *
 * \param queue The queue.
 * \return The number of elements in the queue.
 */
#define q_size(queue)                   list_size(queue)

/**
 * \brief Check if a queue is empty.
 *
 * \param queue The queue.
 * \return 0 if the queue has one or more elements in it. 1 if the
 *   queue is empty.
 */
#define q_empty(queue)                  list_empty(queue)

/**
 * \brief Free a queue.
 *
 * \param queue The queue.
 * \param dealloc A function to apply to each list element before it's
 *   deleted. May be QUEUE_DEALLOC, QUEUE_NODEALLOC, or a custom
 *   function.
 * \sa list_free()
 */
#define q_free(queue, dealloc)          list_free(queue, dealloc)

/* Define the deallocation constants. */
/**
 * \brief Instruct q_free() to free each list item.
 *
 * Only valid if you gave q_enqueue() a non-zero len argument for each
 * list member.
 *
 * \sa q_free()
 * \sa LIST_DEALLOC
 */
#define QUEUE_DEALLOC   LIST_DEALLOC
/**
 * \brief Instruct q_free() to not deallocate or do anything to each
 *   queue element.
 *
 * \sa q_free()
 * \sa LIST_NODEALLOC
 */
#define QUEUE_NODEALLOC LIST_NODEALLOC

#endif /* _LIBLIST_QUEUE_H */
