

all: exec

exec: tests.s
	nasm -f elf tests.s -o test.o
	gcc -m32 -Wall -g test.o -o test
	rm test.o

.PHONY: clean
clean:
	rm -rf ./*.o test

