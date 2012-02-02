# Variables to control the various tools
# CFLAGS go to cc
# LDFLAGS and LDLIBS go to the linker (passed through cc)

CC = gcc

INCLUDEPATH=\
    -Iinclude\

LIBS+= -lpthread -lm

LDFLAGS+= ${LIBS}

CFLAGS+= -Wall -std=c99 -D_POSIX_C_SOURCE ${INCLUDEPATH}

COMMON_OBJ+= obj/cJSON.o obj/messages.o obj/queue.o 

# Targets:

# Do everything by default, if it isn't done already
#all: bin/server bin/client bin/CliServ $(filter-out $(wildcard bin), bin) $(BIN_FILES) 

all: bin/CliServ

#bin/server server.o ${COMMON_OBJ}
#	${CPP} ${LIBFLA$} ${DEBUG} -o $@ $^ 

#bin/client client.o ${COMMON_OBJ}
#	${CPP} ${LIBFLA$} ${DEBUG} -o $@ $^ 

bin/CliServ: obj/CliServ.o ${COMMON_OBJ}
	mkdir -p bin
	${CC} ${LDFLAGS$} ${DEBUG} -o $@ $^

obj/%.o: src/%.c include/%.h 
	mkdir -p obj
	${CC} -c ${CFLAGS} ${DEBUG} -o $@ $<

clean: 
	if [ -e obj ]; then rm -r obj; fi
purge: clean
	if [ -e bin ]; then rm -r bin; fi

