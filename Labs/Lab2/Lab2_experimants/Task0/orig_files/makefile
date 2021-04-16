

all: exec


exec: count-words_orig.c
	gcc -g -m32 -c -o count-words_orig.o count-words_orig.c
	gcc -g -m32 -o tar count-words_orig.o
	rm count-words_orig.o

.PHONY: clean
clean: 
	rm -rf ./*.o tar
