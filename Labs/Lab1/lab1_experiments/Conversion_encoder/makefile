


al: exec



exec: converter.c
	#compiling converter...
	gcc -g -m32 -c -o converter.o converter.c
	gcc -g -m32 -o converter converter.o
	rm converter.o

.PHONY: clean
clean:
	rm -rf ./*.o converter