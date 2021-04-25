

all: exec


exec: looper.c LineParser.c
	gcc -g -m32 -c -o looper.o looper.c
	gcc -g -m32 -c -o LineParser.o LineParser.c
	gcc -g -m32 -o tar LineParser.o looper.o
	rm looper.o LineParser.o

.PHONY: clean
clean: 
	rm -rf ./*.o tar