all: main

main: main.c
	gcc -o main node.c main.c

clean:
	rm main

