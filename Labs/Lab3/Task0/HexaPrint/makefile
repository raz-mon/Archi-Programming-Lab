



all: exec

obs = main.o helper.o

exec: main.c
	gcc -g -m32 -c -o main.o main.c
	gcc -g -m32 -c -o helper.o helper.c
	gcc -g -m32 -o hexaPrint $(obs)
	rm $(obs)

.PHONY: clean
clean: 
	rm -rf ./*.o hexaPrint