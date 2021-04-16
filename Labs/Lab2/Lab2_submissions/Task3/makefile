all: exec

exec: main.c
	gcc -g -m32 -c -o main.o main.c
	gcc -g -m32 -o menu main.o
	rm main.o

.PHONY: clean
clean: 
	rm -rf ./*.o menu