nasm -f elf32 src/implements/x86_64/boot/main.asm -o boot.o
ld -m elf_i386 -T targets/x86_64/linker.ld -o kernel.bin boot.o
qemu-system-i386 -kernel kernel.bin