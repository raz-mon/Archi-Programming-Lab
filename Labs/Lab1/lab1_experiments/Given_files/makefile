all: exec

libs: asm-lib c-lib

obs = main.o add.o numbers.o test.o

asm-lib: add.s	
	# flags meanings:
	# -g = debug symbolx
	# -f elf = output format (ELF 32 bit)
	# -o add.o = write assembled file to add.o
	nasm -g -f elf -o add.o add.s

c-lib: numbers.o test.o
	# flags meanings:
	# -g = debug symbols
	# -m32 = compile to x86_32 architecture (32bit)
	# -c = Don't link (i.e., create an object file, not an executable)
	# -o numbers.o = output compiled file to numbers.o
#	gcc -g -m32 -c -o numbers.o numbers.c
#	gcc -g -m32 -c -o test.o test.c

test.o: test.c
	# compiling test!!!@#$
	gcc -g -m32 -c -o test.o test.c

numbers.o: numbers.c
	# compiling numbers!!!@#$
	gcc -g -m32 -c -o numbers.o numbers.c

#c-lib: numbers.c test.c
	# flags meanings:
	# -g = debug symbols
	# -m32 = compile to x86_32 architecture (32bit)
	# -c = Don't link (i.e., create an object file, not an executable)
	# -o numbers.o = output compiled file to numbers.o
#	gcc -g -m32 -c -o numbers.o numbers.c
#	gcc -g -m32 -c -o test.o test.c



exec: main.c libs
	gcc -g -m32 -c -o main.o main.c
	gcc -g -m32  $(obs) -o main
	rm $(obs)

.PHONY: clean
clean:
	rm -rf ./*.o main
