#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "memlib.h"
#include "utilities.h"

char* read_prompt() {
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
    return realloc(result, length * sizeof(char));
}


int print_prompt(Object*obj){
    if(obj->type==Obj_string_t){
        printf("%s",obj->val.s);
        return 0;
    }
    if(obj->type==Obj_boolean_t){
        printf(*obj->val.b==0?"0b":"1b");
        return 0;
    }
    if(obj->type==Obj_class_t){
        if(!str_ar_contains_str(obj->val.cl->keys,"__string__",obj->val.cl->key_len)){
            printf("instance at :%x",obj);
            return 0;
        }
        
    }
    if(obj->type==Obj_complex_t){
        printf("%Lf + %Lfi",obj->val.c[0],obj->val.c[1]);
        return 0;
    }
    if(obj->type==Obj_floap_t){
        printf("%Lf",*obj->val.f);
        return 0;
    }
    if(obj->type==Obj_list_t){
        for(int i=0;i<obj->val.o[0].val.i;i++){
            print_prompt(&obj->val.o[i+1]);
            return 0;
        }
    }
    if(obj->type==Obj_funcid_t){
        printf("function at :%x",obj->val.funcid);
        return 0;
    }
    if(obj->type==Obj_nil_t){
        printf("nil");
        return 0;
    }
    if(obj->type==Obj_Obj_t){
        print_prompt(obj->val.o);
        return 0;
    }
    if(obj->type==Obj_ount_t){
        printf("%d",*obj->val.i);
        return 0;
    }
    if(obj->type==Obj_typeid_t){
        printf("%s",obj->val.typeid);
        return 0;
    }
    return 1;

}

int println_prompt(Object*obj){
    print_prompt(obj);
    printf('\n');
}