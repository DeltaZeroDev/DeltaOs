nasm -f elf32 src/implements/x86_64/boot/main.asm -o boot.o
gcc -mgeneral-regs-only -Wno-stack-protector -g -m32 -c src/implements/kernel/kernel.c -o kernel.o
ld -m elf_i386 -T targets/x86_64/linker.ld -o kernel.bin boot.o kernel.o
qemu-system-i386 -kernel kernel.bin