


all: exec

obs = mainAssignment0.o asmAssignment0.o

exec: mainAssignment0.c asmAssignment0.s
	gcc -g -m32 -c -o mainAssignment0.o mainAssignment0.c
	nasm -g -f elf -o asmAssignment0.o asmAssignment0.s
	gcc -g -m32 -o tar $(obs)
	rm $(obs)


.PHONY: clean
clean:
	rm -rf ./*.o tar