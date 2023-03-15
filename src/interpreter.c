#include "../include/interpreter.h"
#include "../include/instruction.h"
#include <stdlib.h>

//builtin libs
#include "../sulfur_libs/blt_libs/std.h"
#include "../include/operation.h"



Object*MEMORY;//array of Objec
char**MEMORY_key;//array of st
Object**STACK;//end with an ar
char***STACK_KEY;//end with an

long long int MEMORY_len;
long long int STACK_len;
long long int*sub_STACK_len;//

Funcdef*FUNCDEFS;
long long int FUNCDEFS_len;

CLASSDEF*CLASSDEFS;
long long int CLASSDEF_len;

ref_counter*REF_COUNTS;
long long int REF_COUNT_len;



//void check_libs(){
//    for(int i=0;i<LIBS_len;i++){
//        if(LIBS[i].name[0]==" "[0]){
//            FUNCDEFS_len+=LIBS[i].nbr_funcs;
//            FUNCDEFS=realloc(FUNCDEFS,sizeof(Funcdef)*FUNCDEFS_len);
//            for(int k=0;k<LIBS[i].nbr_funcs;k++){
//                FUNCDEFS[FUNCDEFS_len-LIBS[i].nbr_funcs+k]=LIBS[i].funcs[k];
//
//            }
//        }
//    }
//}


void init_memory(){
    MEMORY=malloc(sizeof(Object));
    MEMORY_key=malloc(sizeof(char *));
    MEMORY_len=0;
}


void init_stack(){
    STACK=malloc(sizeof(Object*));
    STACK_KEY=malloc(sizeof(char**));
    STACK_len=0;
    sub_STACK_len=malloc(sizeof(int));
    *sub_STACK_len=0;
}

