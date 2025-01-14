.PHONY: all clean run build

all: checker

build: checker

run: checker
	./checker

labyrinth.o: labyrinth.asm
	nasm -f elf $^ -o $@

check_labyrinth.o: check_labyrinth.c
	gcc -c -g -m32 -no-pie $^ -o $@

checker: check_labyrinth.o  labyrinth.o
	gcc -m32 -g -no-pie $^ -o $@
	rm *.o

clean:
	rm -f checker
	rm -f output/labyrinth_*