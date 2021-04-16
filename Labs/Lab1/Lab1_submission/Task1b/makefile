


all: exec

obs = main.o converters.o getParams.o


exec: main.c getParams.c converters.c
	# compiling getParams...
	gcc -g -m32 -c -o getParams.o getParams.c
	# Compiling converters...
	gcc -g -m32 -c -o converters.o converters.c
	# compiling main program (converter_with_Debug)...
	gcc -g -m32 -c -o main.o main.c

	# Linking to make executable: CWD - Converter With Debug..
	gcc -g -m32 -o encoder $(obs)
	rm $(obs)



.PHONY: clean
clean:
	rm -rf ./*.o encoder