#include "interpreter.h"




Object*MEMORY;//array of Objec
char**MEMORY_key;//array of st
Object**STACK;//end with an ar
char***STACK_KEY;//end with an

long long int MEMORY_len;
long long int STACK_len;
long long int*sub_STACK_len;//

Funcdef*FUNCDEFS;
char**FUNCDEFS_NAME;
long long int FUNCDEFS_len;

CLASSDEF*CLASSDEFS;
long long int CLASSDEF_len;

ref_counter*REF_COUNTS;
long long int REF_COUNT_len;






void init_memory(){
    MEMORY=malloc(sizeof(Object));
    MEMORY_key=malloc(sizeof(char *));
    MEMORY_len=0;
}


void init_stack(){
    STACK=malloc(sizeof(Object*));
    STACK_KEY=malloc(sizeof(char**));
    STACK_len=0;
   *sub_STACK_len=0;
}

void init_funcdefs(){
    FUNCDEFS=malloc(sizeof(Funcdef));
    FUNCDEFS_NAME=malloc(sizeof(char*));
    FUNCDEFS_len=0;
}

void init_classdefs(){
    CLASSDEF_len=0;
    CLASSDEFS=malloc(sizeof(CLASSDEF));
}

void init_garbage_collect(){
    REF_COUNT_len=0;
    REF_COUNTS=malloc(sizeof(ref_counter));
}



Object*eval_Ast(Ast*x){
    if(x->left==NULL && x->right==NULL){
        if(x->type==Ast_funccall_t){
            printf("ERROR not implemented yet(funccall)");
            exit(-1);
        }
        if(x->type == Ast_varcall_t){
            printf("ERROR not implemented yet(varcall)");
            exit(-1);
        }
        if(x->type == Ast_object_t){
            return x->root.obj;
        }
        else{
            printf("ERROR in Ast");
            exit(-1);
        }
    }
}

//return 1 if the object already exists
//return 0 if it doesnt exists
//each object of a list has to be add_ref-ed one by one 
int add_ref(Object*o){
    int find=-1;
    for(int i=0;i<REF_COUNT_len;i++){
        if(REF_COUNTS[i].pointer==o){
            find=i;
        }
    }
    if(find!=-1){
        REF_COUNTS[find].count++;
        return 1;
    }
    REF_COUNT_len++;
    REF_COUNTS=realloc(REF_COUNTS,sizeof(ref_counter)*REF_COUNT_len);
    REF_COUNTS[REF_COUNT_len-1].count=1;
    REF_COUNTS[REF_COUNT_len-1].pointer=o;
    REF_COUNTS[REF_COUNT_len-1].type= (o->type == Obj_list_t ? ref_list_t : ref_obj_t);
    return 0;
}

int execute(Instruction*code,char*file_name,int len){
    int p=0;
    while(p<len){
        if(code[p].type==inst_varset_t){
            MEMORY_len++;
            MEMORY=realloc(MEMORY,sizeof(Object)*MEMORY_len);
            MEMORY_key=realloc(MEMORY_key,sizeof(char*)*MEMORY_len);
            Object*o=eval_Ast(code[p].value.vs->val);
            add_ref(o);

        }
    }
    return 0;
}