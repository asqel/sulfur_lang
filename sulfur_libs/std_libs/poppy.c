#ifndef ONE_FILE
    #include "../../build/API/memlib.h"
#else
    #include "../../include/memlib.h"
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef ONE_FILE
    Sulfur_ctx context;
#else
    #include "../blt_libs/std.h"
#endif


char *po_read_file(char*path){
    FILE*f=fopen(path,"r");
    if(!f)
        return NULL;
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
    if (!text)
        return nil_Obj;
    Object res = new_string(text);
    free(text);
    return res;
}

Object std_po_write_file(Object *argv, int argc){
    char *path = NULL;
    if (argc != 2){
        printf("poppy::write_file takes only 2 arguments\n");
        exit(1);
    }
    if (argv[0].type != Obj_string_t || argv[1].type != Obj_string_t){
        printf("poppy::write_file takes only string argument\n");
        exit(1);
    }
    path = argv[0].val.s;
    FILE *f = fopen(path, "w");
    if (!f)
        return new_ount(1);
    fwrite(argv[1].val.s, 1, strlen(argv[1].val.s), f);
    return nil_Obj;
}

#ifndef ONE_FILE
Object __loader(Sulfur_ctx ctx) {
    context = ctx;
    Object mod = new_Module();

    add_func_Module(mod, "read_file", &std_po_read_file, "");
    add_func_Module(mod, "write_file", &std_po_write_file, "");

    return mod;
}
#else
Object __load_poppy(Sulfur_ctx ctx) {
    Object mod = new_Module();
    (void)ctx;

    add_func_Module(mod, "read_file", &std_po_read_file, "");
    add_func_Module(mod, "write_file", &std_po_write_file, "");

    return mod;
}
#endif

