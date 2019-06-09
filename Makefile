all: build

build: clean fort.o
	ld -o bin/fort bin/fort.o

fort.o: fort.asm bin_dir
	nasm -f elf64 -g -F dwarf fort.asm -o bin/fort.o

bin_dir: 
	mkdir -p bin 

clean:
	rm -rf bin

run: build
	cat branches.fort - | ./fort
