#include "std.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../include/memlib.h"
#include "../../include/utilities.h"

Object read_prompt(Object*o,int n_arg) {
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
    if(n_arg>1){
        for(int i=0;i<n_arg-1;i++){
            print_prompt(&obj[i],1);
            printf(" ");
        }
        return print_prompt(&obj[n_arg-1],1);

    }
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
        printf("%lld",*obj->val.i);
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
        if(*obj->val.i==0){
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



memory init_std(memory MEMORY){
//-------------------------------------
    //add print
    MEMORY.len+=1;
    MEMORY.keys=realloc_c(MEMORY.keys,sizeof(char*)*(MEMORY.len-1),sizeof(char*)*MEMORY.len);

    MEMORY.keys[MEMORY.len-1]=malloc(sizeof(char)*(1+strlen("print")));
    strcpy(MEMORY.keys[MEMORY.len-1],"print");  

    MEMORY.values=realloc_c(MEMORY.values,sizeof(Object)*(MEMORY.len-1),sizeof(Object)*MEMORY.len);
    MEMORY.values[MEMORY.len-1].type=Obj_funcid_t;
    MEMORY.values[MEMORY.len-1].val.funcid=malloc(sizeof(Funcdef));
    *MEMORY.values[MEMORY.len-1].val.funcid=new_blt_func("print",&print_prompt," ");
//-------------------------------------
    //add println
    MEMORY.len+=1;
    MEMORY.keys=realloc_c(MEMORY.keys,sizeof(char*)*(MEMORY.len-1),sizeof(char*)*MEMORY.len);

    MEMORY.keys[MEMORY.len-1]=malloc(sizeof(char)*(1+strlen("println")));
    strcpy(MEMORY.keys[MEMORY.len-1],"println");  

    MEMORY.values=realloc_c(MEMORY.values,sizeof(Object)*(MEMORY.len-1),sizeof(Object)*MEMORY.len);
    MEMORY.values[MEMORY.len-1].type=Obj_funcid_t;
    MEMORY.values[MEMORY.len-1].val.funcid=malloc(sizeof(Funcdef));
    *MEMORY.values[MEMORY.len-1].val.funcid=new_blt_func("println",&println_prompt," ");
//-------------------------------------
    //add bool
    MEMORY.len+=1;
    MEMORY.keys=realloc_c(MEMORY.keys,sizeof(char*)*(MEMORY.len-1),sizeof(char*)*MEMORY.len);

    MEMORY.keys[MEMORY.len-1]=malloc(sizeof(char)*(1+strlen("bool")));
    strcpy(MEMORY.keys[MEMORY.len-1],"bool");  

    MEMORY.values=realloc_c(MEMORY.values,sizeof(Object)*(MEMORY.len-1),sizeof(Object)*MEMORY.len);
    MEMORY.values[MEMORY.len-1].type=Obj_funcid_t;
    MEMORY.values[MEMORY.len-1].val.funcid=malloc(sizeof(Funcdef));
    *MEMORY.values[MEMORY.len-1].val.funcid=new_blt_func("bool",&std_bool," ");
//-------------------------------------
    //add input
    MEMORY.len+=1;
    MEMORY.keys=realloc_c(MEMORY.keys,sizeof(char*)*(MEMORY.len-1),sizeof(char*)*MEMORY.len);

    MEMORY.keys[MEMORY.len-1]=malloc(sizeof(char)*(1+strlen("input")));
    strcpy(MEMORY.keys[MEMORY.len-1],"input");  

    MEMORY.values=realloc_c(MEMORY.values,sizeof(Object)*(MEMORY.len-1),sizeof(Object)*MEMORY.len);
    MEMORY.values[MEMORY.len-1].type=Obj_funcid_t;
    MEMORY.values[MEMORY.len-1].val.funcid=malloc(sizeof(Funcdef));
    *MEMORY.values[MEMORY.len-1].val.funcid=new_blt_func("input",&read_prompt," ");
        
    
    return MEMORY;
}

