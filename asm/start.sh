nasm -f elf64 $1.asm -o $1.o
gcc -no-pie $1.o -o $1
./$1
