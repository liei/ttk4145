/* list.h -- data structures and such for generic list package
 * 
 * Last edited: Tue Jul 28 15:29:56 1992 by bcs (Bradley C. Spatz) on wasp
 * Updated by Nathan Phillip Brink 2010
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
 */

#ifndef _LIBLIST_LIST_H
#define _LIBLIST_LIST_H

/**
 * \mainpage liblist
 *
 * \section purpose Purpose
 *
 * liblist provides a simple abstraction around doubly-linked
 * lists. The following are the basic and common arguments for the use
 * of any such library:
 *   - A client program needs not reimplement such a common algorithm.
 *   - Sharing well-tested and proven code eliminates one source of
 *     failure or mistake-making.
 *   - Using an API's functions generally results in cleaner and clearer
 *     code than directly manipulating members of data structures.
 *
 * \section api API
 *
 * liblist's public API is defined in list.h and \ref
 * list_traversal. A list handle is represented by the list_t
 * typedef. This is initialized with list_init() and freed with
 * list_free().
 *
 * A list handle has a front, rear, and current element. The current
 * element may be accessed with list_curr() and the other elements
 * would be access with similarly named functions. Inserting new
 * elements into the list causes the current element to refer to the
 * newly inserted element. The current element pointer may also be
 * changed with list_mvnext(), list_mvprev(), list_mvfirst(), and
 * list_mvrear().
 *
 * When updating or accessing a list, often operations are performed
 * on the whole list at once or a search is done to find a certain
 * element in the list. These operations may be performed using the
 * basic list manipulation functions mentioned above. One may apply a
 * function conforming to list_traverse_func_t to list elements using
 * list_traverse(). The return value of this function can interrupt
 * the traversal and the traversal can update the current element,
 * making it useful for performing searches.
 *
 * \section alternate_paradigms Alternate Paradigms
 *
 * The doubly-linked list structure can easily be treated as if it
 * supports the queue and stack paradigms. For example, restricting
 * yourself to list_insert_before(), list_front(), and
 * list_remove_front() lets the list behave as a stack. stack.h and
 * queue.h provide macros to provide alternate interfaces.
 */

/**
 * \file
 *
 * \brief
 *   Generic list operations.
 * \sa list_traversal
 */

#include <list_namespace.h>
#include <stddef.h>

/**
 * \brief
 *   A string containing the version and authors of liblist.
 */
extern const char *list_brag;

/**
 * \brief
 *   Common return values for list_traverse() and list_remove_element().
 */
typedef enum list_status
  {
    /**
     * \brief
     *   The list is empty.
     *
     * The usefulness of such a return value is arguable, as in many
     * circumstances one will not care about the difference between
     * LIST_EMPTY and LIST_OK.
     */
    LIST_EMPTY = 0,
    /**
     * \brief The operation completed successfully.
     */
    LIST_OK = 1,
    /**
     * \brief
     *   The end of the list was reached.
     *
     * For list_traverse(), this means that the traversal function
     * returned TRUE even though it was accessing the last element of
     * the list. (Such an occurance is normal). For
     * list_remove_element(), this means that no elements were removed
     * from the list. I.e., it reached the end of the list without
     * finding anything to do even though it would traverse the whole
     * list anyway.
     */
    LIST_EXTENT = 2,
  } list_status_t;

/* Define a structure to describe the list. */
struct list;
/**
 * \brief
 *   A list handle.
 */
typedef struct list *list_t;

/**
 * Backwards API compatibility plug.
 * We're willing to make the API as backwards-compatible as possible
 * but we are going to enforce opaque handles, eliminating the macros
 * instead of functions option.
 *
 * \deprecated These #defines must be preserved for eternity, but don't use them!
 */
struct list_element;
/**
 * \deprecated Kept only for backwords compatibility.
 */
#define LIST_ELEMENT struct list_element
/**
 * \deprecated Kept only for backwards compatibility.
 */
#define LIST struct list

/**
 * \ingroup list_traversal
 * \brief
 *   A function prototype for the callback passed to list_traverse().
 *
 * \param data The same pointer passed to list_traverse() to provide a
 *   traversal with context information.
 * \param element A list element to process or test.
 * \return TRUE if the traversal should continue or FALSE if the
 *   traversal should be terminated.
 */
