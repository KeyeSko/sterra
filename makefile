all: main

main: main.o node.o
	gcc -o main main.o node.o

main.o: main.c
	gcc -o main.o -c main.c

node.o: node.c
	gcc -o node.o -c node.c

clean: 
	rm main main.o node.o
