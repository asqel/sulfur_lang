#include "funccall_su.h"
#include "../../include/memlib.h"

#include <stdlib.h>
#include <stdio.h>

Module funccall_module;


Object funccall_call(Object* argv, int argc){
    if(argv[0].val.funcid->is_builtin){
        return (*argv[0].val.funcid->func_p)(&argv[1], argc - 1);
    }
    else{
        printf("ERROR do not use that its not implemented yet(function expression call un sulfur)");
        exit(1);
    }
}

memory init_funccall(memory MEMORY,char*path){
    char* module_name = "funccall";

    Object mod = new_Module();

    mod.val.module->filename = malloc(sizeof(char) * (1 + strlen(module_name)));
    strcpy(mod.val.module->filename, module_name);

    add_func_Module(mod,"call",&funccall_call,"");

    funccall_module = *mod.val.module;
    return MEMORY;

}