void init_funcdefs(){
    FUNCDEFS=malloc(sizeof(Funcdef));
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



Object eval_Ast(Ast*x){
    if(x->type==Ast_funccall_t){
        if(!strcmp(x->root.fun->name,"print")){
            int n=-1;
            for(int i=0;i<MEMORY_len;i++){
                if(!strcmp(MEMORY_key[i],"pr")){
                    n=i;
                    break;
                }
            }
            if(n!=-1){
                return print_prompt(&MEMORY[n],1);
                
            }
        }
        else{
            printf("function doesnt exit");
            exit(1);
        }
    }
    if(x->left==NULL && x->right==NULL&& !x->isAst){
        if(x->type == Ast_varcall_t){
            printf("ERROR not implemented yet(varcall)");
            exit(-1);
        }
        if(x->type == Ast_object_t){
            return *(x->root.obj);
        }
        else{
            printf("ERROR in Ast");
            exit(-1);
        }
    }
    if(x->isAst){
        if(x->type==Ast_add_t){
            if(x->left!=NULL && x->right!=NULL){
                Object a=eval_Ast(x->left);
                Object b=eval_Ast(x->right);
                return add(a,b);
            }
        }
    }
}

//return 1 if the object already exists
//return 0 if it doesnt exists
//each object of a list will be add_red-ed automaticly even the first element wich is the size of the list
//class are not supported yet
int add_ref(Object o){
    int find=-1;
    for(int i=0;i<REF_COUNT_len;i++){
        if(REF_COUNTS[i].pointer==get_obj_pointer(o)){
            find=i;
        }
    }
    if(find!=-1){
        REF_COUNTS[find].count++;
        return 1;
    }
    if(o.type=Obj_class_t){
        return 0;
    }
    if(o.type=Obj_list_t){
        REF_COUNT_len++;
        REF_COUNTS=realloc(REF_COUNTS,sizeof(ref_counter)*REF_COUNT_len);
        REF_COUNTS[REF_COUNT_len-1].count=1;
        REF_COUNTS[REF_COUNT_len-1].type=Obj_list_t;
        REF_COUNTS[REF_COUNT_len-1].pointer->li=o.val.li;
        int len=o.val.li->len;
        for(int i=0;i<len;i++){
            add_ref(o.val.li->elements[i]);
        }
        return 0;
    }
    REF_COUNT_len++;
    REF_COUNTS=realloc(REF_COUNTS,sizeof(ref_counter)*REF_COUNT_len);
    REF_COUNTS[REF_COUNT_len-1].count=1;
    REF_COUNTS[REF_COUNT_len-1].type=o.type;
    //REF_COUNTS[REF_COUNT_len-1].pointer=get_obj_pointer(o); faut faire switch case pointer.type
    return 0;
}

int execute(Instruction*code,char*file_name,int len){
    int p=0;
    //check_libs();
    while(p<len){
        if(code[p].type==inst_varset_t){
            for(int i=0;i<MEMORY_len;i++){
                if(!strcmp(MEMORY_key[i],code[p].value.vs->name)){
                    printf("error redefinition of var please delete var befor");
                    exit(1);
                }
            }
            MEMORY_len++;
            MEMORY=realloc(MEMORY,sizeof(Object)*MEMORY_len);
            MEMORY_key=realloc(MEMORY_key,sizeof(char*)*MEMORY_len);
            MEMORY_key[MEMORY_len-1]=malloc(sizeof(char)*(1+strlen(code[p].value.vs->name)));
            strcpy(MEMORY_key[MEMORY_len-1],code[p].value.vs->name);
            Object o=eval_Ast(code[p].value.vs->val);//faut le raplce par Object pas Object*
            add_ref(o);
            MEMORY[MEMORY_len-1]=o;
            p++;
            continue;       

        }
        if(code[p].type==inst_new_varset_t){
            int n=-1;
            for(int i=0;i<MEMORY_len;i++){
                if(!strcmp(MEMORY_key[i],code[p].value.vs->name)){
                    n=i;
                    break;
                }
            }
            MEMORY[n]=eval_Ast(code[p].value.vs->val);
            p++;         
        }
        if(code[p].type==inst_if_t){
            Object condition=eval_Ast(code[p].value.i->condition);
            if(*(std_bool(&condition,1).val.b)){
                p++;
                continue;
            }
            p=code[p].value.i->endif_p+1;
            continue;
        }
        if(code[p].type==inst_elif_t){
            Object condition=eval_Ast(code[p].value.el->condition);
            if(*(std_bool(&condition,1).val.b)){
                p++;
                continue;
            }
            p=code[p].value.el->endif_p+1;
            continue;
        }
        if(code[p].type==inst_else_t){
            p++;
            continue;
        }
        if(code[p].type==inst_endif){
            p=code[p].value.endifelse+1;
            continue;

        }
        if(code[p].type==inst_endifelse){
            p++;
            continue;
        }
        if(code[p].type==inst_return_t){
            Object*x=malloc(sizeof(Object));
            *x=eval_Ast(code[p].value.ret);
            p++;
            continue;
        }

        if(code[p].type==inst_expr_t){
            eval_Ast(code[p].value.expr);
            p++;
            continue;
        }
        if(code[p].type==inst_section_t){
            p++;
        }
        if(code[p].type==inst_goto_t){
            int n=-1;
            //search down
            for(int i=p+1;i<len;i++){
                if(code[i].type==inst_section_t){
                    if(!strcmp(code[p].value.goto_sec,code[i].value.section)){
                        n=i;
                        break;
                    }
                }
            }
            if(n==-1){
                //search up
                for(int i=p-1;i>=0;i--){
                    if(code[i].type==inst_section_t){
                        if(!strcmp(code[p].value.goto_sec,code[i].value.section)){
                            n=i;
                            break;
                        }
                    }
                }
            }
            if(n==-1){
                printf("section %s doesnt exists",code[p].value.goto_sec);
                exit(1);
            }
            p=n;
            continue;
        }
    }
    printf("\n \nMEMORY:\n",MEMORY_len);
    for(int i=0;i<MEMORY_len;i++){
        printf("    %s: ",MEMORY_key[i]);
        println_prompt(&MEMORY[i],1);
    }
    return 0;
}