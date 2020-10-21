all: test

test_read_write.o:
	gcc -c test_read_write.c

test: test_read_write.o
	gcc -o test test_read_write.o

clean:
	rm *.o test
