all: test

test_socket.o:
	gcc -c test_socket.c

test: test_socket.o
	gcc -o test test_socket.o

showip.o:
	gcc -c showip.c

testshowip: showip.o
	gcc -o showip showip.o

connect_socket.o:
	gcc -c connect_socket.c

test_connect_socket: connect_socket.o
	gcc -o connect_socket connect_socket.o

clean:
	rm *.o test showip
