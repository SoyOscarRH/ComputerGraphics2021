
FLAGS = -pedantic -Wall

all: main

main.o : main.c
	gcc -c main.c $(FLAGS)

color.o : color.c
	gcc -c color.c $(FLAGS)

matrix.o : matrix.c
	gcc -c matrix.c $(FLAGS)

fileManagement.o : fileManagement.c
	gcc -c fileManagement.c $(FLAGS)

light.o : light.c
	gcc -c light.c $(FLAGS)

object3D.o : object3D.c
	gcc -c object3D.c $(FLAGS)

point.o : point.c
	gcc -c point.c $(FLAGS)

primitives.o : primitives.c
	gcc -c primitives.c $(FLAGS)

main: main.o color.o matrix.o fileManagement.o light.o object3D.o point.o primitives.o
	gcc main.o color.o matrix.o fileManagement.o light.o object3D.o point.o primitives.o -o main -lm
	# rm -f *.o