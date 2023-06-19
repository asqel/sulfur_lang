#include "../../include/memlib.h"
#include "list_su.h"
#include <stdlib.h>
Module list_module;
char* list_methods = "append";

Object append_list(Object*obj,int n_arg){
    int len = *(obj[0].val.li->elements[0].val.i);
    obj[0].val.li->elements = realloc(obj->val.li->elements,(len+2)*sizeof(Object));
    obj[0].val.li->elements[len+1] = Obj_cpy(obj[1]);
    (*obj[0].val.li->elements[0].val.i)++;
    return obj[0];
}


memory init_list(memory MEMORY,char*path){
    char* module_name = "list";

    Object mod = new_Module();

    mod.val.module->filename = malloc(sizeof(char) * (1 + strlen(module_name)));
    strcpy(mod.val.module->filename, module_name);

    add_func_Module(mod, "append", &append_list, "");

    list_module = *mod.val.module;
    return MEMORY;

}