# Variables to control the various tools
# CFLAGS go to cc
# LDFLAGS and LDLIBS go to the linker (passed through cc)

CC = gcc

INCLUDEPATH=\
    -Iinclude\

LIBS+= -lm

LDFLAGS+= ${LIBS} -pthread

CFLAGS+= -Wall --std=c99 -D_POSIX_C_SOURCE=199309L ${INCLUDEPATH} 

COMMON_OBJ+= obj/cJSON.o obj/messages.o obj/queue.o obj/util.o

# Targets:

# Do everything by default, if it isn't done already
#all: bin/server bin/client bin/CliServ $(filter-out $(wildcard bin), bin) $(BIN_FILES) 

all: bin/server bin/client

bin/server: obj/server.o ${COMMON_OBJ}
	mkdir -p bin
	${CC} ${DEBUG} -o $@ $^ ${LDFLAGS} 

bin/client: obj/client.o ${COMMON_OBJ}
	mkdir -p bin
	${CC} ${DEBUG} -o $@ $^ ${LDFLAGS}

obj/%.o: src/%.c include/%.h 
	mkdir -p obj
	${CC} -c ${CFLAGS} ${DEBUG} -o $@ $<

clean: 
	if [ -e obj ]; then rm -r obj; fi
purge: clean
	if [ -e bin ]; then rm -r bin; fi
