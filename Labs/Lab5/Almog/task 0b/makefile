all:exec

exec:	looper.c
	gcc -g -m32 -c -o looper.o looper.c
	gcc -g -m32 -o looper looper.o

.PHONY: clean
clean:
	rm -rf ./*.o looper