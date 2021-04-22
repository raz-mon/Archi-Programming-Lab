
all: exec

exec: lab4_start.s main.c
	nasm -f elf32 lab4_start.s -o lab4_start.o
	gcc -m32 -Wall -ansi -c -nostdlib -fno-stack-protector lab4_util.c -o lab4_util.o
	gcc -m32 -Wall -ansi -c -nostdlib -fno-stack-protector helpers.c -o helpers.o
	gcc -m32 -Wall -ansi -c -nostdlib -fno-stack-protector main.c -o main.o
	ld -m elf_i386 lab4_start.o main.o lab4_util.o helpers.o -o encoder
	rm lab4_util.o main.o lab4_start.o helpers.o

.PHONY: clean
clean: 
	rm -rf ./*.o encoder