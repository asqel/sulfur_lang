#include "../../build/API/memlib.h"
#include <stdio.h>
#include <stdlib.h>

#ifndef ONE_FILE
    Sulfur_ctx context;
#endif


char*po_read_file(char*path){
    FILE*f=fopen(path,"r");
    char*text=malloc(sizeof(char));
    int n=1;
    char c=fgetc(f);
    text[n-1]=c;
    while(c!=EOF){
        n++;
        text=realloc(text,sizeof(char)*n);
        c=fgetc(f);
        text[n-1]=c;
    }
    text[n-1]='\0';
    fclose(f);
    return text;
}

Object std_po_read_file(Object* argv, int argc){
    if(argc != 1){
        printf("ERROR po_read_file only takes 1 arg\n");
        exit(1);
    }
    if(argv[0].type != Obj_string_t){
        printf("ERROR po_read_file only takes a string\n");
        exit(1);
    }
    char * text = po_read_file(argv[0].val.s);
    Object res = new_string(text);
    free(text);
    return res;
}

#ifndef ONE_FILE
Object __loader(Sulfur_ctx ctx) {
    Object mod = new_Module();
    context = ctx;

    add_func_Module(mod, "po_read_file", &std_po_read_file, "");

    return mod;
}
#else
Object __load_poppy(Sulfur_ctx ctx) {
    Object mod = new_Module();
    (void)ctx;

    add_func_Module(mod, "po_read_file", &std_po_read_file, "");

    return mod;
}
#endif

