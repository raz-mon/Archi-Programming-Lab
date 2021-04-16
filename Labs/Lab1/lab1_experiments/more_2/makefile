

all: exec


exec: converter.c
	gcc -g -m32 -c -o converter.o converter.c
	gcc -g -m32 -o tar converter.o
	rm converter.o



.PHONY: clean
clean: rm -rf ./*.o tar
