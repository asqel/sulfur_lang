//this is the lib to manage memory
#ifndef ONE_FILE
    #include "../../build/API/memlib.h"
#else
    #include "../../include/memlib.h"
#endif
#include <stdlib.h>
#include <stdio.h>

#ifndef ONE_FILE
    Sulfur_ctx context;
#else
    #include "../blt_libs/std.h"
#endif

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
    return new_ount((long int) malloc(argv[0].val.i));
    #else
    return new_ount((long long int) malloc(argv[0].val.i));
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
    free((void *) (long int) argv[0].val.i);
    #else
    free((void *) argv[0].val.i);
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
    return new_ount(*((unsigned char *) (long int) argv[0].val.i));
    #else
    return new_ount(*((unsigned char *) argv[0].val.i));
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
    *((unsigned char *) (long int) argv[0].val.i) = (unsigned char) argv[1].val.i;
    #else
    *((unsigned char *) argv[0].val.i) = (unsigned char) argv[1].val.i;
    #endif

    return nil_Obj;
}

#include <stdio.h>

#ifdef _WIN32
    #include <windows.h>
    #define CLEAR system("cls")
#elif __profanOS__
    #define CLEAR printf("\033[2J")
#else
    #include <stdlib.h>
    #define CLEAR system("clear")
#endif

Object clearConsole(Object *argv, int argc) {
    CLEAR;
    return nil_Obj;
}

#ifndef ONE_FILE

Object __loader(Sulfur_ctx ctx) {
    context = ctx;
    Object mod = new_Module();

    add_func_Module(mod, "free", &std_free, "");
    add_func_Module(mod, "malloc", &std_malloc, "");
    add_func_Module(mod,"get_byte", &std_get_val_byte,"");
    add_func_Module(mod,"set_byte", &std_set_val_byte,"");
    add_func_Module(mod,"clear", &clearConsole,"");

    return mod;
}

#else

Object __load_lilypad(Sulfur_ctx ctx) {
    Object mod = new_Module();
    (void)ctx;

    add_func_Module(mod, "free", &std_free, "");
    add_func_Module(mod, "malloc", &std_malloc, "");
    add_func_Module(mod,"get_byte", &std_get_val_byte,"");
    add_func_Module(mod,"set_byte", &std_set_val_byte,"");
    add_func_Module(mod,"clear", &clearConsole,"");

    return mod;
}

#endif

