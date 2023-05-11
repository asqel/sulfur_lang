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

Object str_lower(Object*x, int argc){
    if(argc != 1){
        printf("lower only takes one arg %d were passed",argc);
    }
    Object res = new_string(x[0].val.s);
    
    int len = strlen(res.val.s);
    for(int i=0; i < len; i++){
        if( 'A' <= res.val.s[i] && res.val.s[i] <= 'Z'){
            res.val.s[i] = res.val.s[i] - ('A' - 'a');
        }

    }
    return res;
}

Object str_contains(Object* argv, int argc){
    if(argc != 2){
        printf("ERROR str:contains only takes 2 args");
        exit(1);
    }
    if(argv[0].type != Obj_string_t || argv[1].type != Obj_string_t){
        printf("ERROR str:contains onlyt takes strings");
        exit(1);
    }
    return new_boolean(strstr(argv[0].val.s, argv[1].val.s));
}


//char* str_escape() {
//    size_t len = strlen(str);
//    char* result = malloc(sizeof(char) * (len + 1));
//    int i = 0;
//    int j = 0;
//    while (i < len) {
//        if (str[i] == '\\') {
//            i++;
//            switch (str[i]) {
//                case 'a': result[j++] = '\a'; break;
//                case 'b': result[j++] = '\b'; break;
//                case 'f': result[j++] = '\f'; break;
//                case 'n': result[j++] = '\n'; break;
//                case 'r': result[j++] = '\r'; break;
//                case 't': result[j++] = '\t'; break;
//                case 'v': result[j++] = '\v'; break;
//                case '\\': result[j++] = '\\'; break;
//                default: result[j++] = '\\'; result[j++] = str[i]; break;
//            }
//        } else {
//            result[j++] = str[i];
//        }
//        i++;
//    }
//    result[j] = '\0';
//    return realloc(result, sizeof(char) * j);
//}

memory init_string(memory MEMORY,char*path){
    char* module_name = "string";

    Object mod = new_Module();

    mod.val.module->filename = malloc(sizeof(char) * (1 + strlen(module_name)));
    strcpy(mod.val.module->filename, module_name);


    add_func_Module(mod,"upper",&str_upper,"");
    add_func_Module(mod,"UPPER",&str_upper,"");
    add_func_Module(mod,"lower",&str_lower,"");
    add_func_Module(mod,"contains",&str_contains,"");
    //add_func_Module(mod,"escape",&str_escape,""); // transforme \t to ttabs \n to line feed
    //CamelCaseTo_snake_case
    //snake_case_toCamelCase
    //function to add upper case every each space like "salut ici " -> "Salut Ici"
    string_module = *mod.val.module;
    return MEMORY;

}
