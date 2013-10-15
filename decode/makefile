CC=g++
CFLAGS=-g -Wall

DOBJS=encode.o BitStreams.o

# rule for building the executable
default:  encode

encode: $(DOBJS)
	$(CC) -o encode $(DOBJS)

all:      encode

# list of file dependencies
encode.o:           encode.cpp BitStreams.h LinkedBinaryTree.h
BitStreams.o:       BitStreams.h BitStreams.cpp

# the following rule is used to compile .cpp files to .o
.cpp.o:
	$(CC) $(CFLAGS) -c $<


# the following removes all .o files, but leaves the executable
clean:
	rm -f *.o
