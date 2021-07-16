#include <stdio.h>
#include <link.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <linux/memfd.h>
#include <string.h>
#include <stdlib.h>


extern char **environ;

void load_elf(char *elf, char **argv) {

    int load_size = 0, write_bytes = 0;

    ElfW(Ehdr) *ehdr = (ElfW(Ehdr) *) elf;
    ElfW(Phdr) *phdr = (ElfW(Phdr) *)(elf + ehdr->e_phoff);

    printf("[+] Loading ELF file...\n");
    printf("[+] ELF Magic : '%2s'\n", ehdr->e_ident);

    switch (ehdr->e_type) {
        case 3:
            printf("[+] ELF type : (3) 'EXEC'\n");
            break;
    }
    printf("[+] Program headers : '%d'\n", ehdr->e_phnum);
    for (int x = 0; x < ehdr->e_phnum; x++, phdr++) {
        if (phdr->p_type == PT_LOAD) {
            printf("[+] PT_LOAD segment at : 0x%08lx\n", phdr->p_offset);
            load_size += (phdr->p_offset + phdr->p_filesz);
        }
    }

    printf("[+] Total PT_LOAD segment size '%d'\n", load_size);

    int out = syscall(SYS_memfd_create, "", MFD_CLOEXEC);
    ftruncate(out, load_size);

    while (write_bytes < load_size) {
        int w = write(out, elf + write_bytes, load_size - write_bytes);
        if (w == -1) {
            printf("Failed to write memory file\n");
            exit(1);
        }
        write_bytes += w;
    }

    syscall(SYS_execveat, out, "", argv, environ, 0x1000);
    return;
}