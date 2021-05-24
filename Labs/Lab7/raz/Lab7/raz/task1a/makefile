
all: exec

exec: main.c helpers.c
	gcc -g -m32 -c -o main.o main.c
				#gcc -g -m32 -c -o units.o units.c
	gcc -g -m32 -c -o helpers.o helpers.c
				#gcc -g -m32 -o hexeditplus main.o helpers.o units.o
	gcc -g -m32 -o hexeditplus main.o helpers.o
	rm main.o helpers.o

.PHONY: clean
clean: 
	rm -rf ./*.o hexeditplus