#include "../../include/memlib.h"
#include "list_su.h"
#include "../../include/operation.h"
#include <stdlib.h>
#include <stdio.h>
#include "std.h"

Module list_module;
char* list_methods = "append";

Object append_list(Object*obj,int n_arg){
    if (n_arg && obj[0].type == Obj_list_t) {
        if (n_arg == 2) {
            int len = obj[0].val.li->elements[0].val.i;
            obj[0].val.li->elements = realloc(obj->val.li->elements,(len+2)*sizeof(Object));
            obj[0].val.li->elements[len+1] = Obj_cpy(obj[1]);
            obj[0].val.li->elements[0].val.i++;
            return obj[0];
        }
        else if (n_arg == 1) {
            int len = obj[0].val.li->elements[0].val.i;
            obj[0].val.li->elements = realloc(obj->val.li->elements,(len+2)*sizeof(Object));
            obj[0].val.li->elements[len+1] = nil_Obj;
            obj[0].val.li->elements[0].val.i++;
            return obj[0];
        }
        else {
            int len = obj[0].val.li->elements[0].val.i;
            obj[0].val.li->elements = realloc(obj->val.li->elements,(len + 1 + (n_arg - 1))*sizeof(Object));
            for (int i = 0; i < n_arg - 1; i++) {
                print_prompt(&(obj[i + 1]), 1);
                obj[0].val.li->elements[len + 1 + i] = Obj_cpy(obj[i + 1]);
            }
            obj[0].val.li->elements[0].val.i += n_arg - 1;
            return nil_Obj;
        }
    }
    return nil_Obj;
    
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

Object list_contains(Object *argv, int argc) {
    if (argc && argv[0].type == Obj_list_t) {
        int len = argv[0].val.li->elements[0].val.i;
        if (argc == 1 && len == 0) 
            return new_boolean(1);
        for(int i = 0; i < len; i++) {
            for (int k = 1; k < argc; k++) {
                if (eq(argv[k], argv[0].val.li->elements[i]).val.b == 1)
                    return new_boolean(1);
            }
        }
    }
    return new_boolean(0);
}

Object init_list(memory *MEMORY,char*path){
    char* module_name = "list";

    Object mod = new_Module();

    mod.val.module->filename = malloc(sizeof(char) * (1 + strlen(module_name)));
    strcpy(mod.val.module->filename, module_name);

    add_func_Module(mod, "append", &append_list, "");
    add_func_Module(mod, "clear", &clear_list, "");
    add_func_Module(mod, "reverse", &list_reverse, "");
    add_func_Module(mod, "__call", &std_list, "");
    add_func_Module(mod, "contains", &list_contains, "");

    list_module = *mod.val.module;
    return mod;

}