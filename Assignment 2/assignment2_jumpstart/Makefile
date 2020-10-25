all: test_read_write server client

test_read_write: test_read_write.o
	gcc -o test_read_write test_read_write.o

test_read_write.o:
	gcc -c test_read_write.c

server: UDPServer.o
	gcc -o server UDPServer.o

UDPServer.o:
	gcc -c UDPServer.c

client: UDPClient.o
	gcc -o client UDPClient.o

UDPClient.o:
	gcc -c UDPClient.c

clean:
	rm *.o test_read_write server client
