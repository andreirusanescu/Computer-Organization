build: main.o functional.o
	gcc -g -m64 -no-pie -fno-PIC -o functional main.o functional.o

main.o: main.c
	gcc -g -no-pie -fno-PIC -c -m64 -o $@ $<

functional.o: functional.asm
	nasm -g -f elf64 -o $@ $<


clean:
	rm -f *.o functional out/*.out
