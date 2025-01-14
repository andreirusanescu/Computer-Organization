CC=gcc
ASC=nasm
ASFLAGS=-F dwarf -f elf32
CFLAGS=-c -g -m32 -fno-pic -masm=intel
LINKFLAGS=-m32 -no-pie
INCPATHS=include
CSRCS=src/check.c src/list.c
ASSRCS=task_3.asm
TESTSDIR= tests

OBJECTS=$(CSRCS:.c=.o) $(ASSRCS:.asm=.o)
INCFLAGS=$(foreach TMP,$(INCPATHS),-I$(TMP))
OUTFOLDERS=$(foreach TMP, $(wildcard $(TESTSDIR)/*), $(TMP))
OUTS=$(addsuffix /dfs.out, $(OUTFOLDERS))

build all: checker

checker: $(OBJECTS)
	$(CC) $(LINKFLAGS) $^ -o $@

%.o: %.asm
	$(ASC) $(ASFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(INCFLAGS) $(CFLAGS) $^ -o $@

.PHONY: clean
clean:
	rm -rf checker $(OBJECTS) $(OUTS)
