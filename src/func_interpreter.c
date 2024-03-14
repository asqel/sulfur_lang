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
#include "../include/memlib.h"

extern memory MEMORY;

stack STACK;

void init_stack(){
    STACK.len = 0;
    STACK.MEM = calloc(1, sizeof(memory));
}

void create_stack(){
    STACK.len++;
    STACK.MEM = realloc(STACK.MEM, sizeof(memory) * STACK.len);
    STACK.MEM[STACK.len - 1].keys = NULL;
    STACK.MEM[STACK.len - 1].len = 0;
    STACK.MEM[STACK.len - 1].values = NULL;
}

void remove_one_stack() {
    for (int i = 0; i < STACK.MEM[STACK.len - 1].len; i++) {
        free(STACK.MEM[STACK.len - 1].keys[i]);
    }
    if (STACK.MEM[STACK.len - 1].keys)
        free(STACK.MEM[STACK.len - 1].keys);
    Obj_free_array(STACK.MEM[STACK.len - 1].values, STACK.MEM[STACK.len - 1].len);
    STACK.len--;
    STACK.MEM = realloc(STACK.MEM, sizeof(memory) * STACK.len);
}


extern void add_loop_count(int index, int *loops_count, int **loops);
extern void remove_loop_count(int *loops_count, int **loops);

