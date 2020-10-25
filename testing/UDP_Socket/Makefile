CC=cc
CXX=cc

CCFLAGS= -g -std=c99 -Wall

# action
all: talker listener

# Compile all c files to 0 files
%.o : %.c
	$(CC) -c $(CCFLAGS) $<


talker: talker.o 
	$(CC) -o talker talker.o

listener: listener.o
	$(CC) -o listener listener.o

clean:
	rm -f core *.o talker listener