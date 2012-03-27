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

dist_man_MANS = man/liblist.3 man/list.h.0 \
	man/liblist_queue.3 man/queue.h.0 \
	man/liblist_stack.3 man/stack.h.0 \
	\
	man/list_init.3 \
	man/list_mvprev.3 \
	man/list_mvnext.3 \
	man/list_mvfront.3 \
	man/list_mvrear.3 \
	man/list_front.3 \
	man/list_curr.3 \
	man/list_rear.3 \
	man/list_insert_before.3 \
	man/list_insert_after.3 \
	man/list_remove_front.3 \
	man/list_remove_curr.3 \
	man/list_remove_rear.3 \
	man/list_remove_element.3 \
	man/list_size.3 \
	man/list_empty.3 \
	man/list_traverse.3 \
	man/list_free.3 \
	\
	man/q_init.3 \
	man/q_enqueue.3 \
	man/q_dequeue.3 \
	man/q_front.3 \
	man/q_size.3 \
	man/q_empty.3 \
	man/q_free.3 \
	\
	man/stack_init.3 \
	man/stack_push.3 \
	man/stack_pop.3 \
	man/stack_top.3 \
	man/stack_size.3 \
	man/stack_empty.3 \
	man/stack_free.3
include_HEADERS = list.h list_namespace.h queue.h stack.h
lib_LTLIBRARIES = liblist.la

if EXAMPLES
dist_man_MANS += examples/cache/lcache.3
include_HEADERS += examples/cache/cache.h
lib_LTLIBRARIES += libcache.la
endif

liblist_la_SOURCES = list.c list_private.h
liblist_la_LDFLAGS = $(AM_LDFLAGS) -version-info 3:0:0 -no-undefined -export-symbols-regex '^liblist_'

libcache_la_SOURCES = examples/cache/cache.c
libcache_la_LIBADD = liblist.la

#
# tests
#
TESTS = tests/basic \
	tests/libc_conflict \
	tests/list_remove_element \
	tests/oldapi
check_PROGRAMS = $(TESTS)

tests_basic_LDADD = liblist.la
tests_libc_conflict_LDADD = liblist.la
tests_list_remove_element_LDADD = liblist.la
tests_oldapi_LDADD = liblist.la

# misc files to throw into the tarball.
# the examples/*test.c files are just interactive prompt-driven
# programs to demonstrate the API.
EXTRA_DIST = examples/Makefile \
	examples/README \
	examples/listtest.c \
	examples/queuetest.c \
	examples/stacktest.c \
	examples/cache/README \
	examples/cache/cachetest.c \
	paper/list.fig \
	paper/paper.tex \
	paper/performance/Makefile \
	paper/performance/allocperf.c \
	paper/performance/stackperf.c \
	paper/performance/travperf.c \
	liblist.pc.in

pkgconfigdir = @libdir@/pkgconfig
pkgconfig_DATA = liblist.pc

doc_DATA =
if DOCS
doc_DATA += paper.dvi list.0
endif

# http://www.gnu.org/software/automake/manual/make/Automatic-Variables.html#Automatic-Variables
paper.dvi: paper/paper.tex list.0 paper.toc
	latex $<

#we need two passes to get the TOC; otherwise, TOC is empty
paper.toc: paper/paper.tex list.0
	latex $<

list.mp: paper/list.fig
	fig2dev -L mp $< $@

list.0: list.mp
	mpost $<
