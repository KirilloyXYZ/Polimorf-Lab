all: vector_lab

vector_lab: main.o vector.o real.o complex.o
	gcc main.o vector.o real.o complex.o -o vector_lab

main.o: main.c vector.h real.h complex.h algebra.h
	gcc -c main.c

vector.o: vector.c vector.h algebra.h
	gcc -c vector.c

real.o: real.c real.h algebra.h
	gcc -c real.c

complex.o: complex.c complex.h algebra.h
	gcc -c complex.c

tests: tests.o vector.o real.o complex.o
	gcc tests.o vector.o real.o complex.o -o tests

tests.o: tests.c vector.h real.h complex.h algebra.h
	gcc -c tests.c

test: tests
	./tests

clean:
	del /Q *.o 2>NUL
	del /Q vector_lab.exe tests.exe 2>NUL