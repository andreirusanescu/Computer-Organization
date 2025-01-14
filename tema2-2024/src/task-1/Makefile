.PHONY: all clean run build

all: checker

build: checker

run: checker
	./checker

check_permission.o: check_permission.asm
	nasm -f elf $^ -o $@

checker.o: checker.c
	gcc -c -g -m32 -no-pie $^ -o $@

checker: checker.o  check_permission.o
	gcc -m32 -g -no-pie $^ -o $@
	rm *.o

clean:
	rm  checker
	rm  output/permission_*
