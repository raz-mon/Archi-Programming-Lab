all: exec

exec: lab3_bubblesort.c
	gcc -g -m32 -c -o lab3_bubblesort.o lab3_bubblesort.c
	gcc -g -m32 -o bubble lab3_bubblesort.o

.PHONY: clean
clean: 
	rm -rf ./*.o bubble