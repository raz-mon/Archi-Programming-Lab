



all: exec


exec: main.c
#	gcc -g -m32 -c -o LineParser.o LineParser.c
#	gcc -g -m32 -c -o helpers.o helpers.c
#	gcc -g -m32 -c -o looper.o looper.c
	gcc -g -m32 -c -o main.o main.c
	gcc -g -m32 -o tar main.o
	rm main.o

.PHONY: clean
clean: 
	rm -rf ./*.o tar