typedef int (*list_traverse_func_t)(void *data, void *element);

/**
 * \brief
 *   A function prototype for the callback passed to list_free().
 *
 * The purpose of this callback is to deallocate an element of the
 * list when list_free() is called. For this reason, it has the same
 * signature as free() and doesn't have a context data pointer like
 * list_traverse_func_t does.
 *
 * \param element The element which should be deallocated.
 */
typedef void (*list_dealloc_func_t)(void *element);

/**
 * \brief
 *   A default element-freeing function for LIST_DEALLOC.
 *
 * This essentially wraps around free().  You should use this to free
 * any memory allocated by passing a nonzero len to
 * list_insert_after() or list_insert_before().
 *
 * \param element The element to deallocate using the system
 *   deallocation function.
 */
void list_free_free(void *element);

/* Prototype ahoy! */

/**
 * \brief
 *   Allocate an initialize a new list_t handle.
 *
 * Produces an empty doubly-linked list.
 */
list_t list_init();

/**
 * \brief
 *   Move the list's current element backward (towards the list's front).
 *
 * \param list The list.
 * \return The list handle or NULL if there was an attempt to move
 *   beyond the front of the list.
 */
list_t list_mvprev(list_t list);

/**
 * \brief
 *   Move the current element forward (towards the list's rear).
 * \param list The list.
 * \return The list handle or NULL if there was an attempt to move
 *   beyond the rear of the list.
 */
list_t list_mvnext(list_t list);

/**
 * \brief
 *   Insert an item into the list positioned before the current
 *   element.
 *
 * After the insertion, the list's current element is set to the newly
 * inserted element.
 *
 * If the list is holding items such as strings or shallow copies of
 * structs, this function can handle memory allocation for you. To
 * request liblist to allocate memory for storage of element, set len
 * to the number of bytes that should be allocated and copied out of
 * element.
 *
 * \param list The list into which an element should be inserted.
 * \param element The pointer or data that should be stored.
 * \param len If zero, the list will store the element pointer. If
 *   nonzero, this specifies the size of the memory to allocate for
 *   storage of element and indicates that a shallow copy should be
 *   made of element and stored in the list. If nonzero, you should
 *   pass LIST_DEALLOC to list_free() when freeing the list.
 * \return A pointer to element. If len is zero, this is the same as
 *   the element passed in. This is useful for when len is nonzero, as
 *   the newly allocated pointer will be returned.
 */
void *list_insert_before(list_t list, void *element, size_t len);

/**
 * \brief
 *   Insert an item into the list positioned after the current
 *   element.
 *
 * After adding an item, the list's current element is set to the new
 * element. This lends itself to populating a newly created list with
 * the contents of an array while preserving the array's order. To do
 * so, just call list_inert_after() repeatedly for each successive
 * member of the array.
 *
 * \param list The list into which an element should be inserted.
 * \param element The pointer to store in the list.
 * \param len If zero, then the element pointer shall be stored
 *   directly into the list. If nonzero, this specifies the number of
 *   bytes necessary to store element. When nonzero, a shallow copy
 *   shall be made of element and stored in the list.
 * \return The pointer to the just-inserted element. This is useful
 *    when len is nonzero because then memory is allocated to store
 *    element and the caller won't know the pointer to this memory
 *    beforehand.
 */
void *list_insert_after(list_t list, void *element, size_t len);

/**
 * \brief
 *   Remove and return the front element of a list.
 *
 * \param list The list whose front element should be removed.
 * \return The removed element or NULL if the list is empty.
 */
void *list_remove_front(list_t list);

/**
 * \brief
 *   Remove and return the curent element of a list.
 *
 * \param list The list.
 * \return The removed element or NULL if the list is empty.
 */
void *list_remove_rear(list_t list);

/**
 * \brief
 *   Remove and return the rear element of a list.
 *
 * \param list The list.
 * \return The removed element or NULL if the list is empty.
 */
void *list_remove_curr(list_t list);

