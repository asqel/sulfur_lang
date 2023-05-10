#include "string_su.h"
#include "../../include/memlib.h"

#include <stdlib.h>
#include <stdio.h>

Module string_module;

Object str_upper(Object*x, int argc){
    if(argc != 1){
        printf("upper only takes one arg %d were passed",argc);
    }
    Object res = new_string(x[0].val.s);
    
    int len = strlen(res.val.s);
    for(int i=0; i < len; i++){
        if( 'a' <= res.val.s[i] && res.val.s[i] <= 'z'){
            res.val.s[i] = res.val.s[i] + ('A' - 'a');
        }

    }
    return res;
}

memory init_string(memory MEMORY,char*path){
    char* module_name = "string";

    Object mod = new_Module();

    mod.val.module->filename = malloc(sizeof(char) * (1 + strlen(module_name)));
    strcpy(mod.val.module->filename, module_name);


    add_func_Module(mod,"upper",&str_upper,"");

    string_module = *mod.val.module;
    return MEMORY;

}
