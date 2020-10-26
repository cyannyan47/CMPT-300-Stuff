CC=cc
CXX=cc

CCFLAGS= -g -std=c99 -D _POSIX_C_200809L -Wall

# action
all: s-talk run

# Compile all c files to 0 files
%.o : %.c
	$(CC) -c $(CCFLAGS) $<


s-talk: s-talk.o
	$(CC) -o s-talk s-talk.o

run: s-talk
	./s-talk

clean:
	rm -f core *.o s-talk