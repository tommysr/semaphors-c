all: p1 p2 p3 main

p1: ./src/p1.c
	gcc -o p1 ./src/p1.c

p2: ./src/p2.c
	gcc -o p2 ./src/p2.c

p3: ./src/p3.c
	gcc -o p3 ./src/p3.c

main: ./src/main.c
	gcc -o main ./src/main.c