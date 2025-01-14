.PHONY: build all run clean

# identify source files & generate object targets
OBJECTS  = $(patsubst %.c,   bin/%.o, $(wildcard *.c))
OBJECTS += $(patsubst %.asm, bin/%.o, $(wildcard *.asm))

# prevent deletion of intermediary files / directories
.SECONDARY:

# defualt target
build, all: checker output/

# checker generation target
checker: $(OBJECTS)
	gcc -no-pie -m32 -g -o $@ $^

# obj target based on C source
bin/%.o: %.c | bin/
	gcc -c -m32 -g -o $@ $<

# obj target based on asm source
bin/%.o: %.asm | bin/
	nasm -f elf32 -o $@ $<

# ephemeral directory generation
%/:
	@mkdir -p $@

# used by top level checker to run all tests
# NOTE: checker output should be ignored
#       calculate score based on diff *{.out,.ref}
run: checker output/
	./checker

# cleanup target
clean:
	@rm -rf bin/ output/ checker

