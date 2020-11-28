CC=cc
CXX=cc

CCFLAGS= -g -std=c99 -D _POSIX_C_SOURCE=200112L -Wall -Werror

# action
all: UnixLs

# Compile all c files to o files
%.o : %.c
	$(CC) -c $(CCFLAGS) $<


UnixLs: UnixLs.o
	$(CC) -o UnixLs UnixLs.o

clean:
	rm -f core *.o UnixLs