/**
 * \brief
 *   Convenience function to remove an element from a list if its
 *   pointer is known.
 *
 * This is only to be useful when the second argument to
 * list_insert_before() or list_insert_after() is 0. Will remove
 * multiple entries of element, at the expense of avoiding a shortcut
 * if you know there is only one entry of element in list.
 *
 * The list's current element (i.e., list_curr(), list_mvnext(), etc.)
 * is NOT preserved.
 *
 * \param list The list from which to remove the element.
 * \param element The pointer by which the element is accessible.
 * \return LIST_OK if successful, LIST_EXTENT if the element is not found.
 */
list_status_t list_remove_element(list_t list, void *element);

/**
 * \brief
 *   Deallocate a list element pointer.
 *
 * Only to be used as an argument for list_free().
 *
 * \sa list_free()
 */
/**
 * \brief
 *   Indicates that list_free() should deallocate memory that was
 *   allocated by liblist.
 *
 * This undoes the allocation performed when passing a nonzero len to
 * list_insert_after() or list_insert_before().
 *
 * It would be improper to use this option to free list items if you
 * use malloc() and free() on your list elements. Currently, this is
 * just a wrapper around free() but this is not guaranteed to always
 * be true.
 */
#define LIST_DEALLOC   (&list_free_free)
/**
 * \brief
 *   A no-op second argument for list_free() that causes
 *   list_free() to not do anything with each element as it's removed
 *   from the list before deallocating the list.
 */
#define LIST_NODEALLOC (list_dealloc_func_t)NULL

/**
 * \brief
 *   Deallocate all members of a list and free the list's handle.
 *
 * Before the list handle itself is deallocated, each member of the
 * list is removed from the list. In most cases, the memory associated
 * with each list item should be deallocated at this point. Thus, this
 * function accepts a second argument which allows a function to be
 * applied to each member of the list as it is removed. Assumably,
 * this function would deallocate each list element it recieves.
 *
 * \param list The list handle to free.
 * \param dealloc_func A function to apply to each element of the list
 *   as it is removed. This may be LIST_DEALLOC if list items were
 *   added using liblist's own allocation mechanism (i.e., if
 *   list_insert_after()'s len argument was non-zero). It may be
 *   LIST_NODEALLOC if the elements should not be processed or
 *   freed. Or it may be a pointer to a custom-defined function which
 *   implements the list_dealloc_func_t prototype.
 */
void list_free(list_t list, list_dealloc_func_t dealloc_func);

/**
 * \defgroup list_traversal List Traversal
 */

/**
 * \ingroup list_traversal
 * Define some constants for controlling list traversals.  We
 * bit-code the attributes so they can be OR'd together.
 */

/**
 * \ingroup list_traversal
 * \brief
 *   The traversal should move forward.
 *
 * The traversal should move towards the list's rear element.
 */
#define LIST_FORW	0
/**
 * \ingroup list_traversal
 * \brief
 *   The traversal should move backwards.
 *
 * The traversal should move towards the list's front element.
 */
#define LIST_BACK	2
/**
 * \ingroup list_traversal
 * \brief
 *   The traversal should begin at the front of the list.
 *
 * Implies LIST_FORW.
 */
#define LIST_FRNT	4
/**
 * \ingroup list_traversal
 * \brief
 *   The traversal should begin as the list's current element.
 *
 * Use this if you want to resume an interrupted list traversal or
 * start a traversal at an arbitrary point withing a list.
 */
#define LIST_CURR	8
/**
 * \ingroup list_traversal
 * \brief
 *   The traversal should begin at the list's rear element.
 *
 * Implies LIST_BACK, since a traversal starting at the end of a list
 * would be otherwise pointless.
 */
#define LIST_REAR	(16 | LIST_BACK)
/**
 * \ingroup list_traversal
 * \brief
 *   The traversal should not update the list's current element.
 *
 * Use this if you want the list's current element to remain untouched
 * during a traversal. This is useful if you depend on the
 * list_mvprev(), list_mvnext(), and list_curr() functions to be
 * stable and yet need to apply a function to the entire list.
 */
