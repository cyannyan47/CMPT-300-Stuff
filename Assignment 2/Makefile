CC=cc
CXX=cc

CCFLAGS= -g -std=c99 -D _POSIX_C_SOURCE=200112L -Wall -Werror

# action
all: s-talk

# Compile all c files to o files
%.o : %.c
	$(CC) -c $(CCFLAGS) $<


s-talk: s-talk.o Shutdown_Manager.o List_Manager.o Kb_in.o Scr_out.o UDP_Rx.o UDP_Tx.o list.o 
	$(CC) -o s-talk s-talk.o Shutdown_Manager.o List_Manager.o Kb_in.o Scr_out.o UDP_Rx.o UDP_Tx.o list.o  -lpthread

clean:
	rm -f core *.o s-talk