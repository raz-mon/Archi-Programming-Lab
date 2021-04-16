



all: exec

obs = main.o helpers.o

exec: main.c helpers.c
	gcc -g -m32 -c -o main.o main.c
	gcc -g -m32 -c -o helpers.o helpers.c
	gcc -g -m32 -o encoder $(obs)
	rm $(obs)


.PHONY: clean
clean:
	rm -rf ./*.o encoder