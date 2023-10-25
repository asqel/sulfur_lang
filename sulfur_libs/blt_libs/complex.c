#include "complex.h"
#include "../../include/memlib.h"
#include "std.h"

#include <stdlib.h>
#include <stdio.h>

Module complex_module;

char *complex_methods = "";

Object complex_re(Object *argv, int argc) {
    return new_ount(argv[0].val.c[0]);
}

Object complex_im(Object *argv, int argc) {
    return new_ount(argv[0].val.c[1]);
}


Object init_complex(memory *MEMORY,char*path){
    char* module_name = "complex";

    Object mod = new_Module();

    mod.val.module->filename = malloc(sizeof(char) * (1 + strlen(module_name)));
    strcpy(mod.val.module->filename, module_name);


    add_func_Module(mod, "re", &complex_re, "");
    add_func_Module(mod, "im", &complex_im, "");
    add_func_Module(mod, "__call", &std_comp, "");

    complex_module = *mod.val.module;
    return mod;

}
