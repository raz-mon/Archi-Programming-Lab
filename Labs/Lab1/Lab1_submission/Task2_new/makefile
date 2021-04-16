



all: exec

obs = newmain.o helpers.o

exec: newmain.c helpers.c
	gcc -g -m32 -c -o newmain.o newmain.c
	gcc -g -m32 -c -o helpers.o helpers.c
	gcc -g -m32 -o encoder $(obs)
	rm $(obs)


.PHONY: clean
clean:
	rm -rf ./*.o encoder