# AssemblyLanguage
This repository contains all my Assembly language codes for the course CS-201P.

## Instructions to run the assembly codes using nasm 
- To create .o file, set the gdb format and generate the corresponding list file:</br>
```nasm -f elf32 -g  -F stabs fileName.asm  -l fileName.lst```

- Linker command:</br>
```ld  -m  elf_i386 -o fileName fileName.o  io.o ```
- Finally, run the executabe file:</br>
```./fileName```
