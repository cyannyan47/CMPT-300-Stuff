CC=cc
CXX=cc

CCFLAGS= -g -std=c99 -D _POSIX_C_SOURCE=200112L -Wall -Werror

# action
all: simOS

# Compile all c files to o files
%.o : %.c
	$(CC) -c $(CCFLAGS) $<


simOS: simOS.o CLIManager.o PCB.o ReadyQueues.o RecvWaitingQueue.o Semaphore.o SendWaitingQueue.o list.o 
	$(CC) -o simOS simOS.o CLIManager.o PCB.o ReadyQueues.o RecvWaitingQueue.o Semaphore.o SendWaitingQueue.o list.o  -lpthread

clean:
	rm -f core *.o simOS