


all: exec

obs = converter_with_Debug.o converters.o getParams.o


exec: converter_with_Debug.c getParams.c converters.c
	# compiling getParams...
	gcc -g -m32 -c -o getParams.o getParams.c
	# Compiling converters...
	gcc -g -m32 -c -o converters.o converters.c
	# compiling main program (converter_with_Debug)...
	gcc -g -m32 -c -o converter_with_Debug.o converter_with_Debug.c

	# Linking to make executable: CWD - Converter With Debug..
	gcc -g -m32 -o CWD $(obs)
	rm $(obs)



.PHONY: clean
clean:
	rm -rf ./*.o CWD