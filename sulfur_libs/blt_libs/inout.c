#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../include/memlib.h"
#include "../../include/utilities.h"
#include "../../include/instruction.h"

Object read_prompt(Object*o) {
    int capacity = 16;
    int length = 0;
    char* result = malloc(capacity * sizeof(char));
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        result[length++] = c;
        if (length == capacity) {
            capacity+=10;
            result = realloc(result, capacity * sizeof(char));
        }
    }
    result[length++]='\0';
    result=realloc(result, length * sizeof(char));
    return (Object){.type=Obj_string_t,.val.s=result};
}
 

Object print_prompt(Object*obj,int n_arg){
    if(obj->type==Obj_string_t){
        printf("%s",obj->val.s);
        return (Object){.type=Obj_ount_t,.val.i=0};
    }
    if(obj->type==Obj_boolean_t){
        printf(*obj->val.b==0?"0b":"1b");
        return (Object){.type=Obj_ount_t,.val.i=0};
    }
    if(obj->type==Obj_class_t){
        if(!str_ar_contains_str(obj->val.cl->keys,"__string__",obj->val.cl->key_len)){
            printf("instance at :%x",obj);
            return (Object){.type=Obj_ount_t,.val.i=0};
        }
        
    }
    if(obj->type==Obj_complex_t){
        printf("%Lf + %Lfi",obj->val.c[0],obj->val.c[1]);
        return (Object){.type=Obj_ount_t,.val.i=0};
    }
    if(obj->type==Obj_floap_t){
        printf("%Lf",*obj->val.f);
        return (Object){.type=Obj_ount_t,.val.i=0};
    }
    if(obj->type==Obj_list_t){
        for(int i=0;i<obj->val.o[0].val.i;i++){
            print_prompt(&obj->val.o[i+1]);
            return (Object){.type=Obj_ount_t,.val.i=0};
        }
    }
    if(obj->type==Obj_funcid_t){
        printf("function at :%x",obj->val.funcid);
        return (Object){.type=Obj_ount_t,.val.i=0};
    }
    if(obj->type==Obj_nil_t){
        printf("nil");
        return (Object){.type=Obj_ount_t,.val.i=0};
    }
    if(obj->type==Obj_Obj_t){
        print_prompt(obj->val.o);
        return (Object){.type=Obj_ount_t,.val.i=0};
    }
    if(obj->type==Obj_ount_t){
        printf("%d",*obj->val.i);
        return (Object){.type=Obj_ount_t,.val.i=0};
    }
    if(obj->type==Obj_typeid_t){
        printf("%s",obj->val.typeid);
        return (Object){.type=Obj_ount_t,.val.i=0};
    }
    return (Object){.type=Obj_ount_t,.val.i=1};

}

Object println_prompt(Object*obj,int n_arg){
    Object n=print_prompt(obj,n_arg);
    printf("\n");
    return n;
}

void init_inout(void){
    LIBS_len++;
    LIBS=realloc(LIBS,sizeof(Lib)*LIBS_len);
    LIBS[LIBS_len-1].name=" ";
    LIBS[LIBS_len-1].nbr_funcs=2;
    LIBS[LIBS_len-1].funcs=malloc(sizeof(Funcdef)*LIBS[LIBS_len-1].nbr_funcs)
    

    LIBS[LIBS_len-1].funcs[0].arg_names=NULL;
    LIBS[LIBS_len-1].funcs[0].arg_types=malloc(sizeof(char*));
    LIBS[LIBS_len-1].funcs[0].arg_types[0]=malloc(sizeof(char)*(1+strlen("any")));
    strcpy(LIBS[LIBS_len-1].funcs[0].arg_types[0],"any");

    LIBS[LIBS_len-1].funcs[1].code=NULL;
    LIBS[LIBS_len-1].funcs[1].func_p=&print_prompt;
    LIBS[LIBS_len-1].funcs[1].is_builtin=1;
    LIBS[LIBS_len-1].funcs[1].name="print";
    LIBS[LIBS_len-1].funcs[1].nbr_of_args=1;
    LIBS[LIBS_len-1].funcs[1].nbr_ret_type=1;

    LIBS[LIBS_len-1].funcs[1].ret_type=malloc(sizeof(char*));
    LIBS[LIBS_len-1].funcs[1].ret_type[0]=malloc(sizeof(char)*(1+strlen("void")));
    strcpy(LIBS[LIBS_len-1].funcs[1].ret_type[0],"void");

    
    LIBS[LIBS_len-1].funcs[0].arg_names=NULL;
    LIBS[LIBS_len-1].funcs[0].arg_types=malloc(sizeof(char*));
    LIBS[LIBS_len-1].funcs[0].arg_types[0]=malloc(sizeof(char)*(1+strlen("any")));
    strcpy(LIBS[LIBS_len-1].funcs[0].arg_types[0],"any");

    LIBS[LIBS_len-1].funcs[1].code=NULL;
    LIBS[LIBS_len-1].funcs[1].func_p=&println_prompt;
    LIBS[LIBS_len-1].funcs[1].is_builtin=1;
    LIBS[LIBS_len-1].funcs[1].name="println";
    LIBS[LIBS_len-1].funcs[1].nbr_of_args=1;
    LIBS[LIBS_len-1].funcs[1].nbr_ret_type=1;

    LIBS[LIBS_len-1].funcs[1].ret_type=malloc(sizeof(char*));
    LIBS[LIBS_len-1].funcs[1].ret_type[0]=malloc(sizeof(char)*(1+strlen("void")));
    strcpy(LIBS[LIBS_len-1].funcs[1].ret_type[0],"void");
}



