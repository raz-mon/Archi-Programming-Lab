


all: exec

obs = main.o helpers.o

exec: main.c
	gcc -g -m32 -c -o helpers.o helpers.c
	gcc -g -m32 -c -o main.o main.c
	gcc -g -m32 -o virusDetector $(obs)
	rm $(obs)

.PHONY: clean
clean: 
	rm -rf ./*.o virusDetector