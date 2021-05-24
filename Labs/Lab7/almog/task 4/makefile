all: exec

exec: digit_cnt.c
	gcc -m32 -g -c -fno-pie -o digit_cnt.o digit_cnt.c
	gcc -m32 -g -o digit_cnt digit_cnt.o 

.PHONY: clean
clean:
	rm -rf ./*.o digit_cnt

