CC=cc
CXX=cc

CCFLAGS= -g -std=c99 -Wall -Werror

# action
all: mainTest test

# Compile all c files to 0 files
%.o : %.c
	$(CC) -c $(CCFLAGS) $<


mainTest: mainTest.o list.o
	$(CC) -o mainTest mainTest.o list.o

test: mainTest
	./mainTest

clean:
	rm -f core *.o mainTest list