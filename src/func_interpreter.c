#include "../include/func_interpreter.h"
#include "../include/func_evaluator.h"
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


extern void add_loop_count(int index, int *loops_count, int **loops);
extern void remove_loop_count(int *loops_count, int **loops);

Object func_execute(Funcdef* func, Object* argv, int argc, int create_stack){
    if(create_stack){
        STACK.len++;
        STACK.MEM = realloc(STACK.MEM, sizeof(memory) * STACK.len);
    }

    if(func->nbr_of_args > argc){
        printf("ERROR too many args in functon call\n");
        exit(1);
    }
    if(func->nbr_of_args < argc){
        printf("ERROR too few args in functon call\n");
        exit(1);
    }

    for(int i = 0; i < argc; i++){
        add_object(&STACK.MEM[STACK.len - 1], func->arg_names[i], argv[i]);
    }

    int p=0;
    int len = func->code_len;
    int *loops = malloc(sizeof(int));
    int loops_count = 0; 
    Instruction* code = func->code;
    while(p<len){
        if(code[p].type==inst_pass_t){
            p++;
            continue;
        }
        
        if(code[p].type==inst_if_t){
            Object condition = func_eval_Ast(code[p].value.i->condition);
            Object old_cond = condition;
            Object c = std_bool(&condition, 1);
            Obj_free_val(old_cond);
            if(*(c.val.b)){
                Obj_free_val(c);
                p++;
                continue;
            }
            Obj_free_val(c);
            p = code[p].value.i->endif_p + 1;
            continue;
        }
        if(code[p].type==inst_elif_t){
            Object condition = func_eval_Ast(code[p].value.el->condition);
            Object old_cond = condition;
            Object c = std_bool(&condition, 1);
            Obj_free_val(old_cond);
            if(*(c.val.b)){
                Obj_free_val(c);
                p++;
                continue;
            }
            Obj_free_val(c);
            p = code[p].value.el->endif_p + 1;
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
            return func_eval_Ast(code[p].value.ret);
        }

        if(code[p].type==inst_expr_t){
            Object x = func_eval_Ast(code[p].value.expr);
            Obj_free_val(x);
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
                printf("ERROR section %s doesnt exists",code[p].value.goto_sec);
                exit(1);
            }
            p=n;
            continue;
        }
        if(code[p].type == inst_for_t){
            add_loop_count(p, &loops_count, &loops);
            Object start = func_eval_Ast(code[p].value.fo->start);
            Object old_start = start;
            start = std_ount(&start,1);
            Obj_free_val(old_start);

            
            Object end = func_eval_Ast(code[p].value.fo->end); 
            Object old_end = end;
            end=std_ount(&end,1);
            Obj_free_val(old_end);

            if(start.type == Obj_nil_t){
                printf("ERROR cant convert the value of start to ount in for\n");
                exit(1);
            }
            if(end.type == Obj_nil_t){
                printf("ERROR cant convert the value of end to ount in for\n");
                exit(1);
            }
            int n = -1;
            for(int i=0; i < STACK.MEM[STACK.len - 1].len; i++){
                if(!strcmp(STACK.MEM[STACK.len - 1].keys[i], code[p].value.fo->var_name)){
                    n = i;
                    break;
                }
            }
            if(n == -1){
                add_object(&STACK.MEM[STACK.len - 1], code[p].value.fo->var_name, start);
            }
            else{
                Obj_free_val(STACK.MEM[STACK.len - 1].values[n]);
                STACK.MEM[STACK.len - 1].values[n] = start;

            }

            if(*start.val.i == *end.val.i){
                Obj_free_val(end);
                p = code[p].value.fo->endfor + 1;
                remove_loop_count(&loops_count, &loops);
                continue;
            }
            Obj_free_val(end);
            p++;
        }
        if(code[p].type==inst_endfor_t){
            int for_p = code[p].value.endfor;

            Object start = func_eval_Ast(code[for_p].value.fo->start);
            Object old_start = start;
            start=std_ount(&start,1);
            Obj_free_val(old_start);

            Object end = func_eval_Ast(code[for_p].value.fo->end);
            Object old_end = end;
            end=std_ount(&end,1);
            Obj_free_val(old_end);


            //positive for 
            if(*start.val.i < *end.val.i){
                int n = -1;
                for(int i=0; i < STACK.MEM[STACK.len - 1].len; i++){
                    if(!strcmp(STACK.MEM[STACK.len - 1].keys[i],code[for_p].value.fo->var_name)){
                        n = i;
                        break;
                    }
                }
                if(n==-1){
                    p++;
                    Obj_free_val(end);
                    Obj_free_val(start);
                    continue;
                }
                Object old = MEMORY.values[n];
                MEMORY.values[n] = std_ount(&MEMORY.values[n],1);
                Obj_free_val(old);

                if(MEMORY.values[n].type==Obj_nil_t){
                    printf("ERROR in for cant convert loop var to ount\n");
                    exit(1);
                }
                (*MEMORY.values[n].val.i)++;

                if(*end.val.i > *MEMORY.values[n].val.i){
                    Obj_free_val(end);
                    Obj_free_val(start);
                    p = for_p + 1;
                    continue;
                }
                else{
                    Obj_free_val(start);
                    Obj_free_val(end);
                    p++;
                    remove_loop_count(&loops_count, & loops);
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
                    Obj_free_val(end);
                    Obj_free_val(start);
                    continue;
                }
                Object old = MEMORY.values[n];
                MEMORY.values[n]=std_ount(&MEMORY.values[n],1);
                Obj_free_val(old);

                if(MEMORY.values[n].type==Obj_nil_t){
                    printf("ERROR in for cant convert loop var to ount\n");
                    exit(1);
                }
                (*MEMORY.values[n].val.i)--;

                if(*end.val.i < *MEMORY.values[n].val.i){
                    Obj_free_val(start);
                    Obj_free_val(end);
                    p=for_p+1;
                    continue;
                }
                else{
                    Obj_free_val(start);
                    Obj_free_val(end);
                    p++;
                    continue;
                }
            }
            
        }
        if(code[p].type==inst_while_t){
            add_loop_count(p, &loops_count,& loops);
            Object condition = func_eval_Ast(code[p].value.wh->condition);
            Object c=std_bool(&condition,1);
            Obj_free_val(condition);
            if(*c.val.b){
                Obj_free_val(c);
                p++;
                continue;
            }
            else{
                Obj_free_val(c);
                p=code[p].value.wh->endwhile+1;
                remove_loop_count(&loops_count, &loops);
                continue;
            }
        }
        if(code[p].type==inst_endwhile_t){
            int while_p = code[p].value.endwhile;
            Object condition = func_eval_Ast(code[while_p].value.wh->condition);
            Object c=std_bool(&condition,1);
            Obj_free_val(condition);
            if(*c.val.b){
                Obj_free_val(c);
                p=code[p].value.endwhile+1;
                continue;
            }
            else{
                Obj_free_val(c);
                remove_loop_count(&loops_count, &loops);
                p++;
            }
        }
        if(code[p].type == inst_proceed_t){
            if(loops_count){
                int index = loops[loops_count - 1];
                if(code[index].type == inst_while_t){
                    p = code[index].value.wh->endwhile;
                }
                else if(code[index].type == inst_for_t){
                    p = code[index].value.fo->endfor;
                }
            }
            else{
                p = 0;
            }
            continue;
        }
        if(code[p].type == inst_stop_t){
            if(loops_count){
                int index = loops[loops_count - 1];
                if(code[index].type == inst_while_t){
                    p = code[index].value.wh->endwhile + 1;
                }
                else if(code[index].type == inst_for_t){
                    p = code[index].value.fo->endfor + 1;
                }
            }
            else{
                p = len;
            }
            continue;
        }
        if(code[p].type==inst_funcdef_t){
            int n=-1;
            for(int i=0;i<MEMORY.len;i++){
                if(!strcmp(MEMORY.keys[i],code[p].value.fc->info.name)){
                    n=i;
                    break;
                }
            }
            if(n==-1){
                Object f;
                f.type = Obj_funcid_t;
                f.val.funcid=malloc(sizeof(Funcdef));
                f.val.funcid->code = code[p].value.fc->code;
                f.val.funcid->code_len = code[p].value.fc->code_len;
                f.val.funcid->is_builtin=0;
                f.val.funcid->arg_names = code[p].value.fc->args;
                f.val.funcid->nbr_of_args = code[p].value.fc->args_len;
                add_object(&STACK.MEM[STACK.len - 1], code[p].value.fc->info.name, f);
                p++;
                continue;
            }
            else{
                printf("ERROR function has same name as variable or another function\n");
                exit(1);

            }
            p++;
        }
        
    }
    
    return nil_Obj;
}