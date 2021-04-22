all:exec

exec:	main.c LineParser.c
	gcc -g -m32 -c -o main.o main.c
	gcc -g -m32 -c -o LineParser.o LineParser.c
	gcc -g -m32 -o main main.o LineParser.o

.PHONY: clean
clean:
	rm -rf ./*.o main