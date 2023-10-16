#ifndef SULFUR_H
#define SULFUR_H

#include "memlib.h"

#define VERSION "2.5"
#define SUB_VERSION "0001"
#define COMPLETE_VERSION (VERSION "." SUB_VERSION)

typedef struct {
    int show_mem;
    int show_parse;
    int show_lexe;
    int show_lexe_include;

    int help;
    int version;

    char *filepath;
} sulfur_args_t;

sulfur_args_t *parse_main_args(int argc, char** argv);
int interactive_shell(sulfur_args_t *args);

extern Sulfur_ctx CTX;

#endif