Object func_execute(Object* argv, int argc, sulfur_func func, char *name, int add_stack){
    if (!add_stack && argc) {
        printf("ERROR func_excute cannot execute a function with arg without creating stack (%s)\n", name);
        exit(1);
    }
    if (add_stack)
        create_stack();
    if (func.args_mod == 'o') {
        for(int i = 0; i < argc; i++){
            add_object(&STACK.MEM[STACK.len - 1], func.args[i],  Obj_cpy(argv[i]));
        }
    }
    else if (func.args_mod == '+') {
        for (int i = 0; i < func.args_len - 1; i++) {
            add_object(&STACK.MEM[STACK.len - 1], func.args[i],  Obj_cpy(argv[i]));
        }
        int pack_start = func.args_len - 1;
        int pack_len = argc - (func.args_len - 1);
        if (pack_len) {
            Object packed = std_list(&(argv[pack_start]), pack_len);
            add_object(&STACK.MEM[STACK.len - 1], func.args[pack_start],packed);
        }
        else
            add_object(&STACK.MEM[STACK.len - 1], func.args[pack_start], std_list(NULL, 0));
    }

    int p = 0;
    int *loops = malloc(sizeof(int));
    *loops = 0;
    int loops_count = 1; 
    int len = func.code_len;
    Instruction *code = func.code;
    while(p < len){
        if(code[p].type == inst_pass_t){
            p++;
        }
        if(code[p].type == inst_if_t){
            Object condition = Obj_cpy(func_eval_Ast(code[p].value.i->condition));
            Object old_cond = condition;
            Object c = std_bool(&condition, 1);
            Obj_free_val(old_cond);
            if(c.val.b){
                Obj_free_val(c);
                p++;
                continue;
            }
            Obj_free_val(c);
            p = code[p].value.i->endif_p + 1;
        }
        if(code[p].type == inst_elif_t){
            Object condition = Obj_cpy(func_eval_Ast(code[p].value.el->condition));
            Object old_cond = condition;
            Object c = std_bool(&condition, 1);
            Obj_free_val(old_cond);
            if(c.val.b){
                Obj_free_val(c);
                p++;
                continue;
            }
            Obj_free_val(c);
            p = code[p].value.el->endif_p + 1;
        }

        else if(code[p].type == inst_else_t){
            p++;
        }
        else if (code[p].type == inst_jmp_t) {
            p = code[p].value.jmp;
            continue;
        }

        else if(code[p].type == inst_endif){
            p = code[p].value.endifelse + 1;
        }

        else if(code[p].type == inst_endifelse){
            p++;
        }

        else if(code[p].type == inst_return_t){
            free(loops);
            
            Object res = func_eval_Ast(code[p].value.ret);
            return res;
        }

        else if(code[p].type == inst_expr_t){
            Object x = Obj_cpy(func_eval_Ast(code[p].value.expr));
            Obj_free_val(x);
            p++;
        }

        else if(code[p].type == inst_section_t){
            p++;
        }

        else if(code[p].type == inst_goto_t){
            int n=-1;
            //search down
            for(int i = p + 1; i < len; i++){
                if(code[i].type == inst_section_t){
                    if(!strcmp(code[p].value.goto_sec, code[i].value.section)){
                        n = i;
                        break;
                    }
                }
            }
            if(n == -1){
                //search up
                for(int i = p - 1; i >= 0; i--){
                    if(code[i].type == inst_section_t){
                        if(!strcmp(code[p].value.goto_sec, code[i].value.section)){
                            n = i;
                            break;
                        }
                    }
                }
            }
            if(n == -1){
                printf("section %s doesnt exists\n", code[p].value.goto_sec);
                exit(1);
            }
            p = n;
            continue;
        }

        else if(code[p].type == inst_for_t){
            add_loop_count(p, &loops_count, &loops);
            Object start = Obj_cpy(func_eval_Ast(code[p].value.fo->start));
            Object old_start = start;
            start = std_ount(&start, 1);
            Obj_free_val(old_start);

            Object end = Obj_cpy(func_eval_Ast(code[p].value.fo->end));
            Object old_end = end;
            end = std_ount(&end, 1);
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
            for(int i = 0; i < STACK.MEM[STACK.len - 1].len; i++){
                if(!strcmp(STACK.MEM[STACK.len - 1].keys[i], code[p].value.fo->var_name)){
                    n = i;
                    break;
                }
            }
            if(n == -1){
                add_object(&(STACK.MEM[STACK.len - 1]), code[p].value.fo->var_name,start);
            }
            else{
                Obj_free_val(STACK.MEM[STACK.len - 1].values[n]);
                STACK.MEM[STACK.len - 1].values[n] = start;

            }
            if(start.val.i == end.val.i){
                Obj_free_val(end);
                p = code[p].value.fo->endfor + 1;
                remove_loop_count(&loops_count, &loops);
                continue;
            }
            Obj_free_val(end);
            p++;
        }

        else if(code[p].type == inst_endfor_t){
            int for_p = code[p].value.endfor;

            Object start = Obj_cpy(func_eval_Ast(code[for_p].value.fo->start));
            Object old_start = start;
            start = std_ount(&start, 1);
            Obj_free_val(old_start);

            Object end = Obj_cpy(func_eval_Ast(code[for_p].value.fo->end));
            Object old_end = end;
            end=std_ount(&end, 1);
            Obj_free_val(old_end);


            //positive for 
            if(start.val.i < end.val.i){
                int n = -1;
                for(int i = 0; i < STACK.MEM[STACK.len - 1].len; i++){
                    if(!strcmp(STACK.MEM[STACK.len - 1].keys[i],code[for_p].value.fo->var_name)){
                        n = i;
                        break;
                    }
                }
                if(n == -1){
                    p++;
                    Obj_free_val(end);
                    Obj_free_val(start);
                    continue;
                }
                Object old_obj = STACK.MEM[STACK.len - 1].values[n];
                STACK.MEM[STACK.len - 1].values[n] = std_ount(&STACK.MEM[STACK.len - 1].values[n], 1);
                Obj_free_val(old_obj);

                if(STACK.MEM[STACK.len - 1].values[n].type==Obj_nil_t){
                    printf("ERROR in for cant convert loop var '%s' to ount\n",code[for_p].value.fo->var_name);
                    exit(1);
                }
                STACK.MEM[STACK.len - 1].values[n].val.i++;

                if(end.val.i > STACK.MEM[STACK.len - 1].values[n].val.i){
                    Obj_free_val(end);
                    Obj_free_val(start);
                    p=for_p+1;
                    continue;
                }
                else{
                    Obj_free_val(end);
                    Obj_free_val(start);
                    p++;
                    remove_loop_count(&loops_count, &loops);
                    continue;
                }
            }
            else{
                int n = -1;
                for(int i = 0; i < STACK.MEM[STACK.len - 1].len; i++){
                    if(!strcmp(STACK.MEM[STACK.len - 1].keys[i],code[for_p].value.fo->var_name)){
                        n = i;
                        break;
                    }
                }
                if(n == -1){
                    p++;
                    Obj_free_val(end);
                    Obj_free_val(start);
                    continue;
                }
                Object old_obj = STACK.MEM[STACK.len - 1].values[n];
                STACK.MEM[STACK.len - 1].values[n] = std_ount(&STACK.MEM[STACK.len - 1].values[n], 1);
                Obj_free_val(old_obj);

                if(STACK.MEM[STACK.len - 1].values[n].type==Obj_nil_t){
                    printf("ERROR in for cant convert loop var '%s' to ount\n",code[for_p].value.fo->var_name);
                    exit(1);
                }
                STACK.MEM[STACK.len - 1].values[n].val.i--;

                if(end.val.i < STACK.MEM[STACK.len - 1].values[n].val.i){
                    Obj_free_val(end);
                    Obj_free_val(start);
                    p=for_p+1;
                    continue;
                }
                else{
                    Obj_free_val(end);
                    Obj_free_val(start);
                    p++;
                    continue;
                }
            }
            
        }

        else if(code[p].type == inst_while_t){
            add_loop_count(p, &loops_count, &loops);
            Object condition = Obj_cpy(func_eval_Ast(code[p].value.wh->condition));
            Object c = std_bool(&condition, 1);
            Obj_free_val(condition);
            if (c.val.b) {
                Obj_free_val(c);
                p++;
            } else {
                Obj_free_val(c);
                p = code[p].value.wh->endwhile + 1;
                remove_loop_count(&loops_count, &loops);
            }
        }

        else if (code[p].type == inst_endwhile_t){
            int while_p = code[p].value.endwhile;
            Object condition = Obj_cpy(func_eval_Ast(code[while_p].value.wh->condition));
            Object c = std_bool(&condition,1);
            Obj_free_val(condition);
            if (c.val.b) {
                Obj_free_val(c);
                p=code[p].value.endwhile+1;
            } else{
                Obj_free_val(c);
                remove_loop_count(&loops_count, &loops);
                p++;
            }
        }
        else if(code[p].type == inst_proceed_t){
            int index = loops[loops_count - 1];
            if(code[index].type == inst_while_t){
                p = code[index].value.wh->endwhile;
            }
            else if(code[index].type == inst_for_t){
                p = code[index].value.fo->endfor;
            }
            else{
                p = 0;
            }
        }
        else if(code[p].type == inst_stop_t){
            int index = loops[loops_count - 1];
            if(code[index].type == inst_while_t){
                p = code[index].value.wh->endwhile + 1;
            }
            else if(code[index].type == inst_for_t){
                p = code[index].value.fo->endfor + 1;
            }
            else{
                p = len;
            }
        }
        //TODO maybe one day implement this asqel
        else if(code[p].type==inst_funcdef_t){
            int n = -1;
            for (int i = 0; i < STACK.MEM[STACK.len - 1].len; i++) {
                if (!strcmp(code[p].value.fc->info.name, STACK.MEM[STACK.len - 1].keys[i])) {
                    n = i;
                    break;
                }
            }
            if (n == -1) {
                Object f;
                f.type = Obj_funcid_t;
                f.val.funcid = malloc(sizeof(Funcdef));
                f.val.funcid->name = uti_strdup(code[p].value.fc->info.name);
                f.val.funcid->is_builtin = 0;
                f.val.funcid->description = code[p].value.fc->info.description;
                f.val.funcid->defs_len = 1;
                f.val.funcid->defs = malloc(sizeof(sulfur_func));
                f.val.funcid->defs->args = code[p].value.fc->args;
                f.val.funcid->defs->args_len = code[p].value.fc->args_len;
                f.val.funcid->defs->args_mod = code[p].value.fc->args_mod;
                f.val.funcid->defs->code = code[p].value.fc->code;
                f.val.funcid->defs->code_len = code[p].value.fc->code_len;
                add_object(&STACK.MEM[STACK.len - 1], f.val.funcid->name, f);
            }
            p++;
            continue;
        }
        else {
            printf("ERROR in execute unknown instruction type %d\n", code[p].type);
            exit(1);
        }
    }
    free(loops);
    if (func.args_mod != 'A')
        remove_one_stack();
    return nil_Obj;
}