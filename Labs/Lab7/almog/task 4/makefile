

all: exec

exec: digit_cnt1.s
	nasm -f elf digit_cnt1.s -o digit_cnt1.o
	gcc -m32 -Wall -g digit_cnt1.o -o digit_cnt1

.PHONY: clean
clean:
	rm -rf ./*.o digit_cnt1

