all: exec

exec: hexaPrint.c
	gcc -g -m32 -c -o hexaPrint.o hexaPrint.c
	gcc -g -m32 -o hexa hexaPrint.o

.PHONY: clean
clean: 
	rm -rf ./*.o hexa