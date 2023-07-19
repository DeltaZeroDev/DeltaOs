rm DeltaOs.iso iso/boot/kernel.bin
cd ..
cd ..
nasm -f elf32 src/implements/x86_64/boot/main.asm -o build/boot.o
gcc -mgeneral-regs-only -Wno-stack-protector -g -m32 -c src/implements/kernel/kernel.c -o build/kernel.o
ld -m elf_i386 -T targets/x86_64/linker.ld -o build/kernel.bin build/boot.o build/kernel.o
cp build/kernel.bin targets/x86_64/iso/boot/kernel.bin
cd targets/x86_64
grub-mkrescue -o DeltaOs.iso iso/
# qemu-system-i386 -vga std -cdrom DeltaOs.iso