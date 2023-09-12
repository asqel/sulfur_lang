//this is the lib to manage memory
#include "../../include/memlib.h"
#include <stdlib.h>
#include <stdio.h>


Object std_malloc(Object* argv, int argc){
    if(argc != 1){
        printf("ERROR in malloc\n");
        exit(1);
    }
    if(argv[0].type != Obj_ount_t){
        printf("ERRRIR in malloc\n");
        exit(1);
    }
    #ifdef __profanOS__
    // 32 bit system
    return new_ount((long int) malloc(*argv[0].val.i));
    #else
    return new_ount((long long int) malloc(*argv[0].val.i));
    #endif
}

Object std_free(Object* argv, int argc){
    if(argc != 1){
        printf("ERROR in malloc\n");
        exit(1);
    }
    if(argv[0].type != Obj_ount_t){
        printf("ERRRIR in malloc\n");
        exit(1);
    }
    #ifdef __profanOS__
    // 32 bit system
    free((void *) (long int) *argv[0].val.i);
    #else
    free((void *) *argv[0].val.i);
    #endif

    return nil_Obj;
}

Object std_get_val_byte(Object* argv, int argc){
    if(argc != 1){
        printf("ERROR in malloc\n");
        exit(1);
    }
    if(argv[0].type != Obj_ount_t){
        printf("ERRRIR in malloc\n");
        exit(1);
    }

    #ifdef __profanOS__
    // 32 bit system
    return new_ount(*((unsigned char *) (long int) *argv[0].val.i));
    #else
    return new_ount(*((unsigned char *) *argv[0].val.i));
    #endif
}

Object std_set_val_byte(Object* argv, int argc){
    if(argc != 2){
        printf("ERROR in malloc\n");
        exit(1);
    }
    if(argv[0].type != Obj_ount_t, argv[1].type != Obj_ount_t){
        printf("ERRRIR in malloc\n");
        exit(1);
    }

    #ifdef __profanOS__
    // 32 bit system
    *((unsigned char *) (long int) *argv[0].val.i) = (unsigned char) *argv[1].val.i;
    #else
    *((unsigned char *) *argv[0].val.i) = (unsigned char) *argv[1].val.i;
    #endif

    return nil_Obj;
}

#ifndef ONE_FILE

Object __loader() {
    Object mod = new_Module();

    add_func_Module(mod, "free", &std_free, "");
    add_func_Module(mod, "malloc", &std_malloc, "");
    add_func_Module(mod,"get_byte", &std_get_val_byte,"");
    add_func_Module(mod,"set_byte", &std_set_val_byte,"");

    return mod;
}

#else

Object __load_lilypad() {
    Object mod = new_Module();

    add_func_Module(mod, "free", &std_free, "");
    add_func_Module(mod, "malloc", &std_malloc, "");
    add_func_Module(mod,"get_byte", &std_get_val_byte,"");
    add_func_Module(mod,"set_byte", &std_set_val_byte,"");

    return mod;
}

#endif
