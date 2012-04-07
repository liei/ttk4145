
JAVA_HOME = $(shell readlink -f $(shell which javac) | sed "s:bin/javac::")

INCLUDES = -I. -I$(JAVA_HOME)include

# Compiler flags go here.
CFLAGS = -std=c99 -g -fPIC -Wall $(INCLUDES) -MMD

# Linker flags go here.
LDFLAGS = -shared

# Libraries
LIBS = -lpthread -lcomedi -lm

# list of sources
SOURCES = $(shell find -name "*.c")

# Program executable file name.
TARGET = libdriver1.so

# Default rule, to compile everything.
all: $(TARGET)


# -- The below lines should not need to be changed --

# Define dependencies
DEPS = $(shell find -name "*.d")

# Define all object files.
OBJECTS = $(SOURCES:.c=.o)

# rule to link the program
$(TARGET): $(OBJECTS)
	gcc $(LDFLAGS) -o $@ $^ $(LIBS) 
	cp $@ ..

# Compile: create object files from C source files.
%.o : %.c
	gcc $(CFLAGS) -c -o $@ $< 

# Include dependencies, if any.
-include $(DEPS)

# rule for cleaning re-compilable files.
clean:
	rm -f $(OBJECTS) $(DEPS)

purge: clean
	rm -f $(TARGET)

rebuild: clean all

.PHONY: all rebuild clean

