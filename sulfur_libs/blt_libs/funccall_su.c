#include "funccall_su.h"
#include "../../include/memlib.h"

#include <stdlib.h>
#include <stdio.h>

Module funccall_module;

char* funccall_methods ="";


Object funccall_call(Object* argv, int argc){
    if(argv[0].val.funcid->is_builtin){
        return (*argv[0].val.funcid->func_p)(&argv[1], argc - 1);
    }
    else{
        printf("ERROR do not use that its not implemented yet(function expression call un sulfur)\n");
        exit(1);
    }
}

Object funccall_help(Object *argv, int argc) {
    if (argc != 1) {
        printf("ERROR funcid:help dont takes arg\n");
        exit(1);
    }
    return new_string(argv[0].val.funcid->description ? argv[0].val.funcid->description : "");
}

Object init_funccall(memory *MEMORY,char*path){
    char* module_name = "funccall";

    Object mod = new_Module();

    mod.val.module->filename = malloc(sizeof(char) * (1 + strlen(module_name)));
    strcpy(mod.val.module->filename, module_name);

    add_func_Module(mod,"call",&funccall_call, "");
    add_func_Module(mod,"help",&funccall_help, "");

    funccall_module = *mod.val.module;
    return mod;

}