#define LIST_SAVE	32
/**
 * \ingroup list_traversal
 * \brief
 *   The traversal should update the list's current element.
 *
 * Use this if you want to search through the list for a particular
 * element using a traversal and then want to operate on this element
 * after the traversal is complete. This is the recommended way to
 * delete particular list items, since it is unsafe to delete items
 * from inside of a list traversal.
 *
 * If you are using this option for searches or selecting elements to
 * delete, don't forget to check if list_traverse() is returning
 * LIST_EXTENT instead of LIST_OK or LIST_EMPTY. Otherwise, your code
 * might mistake the last element of the list for the sought element
 * (particularly if the sought element isn't in the list or if
 * multiple elements in the list are being sought and you are resuming
 * the search).
 */
#define LIST_ALTR	64

/* Define some constants for return codes and such. */
#ifndef TRUE
/**
 * \ingroup list_traversal
 * \brief
 *   Indicate that a traversal should continue.
 *
 * If a list traversal function returns this, list_traverse() will try
 * to continue on to the next (for LIST_FORW) or previous (for
 * LIST_BACK) element in the list. If this would result in
 * list_traverse() going beyond the end of the list, that function
 * will terminate the traversal and return LIST_EXTENT.
 */
#define TRUE  1
#endif
#ifndef FALSE
/**
 * \ingroup list_traversal
 * \brief
 *   Indicate that a traversal should stop.
 *
 * If a list traversal function returns this, list_traverse() will
 * terminate the traversal and return LIST_OK.
 *
 * \sa LIST_ALTR
 */
#define FALSE 0
#endif

/* Yet more prototypes. */

/**
 * \brief
 *   Retrieve the element at the front of a list.
 *
 * \param list The list.
 * \return The element at the front of list or NULL if the list is empty.
 */
void *list_front(list_t list);

/**
 * \brief
 *   Retrieve the list's current element.
 *
 * \param list The list.
 * \return The list's current element or NULL if the list is empty.
 */
void *list_curr(list_t list);

/**
 * \brief
 *   Retrieve the element at the rear of a list.
 *
 * \param list The list.
 * \return The list's rear element or NULL if the list is empty.
 */
void *list_rear(list_t list);

/**
 * \brief
 *   Set the list's current element to the front of the list.
 *
 * \param list The list.
 * \return The same list passed in (even if the list is empty, this
 *   will be non-NULL).
 */
list_t list_mvfront(list_t list);

/**
 * \brief
 *   Set the list's current element to the rear of the list.
 *
 * \param list The list.
 * \return The same list passed in.
 */
list_t list_mvrear(list_t list);

/**
 * \brief
 *   Check if the list is empty
 *
 * If this function returns 1, then list_size() would return 0.
 *
 * \return 1 if the list is empty, 0 if the list has one or more
 *   elements.
 */
int list_empty(list_t);

/**
 * \brief
 *   Retrieve the number of elements in the list.
 *
 * If this function returns 0, then list_empty() would return 1.
 *
 * \return The number of elements in the list.
 */
size_t list_size(list_t);

/**
 * \ingroup list_traversal
 * \brief
 *   Traverse a list and apply func to all or some of the list's
 *   elements.
 *
 * \par Traversal Behavior
 * Different macros ORed together determine the behavior of a list
 * traversal. The direction of the traversal is controlled by
 * LIST_FORW (default) and LIST_BACK. The starting element of the
 * traversal is chosen with LIST_FRONT (default), LIST_CURR, and
 * LIST_REAR. Whether or not to update the list's current element is
 * determined by LIST_SAVE (default)_ and LIST_ALTR.
 *
 * \param list The list to traverse.
 * \param data A pointer to pass to func to give it context.
 * \param func The function to apply to the visited members of the list.
 * \param opts An ORing of the following macros deciding the behavior
 *   of the traversal.
 * \return A list_status_t indicating if the list was empty, if the
 *   end of the list was reached during the traversal, or if the
 *   traversal was stopped before the end of the list (LIST_OK).
 */
list_status_t list_traverse(list_t list, void *data, list_traverse_func_t func, int opts);

#endif
