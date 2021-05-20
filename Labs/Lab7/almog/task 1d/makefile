all: exec

obs = hexeditplus.o auxiliary.o

exec: hexeditplus.c auxiliary.c
	gcc -g -m32 -c -o hexeditplus.o hexeditplus.c
	gcc -g -m32 -c -o auxiliary.o auxiliary.c
	gcc -g -m32 -o hexeditplus $(obs)
	rm $(obs)

.PHONY: clean
clean:
	rm -rf ./*.o hexeditplus