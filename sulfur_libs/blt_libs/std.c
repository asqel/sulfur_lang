#include "std.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../include/memlib.h"
#include "../../include/utilities.h"

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
        return nil_Obj;
    }
    if(obj->type==Obj_boolean_t){
        printf(*obj->val.b==0?"0b":"1b");
        return nil_Obj;
    }
    if(obj->type==Obj_class_t){
        if(!str_ar_contains_str(obj->val.cl->keys,"__string__",obj->val.cl->key_len)){
            printf("instance at :%x",obj);
            return nil_Obj;
        }
        
    }
    if(obj->type==Obj_complex_t){
        printf("%Lf + %Lfi",obj->val.c[0],obj->val.c[1]);
        return nil_Obj;
    }
    if(obj->type==Obj_floap_t){
        printf("%Lf",*obj->val.f);
        return nil_Obj;
    }
    if(obj->type==Obj_list_t){
        for(int i=0;i<*(obj->val.o[0].val.i);i++){
            print_prompt(&obj->val.o[i+1],1);
            return nil_Obj;
        }
    }
    if(obj->type==Obj_funcid_t){
        printf("function at :%x",obj->val.funcid);
        return nil_Obj;
    }
    if(obj->type==Obj_nil_t){
        printf("nil");
        return nil_Obj;
    }
    if(obj->type==Obj_Obj_t){
        //print_prompt(obj->val.o,);
        return nil_Obj;
    }
    if(obj->type==Obj_ount_t){
        printf("%d",*obj->val.i);
        return nil_Obj;
    }
    if(obj->type==Obj_typeid_t){
        printf("%s",obj->val.typeid);
        return nil_Obj;
    }
    return nil_Obj;

}

Object println_prompt(Object*obj,int n_arg){
    Object n=print_prompt(obj,n_arg);
    printf("\n");
    return n;
}


Object std_bool(Object*obj,int n_arg){
    if(n_arg!=1){
        printf("ERROR %s only 1 argument needed in bool call",n_arg>1?"too many arguments":"too few arguments");
        exit(1);
    }
    Object res;
    res.type=Obj_boolean_t;
    res.val.b=malloc(sizeof(short int));
    *res.val.b=1;
    if(obj->type==Obj_ount_t){
        if(obj->val.i==0){
            *res.val.b=0;
        }
        return res;
    }
    if(obj->type==Obj_string_t){
        if(obj->val.s[0]==0){
            *res.val.b=0;
        }
        return res;
    }
    if(obj->type==Obj_boolean_t){
        *res.val.b=*obj->val.b;
        return res;
    }
}



