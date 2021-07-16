#include <string.h>
#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>

#include "colibri.h"

static void usage(const char *name)
{
    printf("Usage: %s [options]\n", name);
    printf("  -s, --source           input source (binary, socket)\n");
    printf("  -f, --file             'file' for 'binary' input only\n");
    printf("  -h, --host             'host' for 'socket' input only\n");
    printf("  -p, --port             'port' for 'socket' input only\n");
    printf("\n");
    exit(1);
}


int main(int argc, char **argv) {

    char *program = argv[0];

    if (argc < 2) {
        usage(program);
    }

    int *index = 0;
    int opt;
    colibri_t *colibri = colibri_init(argv);

    struct option option[] = {
            {"source", required_argument, NULL, 's'},
            {"host",   optional_argument, NULL, 'h'},
            {"port",   optional_argument, NULL, 'p'},
            {"file",   optional_argument, NULL, 'f'},
            { 0, 0, NULL, 0 }
    };


    const char *short_option = "s:h:p:f:";

    while ((opt = getopt_long(argc, argv, short_option, option, index)) != -1) {
        switch (opt) {
            case 's':
                if (strcmp("socket", optarg) == 0) {
                    colibri->input = INPUT_SOCKET;
                } else if (strcmp("binary", optarg) == 0) {
                    colibri->input = INPUT_FILE;
                } else {
                    colibri->input = INPUT_UNDEFINED;
                }
                break;
            case 'f':
                if (colibri->input == INPUT_FILE) {
                    colibri->file = optarg;
                } else {
                    printf("'file' argument as not required for this input type.\n");
                }
                break;
            case 'h':
                colibri->host = optarg;
                break;
            case 'p':
                colibri->port = atoi(optarg);
                break;
            default:
                usage(program);

        }
    }


    colibri_execute(colibri);
    return 0;
}