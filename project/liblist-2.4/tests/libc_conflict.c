/* libc_conflict.c -- try to include as many system headers as possible to check for conflicts.
 *
 * Copyright (C) 2011 Nathan Phillip Brink <ohnobinki@ohnopublishing.net>
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
#include <stack.h>
#include <queue.h>

#ifdef HAVE_ARPA_INET_H
#include <arpa/inet.h>
#endif
#ifdef HAVE_INTTYPES_H
#include <inttypes.h>
#endif
#ifdef HAVE_STDINT_H
#include <stdint.h>
#endif
#ifdef HAVE_SYS_SIGNAL_H
#include <sys/signal.h>
#endif
#ifdef HAVE_SYS_WAIT_H
#include <sys/wait.h>
#ifdef HAVE_SYS__STRUCTS_H
#include <sys/_structs.h>
#endif
#endif
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#include <locale.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
  list_t list;
  lstack_t stack;
  queue_t queue;

  list = list_init();
  stack = stack_init();
  queue = q_init();

  list_free(list, LIST_NODEALLOC);
  stack_free(stack, STACK_NODEALLOC);
  q_free(queue, QUEUE_NODEALLOC);

  return 0;
}
