


all: exec

exec: lab4_start.s lab4_main.c
	nasm -f elf32 lab4_start.s -o lab4_start.o
	gcc -m32 -Wall -ansi -c -nostdlib -fno-stack-protector lab4_util.c -o lab4_util.o
	gcc -m32 -Wall -ansi -c -nostdlib -fno-stack-protector lab4_main.c -o lab4_main.o
	ld -m elf_i386 lab4_start.o lab4_main.o lab4_util.o -o task0
	rm lab4_util.o lab4_main.o lab4_start.o

.PHONY: clean
clean: rm -rf ./*.o task0