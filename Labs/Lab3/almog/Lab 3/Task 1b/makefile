all: exec

obs = main.o auxiliary.o

exec: main.c auxiliary.c
	gcc -g -m32 -c -o main.o main.c
	gcc -g -m32 -c -o auxiliary.o auxiliary.c
	gcc -g -m32 -o main $(obs)
	rm $(obs)

.PHONY: clean
clean:
	rm -rf ./*.o main