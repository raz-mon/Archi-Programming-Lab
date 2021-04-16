

all: exec


exec: count-words.c
	gcc -g -m32 -c -o count-words.o count-words.c
	gcc -g -m32 -o tar count-words.o
	rm count-words.o

.PHONY: clean
clean: 
	rm -rf ./*.o tar
