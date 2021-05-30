all: exec

obs = myELF.o auxiliary.o

exec: myELF.c auxiliary.c
	gcc -g -m32 -c -o myELF.o myELF.c
	gcc -g -m32 -c -o auxiliary.o auxiliary.c
	gcc -g -m32 -o myELF $(obs)
	rm $(obs)

.PHONY: clean
clean:
	rm -rf ./*.o myELF