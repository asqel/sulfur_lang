#ifndef SULFUR_H
#define SULFUR_H

#define VERSION "2.4"

typedef struct {
    int show_mem;
    int show_parse;
    int show_lexe;

    int help;
    int version;

    char *filepath;
} sulfur_args_t;

sulfur_args_t *parse_main_args(int argc, char** argv);
int interactive_shell(sulfur_args_t *args);

#endif
