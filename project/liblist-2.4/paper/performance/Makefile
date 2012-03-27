# Makefile for list(3) performance test programs
#
# Bradley C. SPpatz, University of Florida, bcs@ufl.edu
# Last edited: Wed Sep 18 23:39:51 1991 by bcs (Bradley C. Spatz) on cutback
#
CC	= gcc -fstrength-reduce -finline-functions

all:		
		@echo "Please specify one of:"
		@echo "	stackperf, macroperf, travperf, allocperf"

stackperf:	stackperf.o
		$(CC) $(CFLAGS) -o stackperf stackperf.o -llist

macroperf:	macroperf.o
		$(CC) $(CFLAGS) -o macroperf macroperf.o -llist

travperf:	travperf.o
		$(CC) $(CFLAGS) -o travperf travperf.o -llist

allocperf:	allocperf.o
		$(CC) $(CFLAGS) -o allocperf allocperf.o -llist

clean:
		rm -f stackperf macroperf travperf *.o core
