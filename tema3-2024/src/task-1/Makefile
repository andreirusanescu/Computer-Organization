build: paranthesinator.o main.o
	gcc -g -m32 -o paranthesinator $^

paranthesinator.o: paranthesinator.asm
	nasm -g -f elf32 -o $@ $<

main.o: main.c
	gcc -g -m32 -c -o $@ $<

clean:
	rm -f *.o paranthesinator out/*.out
