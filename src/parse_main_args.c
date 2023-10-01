#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "../include/sulfur.h"

//   -m, --show-mem      show memory after execution
//   -p, --show-parse    show parse tree
//   -l, --show-lexe     show tokens after lexing
//   -h, --help          show this help message and exit
//   -v, --version       show program's version number and exit

// If no file is given, sulfur will run in interactive mode.
// If file is given, sulfur will execute it and exit.

sulfur_args_t *parse_main_args(int argc, char **argv) {
    sulfur_args_t *args = malloc(sizeof(sulfur_args_t));
    args->show_mem   = 0;
    args->show_parse = 0;
    args->show_lexe  = 0;
    args->show_lexe_include  = 0;

    args->help       = 0;
    args->version    = 0;

    args->filepath   = NULL;

    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            if (argv[i][1] == '-') {
                if (strcmp(argv[i] + 2, "show-mem") == 0) {
                    args->show_mem = 1;
                } else if (strcmp(argv[i] + 2, "show-parse") == 0) {
                    args->show_parse = 1;
                } else if (strcmp(argv[i] + 2, "show-lexe") == 0) {
                    args->show_lexe = 1;
                } else if (strcmp(argv[i] + 2, "show-lexe-include") == 0) {
                    args->show_lexe_include = 1;
                } else if (strcmp(argv[i] + 2, "help") == 0) {
                    args->help = 2;
                } else if (strcmp(argv[i] + 2, "version") == 0) {
                    args->version = 1;
                } else {
                    printf("Unknown option: %s\n", argv[i]);
                    args->help = 1;
                }
            } else {
                for (int j = 1; argv[i][j]; j++) {
                    switch (argv[i][j]) {
                        case 'm':
                            args->show_mem = 1;
                            break;
                        case 'p':
                            args->show_parse = 1;
                            break;
                        case 'l':
                            args->show_lexe = 1;
                            break;
                        case 'i':
                            args->show_lexe_include = 1;
                            break;
                        case 'h':
                            args->help = 2;
                            break;
                        case 'v':
                            args->version = 1;
                            break;
                        default:
                            printf("Unknown option: %c\n", argv[i][j]);
                            args->help = 1;
                    }
                }
            }
        } else {
            if (args->filepath) {
                printf("Too many arguments\n");
                args->help = 1;
            } else {
                args->filepath = argv[i];
            }
        }
    }
    return args;
}
