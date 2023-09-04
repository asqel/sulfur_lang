#include <string.h>
#include <stdio.h>
#include <stdlib.h>

extern int show_mem;
extern int show_parse;
extern int show_lexe;
extern char*filepath;

int parse_main_args(int argc, char** argv){
    for (int i = 1; i < argc; i++){
        if (argv[i][0] == '-'){
            if (argv[i][1] == 'm'){
                show_mem = 1;
            } else if (argv[i][1] == 'p'){
                show_parse = 1;
            }else if (argv[i][1] == 'l'){
                show_lexe = 1;
            } else {
                printf("Unknown flag: %s\n", argv[i]);
                return 1;
            }
        } else {
            if (filepath){
                printf("Multiple filepaths given\n");
                return 1;
            }
            filepath = argv[i];
        }
    }
    return 0;
}
