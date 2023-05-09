//this is the lib to manage memory
#include "../../include/memlib.h"
#include <stdlib.h>
#include <stdio.h>


Object std_malloc(Object* argv, int argc){
    if(argc != 1){
        printf("ERROR in malloc");
        exit(1);
    }
    if(argv[0].type != Obj_ount_t){
        printf("ERRRIR in malloc");
        exit(1);
    }
    return new_ount(malloc(*argv[0].val.i));
}
Object std_free(Object* argv, int argc){
    if(argc != 1){
        printf("ERROR in malloc");
        exit(1);
    }
    if(argv[0].type != Obj_ount_t){
        printf("ERRRIR in malloc");
        exit(1);
    }
    free(*argv[0].val.i);
    return nil_Obj;
}

Object std_get_val_byte(Object* argv, int argc){
    if(argc != 1){
        printf("ERROR in malloc");
        exit(1);
    }
    if(argv[0].type != Obj_ount_t){
        printf("ERRRIR in malloc");
        exit(1);
    }
    return new_ount(*((unsigned char *)*argv[0].val.i));
}
Object std_set_val_byte(Object* argv, int argc){
    if(argc != 2){
        printf("ERROR in malloc");
        exit(1);
    }
    if(argv[0].type != Obj_ount_t, argv[1].type != Obj_ount_t){
        printf("ERRRIR in malloc");
        exit(1);
    }
    *((unsigned char *)*argv[0].val.i) = *argv[1].val.i;
    return nil_Obj;
}


Object __loader() {
    Object mod = new_Module();

    add_func_Module(mod, "free", &std_free, "");
    add_func_Module(mod, "malloc", &std_malloc, "");
    add_func_Module(mod,"get_byte", &std_get_val_byte,"");
    add_func_Module(mod,"set_byte", &std_set_val_byte,"");

    return mod;
}