/* list_namespace.h -- rename functions to prevent symbol conflicts.
 * Particularly for libmysqlclient.so which exports list_free()
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

#ifndef _LIBLIST_LIST_NAMESPACE_H
#define _LIBLIST_LIST_NAMESPACE_H

/**
 *  \file
 * Rename all of the functions/symbols declared in list.h to avoid
 * symbol collisions. Most notable is libmysqlclient.so which exports
 * list_free().  These \#define-s are here to allow list.h to become
 * more clear/concise.  Ideally, the list.h should be usable as an API
 * reference.
 */

#define list_brag liblist_list_brag
#define list_free_free liblist_list_free_free
#define list_init liblist_list_init
#define list_mvprev liblist_list_mvprev
#define list_mvnext liblist_list_mvnext
#define list_insert_before liblist_list_insert_before
#define list_insert_after liblist_list_insert_after
#define list_remove_front liblist_list_remove_front
#define list_remove_rear liblist_list_remove_rear
#define list_remove_curr liblist_list_remove_curr
#define list_remove_element liblist_list_remove_element
#define list_free liblist_list_free
#define list_front liblist_list_front
#define list_curr liblist_list_curr
#define list_rear liblist_list_rear
#define list_mvfront liblist_list_mvfront
#define list_mvrear liblist_list_mvrear
#define list_empty liblist_list_empty
#define list_size liblist_list_size
#define list_traverse liblist_list_traverse

#endif /* _LIBLIST_LIST_NAMESPACE_H */
