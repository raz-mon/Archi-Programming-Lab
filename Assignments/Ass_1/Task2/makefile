



all: exec


exec: main_task2Assignment1.c asm_task2Assignment1.s
	nasm -g -f elf -o asm_task2Assignment1.o asm_task2Assignment1.s
	gcc -g -m32 -c -o main_task2Assignment1.o main_task2Assignment1.c
	gcc -g -m32 -o tar main_task2Assignment1.o asm_task2Assignment1.o
	rm main_task2Assignment1.o asm_task2Assignment1.o

.PHONY: clean
clean: 
	rm -rf ./*.o tar

