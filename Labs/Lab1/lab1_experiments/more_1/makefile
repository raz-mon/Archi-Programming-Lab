

all: exec

exec: simple_echo.c
	gcc -g -m32 -c -o simple_echo.o simple_echo.c
	gcc -g -m32 -o tar simple_echo.o
	rm simple_echo.o



#exec: simple_echo.c getAns.c
#	gcc -g -m32 -c -o simple_echo.o simple_echo.c
#	gcc -g -m32 -c -o getAns.o getAns.c
#	gcc -g -m32 -o tar simple_echo.o getAns.o
#	rm simple_echo.o getAns.o

.PHONY: clean
clean:
	rm -rf ./*.o tar
