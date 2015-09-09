all: raw jump-tables

raw: raw.o
	ld $< -o $@

raw.o: raw.asm
	nasm -f elf64 $< -o $@


jump-tables: jump-tables.o
	ld $< -o $@

jump-tables.o: jump-tables.asm
	nasm -f elf64 $< -o $@

clean:
	rm -f raw raw.o jump-tables jump-tables.o
