#ifndef ELFMEMORY_COLIBRI_H
#define ELFMEMORY_COLIBRI_H

#define INPUT_FILE 0
#define INPUT_SOCKET 1
#define INPUT_UNDEFINED 2

typedef struct colibri {
    int input;
    char *file;
    char *host;
    char **argv;
    int port;
} colibri_t;

#define SIZE_OF_ELF 16104

colibri_t *colibri_init(char **argv);
int colibri_execute(colibri_t *injector);


#endif //ELFMEMORY_COLIBRI_H
