#include "../include/func_interpreter.h"
#include "../include/instruction.h"
#include <stdlib.h>

#include "../sulfur_libs/blt_libs/std.h"
#include "../sulfur_libs/blt_libs/string_su.h"
#include "../sulfur_libs/blt_libs/funccall_su.h"

#include "../include/operation.h"
#include "../include/utilities.h"
#include "../include/token_class.h"
#include "../include/evaluator.h"

extern memory MEMORY;

stack STACK;

void init_stack(){
    STACK.len = 0;
    STACK.MEM = malloc(sizeof(memory));
}

Object func_execute(Instruction*code,char*file_name,int len){
    STACK.len++;
    STACK.MEM = realloc(STACK.MEM, sizeof(memory) * STACK.len);
    int p=0;
    while(p<len){
        if(code[p].type==inst_pass_t){
            p++;
            continue;
        }
        
        if(code[p].type==inst_varset_t){
            if(get_Obj_mem(STACK.MEM[STACK.len - 1], code[p].value.vs->name).type != Obj_not_found_t){
                printf("ERROR redefinition of variable in fucntion");
                exit(1);
            }
            Object o=eval_Ast(code[p].value.vs->val);
            add_object(&STACK.MEM[STACK.len - 1], code[p].value.vs->name, o);
            p++;
            continue;       

        }
        if(code[p].type==inst_new_varset_t){
            int n=-1;
            for(int i = 0; i < STACK.MEM[STACK.len - 1].len; i++){
                if(!strcmp(STACK.MEM[STACK.len - 1].keys[i], code[p].value.vs->name)){
                    n = i;
                    break;
                }
            }
            if(n == -1){
                for(int i = 0; i < MEMORY.len; i++){
                if(!strcmp(MEMORY.keys[i], code[p].value.vs->name)){
                    n = i;
                    break;
                }
                MEMORY.values[n]=eval_Ast(code[p].value.vs->val);
                p++;
                continue;
            }
            }
            STACK.MEM[STACK.len - 1].values[n]=eval_Ast(code[p].value.vs->val);

            p++;  
            continue;       
        }
        if(code[p].type==inst_if_t){
            Object condition=eval_Ast(code[p].value.i->condition);
            Object c=std_bool(&condition,1);
            if(*(c.val.b)){
                p++;
                continue;
            }
            p=code[p].value.i->endif_p+1;
            continue;
        }
        if(code[p].type==inst_elif_t){
            Object condition=eval_Ast(code[p].value.el->condition);
            Object c=std_bool(&condition,1);
            if(*(c.val.b)){
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
            return eval_Ast(code[p].value.ret);
        }

        if(code[p].type==inst_expr_t){
            Object x=eval_Ast(code[p].value.expr);
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
        if(code[p].type==inst_for_t){
            Object start=eval_Ast(code[p].value.fo->start);
            start=std_ount(&start,1);
            Object end=eval_Ast(code[p].value.fo->end);
            end=std_ount(&end,1);
            if(start.type==Obj_nil_t){
                printf("ERROR cant convert the value of start to ount in for");
                exit(1);
            }
            if(end.type==Obj_nil_t){
                printf("ERROR cant convert the value of end to ount in for");
                exit(1);
            }

            int n=-1;
            for(int i=0;i<MEMORY.len;i++){
                if(!strcmp(MEMORY.keys[i],code[p].value.fo->var_name)){
                    n=i;
                    break;
                }
            }
            if(n==-1){
                MEMORY.len++;
                MEMORY.values=realloc_c(MEMORY.values,sizeof(Object)*(MEMORY.len-1),sizeof(Object)*MEMORY.len);
                MEMORY.keys=realloc_c(MEMORY.keys,sizeof(char*)*(MEMORY.len-1),sizeof(char*)*MEMORY.len);
                MEMORY.keys[MEMORY.len-1]=malloc(sizeof(char)*(1+strlen(code[p].value.fo->var_name)));
                strcpy(MEMORY.keys[MEMORY.len-1],code[p].value.fo->var_name);
                MEMORY.values[MEMORY.len-1].type=Obj_ount_t;
                MEMORY.values[MEMORY.len-1].val.i=malloc(sizeof(long long int));
                *MEMORY.values[MEMORY.len-1].val.i=*start.val.i;
            }
            else{
                MEMORY.values[n].type=Obj_ount_t;
                MEMORY.values[n].val.i=malloc(sizeof(long long int));
                *MEMORY.values[n].val.i=*start.val.i;

            }

            if(*start.val.i==*end.val.i){
                p=code[p].value.fo->endfor+1;
                continue;
            }
            p++;
        }
        if(code[p].type==inst_endfor_t){
            int for_p=code[p].value.endfor;
            Object start=eval_Ast(code[for_p].value.fo->start);
            start=std_ount(&start,1);

            Object end=eval_Ast(code[for_p].value.fo->end);
            end=std_ount(&end,1);


            //positive for 
            if(*start.val.i<*end.val.i){
                int n=-1;
                for(int i=0;i<MEMORY.len;i++){
                    if(!strcmp(MEMORY.keys[i],code[for_p].value.fo->var_name)){
                        n=i;
                        break;
                    }
                }
                if(n==-1){
                    p++;
                    continue;
                }
                
                MEMORY.values[n]=std_ount(&MEMORY.values[n],1);
                if(MEMORY.values[n].type==Obj_nil_t){
                    printf("ERROR in for cant convert loop var to ount");
                    exit(1);
                }
                (*MEMORY.values[n].val.i)++;

                if(*end.val.i>*MEMORY.values[n].val.i){

                    p=for_p+1;
                    continue;
                }
                else{
                    p++;
                    continue;
                }
            }
            else{
                int n=-1;
                for(int i=0;i<MEMORY.len;i++){
                    if(!strcmp(MEMORY.keys[i],code[for_p].value.fo->var_name)){
                        n=i;
                        break;
                    }
                }
                if(n==-1){
                    p++;
                    continue;
                }
                MEMORY.values[n]=std_ount(&MEMORY.values[n],1);
                if(MEMORY.values[n].type==Obj_nil_t){
                    printf("ERROR in for cant convert loop var to ount");
                    exit(1);
                }
                (*MEMORY.values[n].val.i)--;

                if(*end.val.i<*MEMORY.values[n].val.i){

                    p=for_p+1;
                    continue;
                }
                else{
                    p++;
                    continue;
                }
            }
            
        }
        if(code[p].type==inst_while_t){
            Object condition=eval_Ast(code[p].value.wh->condition);
            Object c=std_bool(&condition,1);
            if(*c.val.b){
                p++;
                continue;
            }
            else{
                p=code[p].value.wh->endwhile+1;
                continue;
            }
        }
        if(code[p].type==inst_endwhile_t){
            int while_p=code[p].value.endwhile;
            Object condition=eval_Ast(code[while_p].value.wh->condition);
            Object c=std_bool(&condition,1);
            if(*c.val.b){
                p=code[p].value.endwhile+1;
                continue;
            }
            else{
                p++;
            }
        }
        if(code[p].type==inst_funcdef_t){
            int n=-1;
            printf("rrrr");
            for(int i=0;i<MEMORY.len;i++){
                if(!strcmp(MEMORY.keys[i],code[p].value.fc->name)){
                    n=i;
                    break;
                }
            }
            if(n==-1){
                Object f;
                printf("ta crasgh ?");

                f.type=Obj_funcid_t;
                f.val.funcid=malloc(sizeof(Funcdef));
                f.val.funcid->code=code[p].value.fc->code;
                f.val.funcid->code_len=code[p].value.fc->code_len;
                f.val.funcid->is_builtin=0;
                add_object(&MEMORY,code[p].value.fc->name,f);
                p++;
                continue;
            }
            else{
                printf("ERROR function has same name as variable or another function");
                exit(1);

            }
            p++;
        }

        
    }
    
    return nil_Obj;
}