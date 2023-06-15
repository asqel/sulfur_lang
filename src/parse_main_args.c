#include <string.h>
#include <stdio.h>
#include <stdlib.h>
extern int show_mem;
extern int show_parse;
extern char*filepath;




void parse_main_args(int argc, char** argv){
    if(argc > 4){
        printf("too many arguments receveide %d\n",argc);
        for(int i = 0; i < argc; i++){
            printf("    0:%s\n",argv[i]);
        }
        exit(1);
    }
    for(int i = 1; i < argc; i++){
        if(!strcmp(argv[i], "-m")){
            show_mem = 1;
        }
        if(!strcmp(argv[i], "-p")){
            show_parse = 1;
        }
    }
    if(argc > 1){
        if(strcmp(argv[1], "-m") && strcmp(argv[1], "-p")){
            filepath = argv[1];
        }
    }
}