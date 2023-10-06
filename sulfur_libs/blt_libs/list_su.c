#include "../../include/memlib.h"
#include "list_su.h"
#include <stdlib.h>
Module list_module;
char* list_methods = "append";

Object append_list(Object*obj,int n_arg){
    int len = obj[0].val.li->elements[0].val.i;
    obj[0].val.li->elements = realloc(obj->val.li->elements,(len+2)*sizeof(Object));
    obj[0].val.li->elements[len+1] = Obj_cpy(obj[1]);
    obj[0].val.li->elements[0].val.i++;
    return Obj_cpy(obj[0]);
}

Object clear_list(Object*obj,int n_arg){
    Obj_free_array(obj[0].val.li->elements, obj[0].val.li->elements[0].val.i);
    obj->val.li->elements = malloc(sizeof(Object));
    obj->val.li->elements[0] = new_ount(0);
    return nil_Obj;
}

Object list_reverse(Object *argv, int argc){
    int len = get_list_len(argv[0]);
    Object temp;
    if (len < 2)
        return nil_Obj;
    for(int i = 0; i < len / 2; i++){
        temp = argv[0].val.li->elements[i + 1];
        argv[0].val.li->elements[i + 1] = argv[0].val.li->elements[len - i]; 
        argv[0].val.li->elements[len - i] = temp;
    }
    return nil_Obj;
}

Object init_list(memory *MEMORY,char*path){
    char* module_name = "list";

    Object mod = new_Module();

    mod.val.module->filename = malloc(sizeof(char) * (1 + strlen(module_name)));
    strcpy(mod.val.module->filename, module_name);

    add_func_Module(mod, "append", &append_list, "");
    add_func_Module(mod, "clear", &clear_list, "");
    add_func_Module(mod, "reverse", &list_reverse, "");

    list_module = *mod.val.module;
    return mod;

}