#include "string_su.h"
#include "../../include/memlib.h"

#include <stdlib.h>
#include <stdio.h>

Module string_module;

char * string_methods = "upper() : return a new string by converting every letter to upper case\n"
                        "UPPER() : same as upper\n"
                        "lower() : return a new string by converting every letter to lower case\n"
                        "contains(x) : check if x is in the string\n"
                        "length() : return the length of the string\n"
                        "starts_with(a,...) : check if the string starts with at least one the args\n" 
;
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
    return new_boolean(strstr(argv[0].val.s,argv[1].val.s) != NULL);
}

Object str_length(Object* argv, int argc){
    if(argc > 1){
        printf("ERROR str:length doesn't take any arg");
        exit(1);
    }
    return new_ount(strlen(argv[0].val.s));
}
Object str_starts_with(Object* argv, int argc){
    if(argc == 1){
        printf("ERROR str:starts_with takes at least one arg");
        exit(1);
    }
    for(int i=0; i<argc; i++){
        if(argv[i].type != Obj_string_t){
            printf("ERROR str:starts_with only takes strings as arg");
            exit(1);
        }
    }
    if(argc == 2){
        if(strlen(argv[0].val.s) < strlen(argv[1].val.s)){
            return new_boolean(0);
        }
        for(int i=0; i < strlen(argv[1].val.s); i++){
            if(argv[0].val.s[i] != argv[1].val.s[i]){
                return new_boolean(0);
            }
        }
        return new_boolean(1);
    }
    int res = 0;
    for(int i=1; i < argc; i++){
        if(strlen(argv[0].val.s) < strlen(argv[i].val.s)){
            continue;
        }
        for(int i=0; i < strlen(argv[i].val.s); i++){
            if(argv[0].val.s[i] != argv[i].val.s[i]){
                continue;
            }
        }
        res ++;
        return new_boolean(res);
    }
    return new_boolean(res);
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

Object str_get(Object* argv, int argc){
    Object index = argv[1];
    int len=strlen(argv[0].val.s);
    if (*index.val.i==-1){
        Object res;
        res.type=Obj_ount_t;
        res.val.s=malloc(sizeof(long long int));
        *res.val.s=len;
        return res;
    }
    if (*index.val.i >= len || *index.val.i<-1){
        printf("ERROR get out of range");
        exit(1);
    }
    Object res;
    res.type=Obj_string_t;
    res.val.s=malloc(sizeof(char)*2);
    res.val.s[0]=argv[0].val.s[*index.val.i];
    res.val.s[1]='\0';
    return res;
}

Object str_set(Object* argv, int argc){
    argv[0].val.s[*argv[1].val.i] = argv[2].val.s[0];
    return new_string(argv[0].val.s);
}

memory init_string(memory MEMORY,char*path){
    char* module_name = "string";

    Object mod = new_Module();

    mod.val.module->filename = malloc(sizeof(char) * (1 + strlen(module_name)));
    strcpy(mod.val.module->filename, module_name);


    add_func_Module(mod,"upper",&str_upper,"");
    add_func_Module(mod,"UPPER",&str_upper,"");
    add_func_Module(mod,"lower",&str_lower,"");
    add_func_Module(mod,"contains",&str_contains,"");
    add_func_Module(mod,"length",&str_length,"");
    add_func_Module(mod,"len",&str_length,"");
    add_func_Module(mod,"starts_with",&str_starts_with,"");
    add_func_Module(mod,"get",&str_get,"");
    add_func_Module(mod,"set",&str_set,"");
    //add_func_Module(mod,"escape",&str_escape,""); // transforme \t to ttabs \n to line feed
    //CamelCaseTo_snake_case
    //snake_case_toCamelCase
    //function to add upper case every each space like "salut ici " -> "Salut Ici"
    string_module = *mod.val.module;
    return MEMORY;

}
