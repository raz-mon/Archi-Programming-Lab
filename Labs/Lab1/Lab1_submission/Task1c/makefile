


all: exec

obs = main.o getParams.o encryptors.o


exec: main.c getParams.c encryptors.c
	# compiling getParams...
	gcc -g -m32 -c -o getParams.o getParams.c
	# compiling main program (converter_with_Debug)...
	gcc -g -m32 -c -o main.o main.c
	# compiling encryptos...
	gcc -g -m32 -c -o encryptors.o encryptors.c
	# Linking to make executable: CWD - Converter With Debug..
	gcc -g -m32 -o 1c $(obs)
	rm $(obs)



.PHONY: clean
clean:
	rm -rf ./*.o CWD