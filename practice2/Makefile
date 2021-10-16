all: main

main.o : main.c
	gcc -c main.c -pedantic -Wall
point.o : point.c 
	gcc -c point.c -pedantic -Wall
render.o : render.c
	gcc -c render.c -pedantic -Wall
fileManagement.o: fileManagement.c
	gcc -c fileManagement.c -pedantic -Wall
primitives.o: primitives.c
	gcc -c primitives.c -pedantic -Wall
threadPool.o: threadPool.c
	gcc -c threadPool.c -pedantic -Wall

main: main.o render.o point.o fileManagement.o primitives.o threadPool.o
	gcc main.o render.o point.o fileManagement.o primitives.o threadPool.o -o main -lm -lpthread
	rm -f *.o