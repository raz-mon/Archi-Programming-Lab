


all: exec

obs = encryption.o getParams.o encryptors.o


exec: encryption.c getParams.c encryptors.c
	# compiling getParams...
	gcc -g -m32 -c -o getParams.o getParams.c
	# compiling main program (converter_with_Debug)...
	gcc -g -m32 -c -o encryption.o encryption.c
	# compiling encryptos...
	gcc -g -m32 -c -o encryptors.o encryptors.c
	# Linking to make executable: CWD - Converter With Debug..
	gcc -g -m32 -o 1c $(obs)
	rm $(obs)



.PHONY: clean
clean:
	rm -rf ./*.o CWD