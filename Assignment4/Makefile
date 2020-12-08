CC=cc
CXX=cc

CCFLAGS= -g -std=c99 -D _POSIX_C_SOURCE=200112L -Wall -Werror

# action
all: UnixLs

# Compile all c files to o files
%.o : %.c
	$(CC) -c $(CCFLAGS) $<


UnixLs: UnixLs.o UnixPrint.o list.o
	$(CC) -o UnixLs UnixLs.o UnixPrint.o list.o

infodemo: infodemo.o
	$(CC) -o infodemo infodemo.o

stat-example: stat-example.o
	$(CC) -o stat-example stat-example.o

readlink-example: readlink-example.o
	$(CC) -o readlink-example readlink-example.o

UnixLs-example: UnixLs-example.o
	$(CC) -o UnixLs-example UnixLs-example.o

getopt-example: getopt-example.o
	$(CC) -o getopt-example getopt-example.o

clean:
	rm -f core *.o UnixLs
	rm -f core *.o infodemo
	rm -f core *.o stat-example
	rm -f core *.o readlink-example
	rm -f core *.o UnixLs-example
	rm -f core *.o getopt-example