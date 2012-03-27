# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

AC_INIT([liblist], [2.4], [https://ohnopublishing.net/bugzilla/enter_bug.cgi?product=liblist], [liblist], [http://ohnopub.net/liblist])
AC_CONFIG_SRCDIR([list.c])

AM_INIT_AUTOMAKE([dist-bzip2 parallel-tests subdir-objects])

AM_PROG_CC_C_O
LT_INIT([win32-dll])

AC_TYPE_INTPTR_T

AC_ARG_ENABLE([examples],
	[AS_HELP_STRING([--enable-examples], [attempt to compile and install examples])],
	[enable_examples=$enableval],
	[enable_examples=no])
AM_CONDITIONAL([EXAMPLES], [test "x$enable_examples" = "xyes"])

AC_ARG_ENABLE([docs],
	[AS_HELP_STRING([--enable-docs], [Compile and install a paper from this lib's original author])],
	[enable_docs=$enableval],
	[enable_docs=no])
AM_CONDITIONAL([DOCS], [test "x$enable_docs" = "xyes"])

dnl for test/libc_conflict.c
AC_CHECK_HEADERS([arpa/inet.h inttypes.h stdint.h sys/signal.h sys/wait.h sys/_structs.h unistd.h])

AC_CONFIG_FILES([Makefile
	liblist.pc])
AC_OUTPUT
