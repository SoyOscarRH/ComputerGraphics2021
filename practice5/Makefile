FLAGS = -pedantic -Wall -g

all: main

main.o : main.c
	gcc -c main.c $(FLAGS)

fileManagement.o : fileManagement.c
	gcc -c fileManagement.c $(FLAGS)

matrix.o : matrix.c
	gcc -c matrix.c $(FLAGS)

parametric.o : parametric.c
	gcc -c parametric.c $(FLAGS)

point.o : point.c
	gcc -c point.c $(FLAGS)

primitives.o : primitives.c
	gcc -c primitives.c $(FLAGS)

main: main.o fileManagement.o matrix.c parametric.o point.o primitives.o
	gcc main.o fileManagement.o matrix.c parametric.o point.o primitives.o -o main -lm
	rm -f *.o
