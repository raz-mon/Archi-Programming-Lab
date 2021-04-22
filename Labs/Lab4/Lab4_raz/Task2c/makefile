
all: exec

exec: start.s helpers.c main.c
	nasm -f elf32 start.s -o start.o
	gcc -m32 -Wall -ansi -c -nostdlib -fno-stack-protector lab4_util.c -o lab4_util.o
	gcc -m32 -Wall -ansi -c -nostdlib -fno-stack-protector helpers.c -o helpers.o
	gcc -m32 -Wall -ansi -c -nostdlib -fno-stack-protector main.c -o main.o
	ld -m elf_i386 start.o main.o lab4_util.o helpers.o -o flame2
	rm lab4_util.o main.o start.o helpers.o

.PHONY: clean
clean: 
	rm -rf ./*.o flame2