#include "../include/interpreter.h"
#include "../include/instruction.h"
#include <stdlib.h>

#include "../sulfur_libs/blt_libs/std.h"
#include "../sulfur_libs/blt_libs/string_su.h"
#include "../sulfur_libs/blt_libs/funccall_su.h"
#include "../sulfur_libs/blt_libs/list_su.h"

#include "../include/sulfur.h"
#include "../include/operation.h"
#include "../include/utilities.h"
#include "../include/token_class.h"
#include "../include/evaluator.h"

#define NO_GARBAGE

memory MEMORY;

ref_count* REFS;
int REFS_len;

Instruction *current_instructions = NULL;
int *current_index = NULL;
int instruction_len = 0;

Instruction **__instructions_ptr = NULL;

Object add_module_mem(Object (*loader)(Sulfur_ctx), char* name, char* as){
    if(as != NULL){
        Object o = (*loader)(CTX);
        if(o.type !=obj_module_t){
            printf("ERROR in loading module '%s' as '%s', value return by loader incorrect\n", name, as);
            exit(1);
        }
        add_object(&MEMORY, as, o);
    }
    else{
        Object o=(*loader)(CTX);
        if(o.type !=obj_module_t){
            printf("ERROR in loading module %s , value return by loader incorrect\n",name);
            exit(1);
        }
        for(int i=0;i<o.val.module->MEM->len;i++){
            add_object(&MEMORY,o.val.module->MEM->keys[i],o.val.module->MEM->values[i]);
        }
    }
}

Object import_func(Object*arg,int argc){
    if (argc>2){
        printf("ERROR in import maximum 2 arguments\n");
        exit(1);
    }  
    for (int i = 0; i < argc; i++){
        if (arg[i].type != Obj_string_t){
            printf("ERROR in import only string arguments accepted\n");
            exit(1);
        }
    }
    if(argc == 1){
        if (!id_acceptable_ptr(arg[0].val.s)){
            printf("ERROR cannot import file with space in its name (%s)\n",arg[0].val.s);
            printf("use second argument to import as\n");
            exit(1);
        }
        add_module_mem(get_module_loader(arg[0].val.s), arg[0].val.s, arg[0].val.s);
    }
    if (argc==2){
        if (strcmp(arg[1].val.s,"") && !id_acceptable_ptr(arg[1].val.s)){
            printf("ERROR cannot import file as if alias contains space (%s)\n",arg[1].val.s);
            exit(1);
        }
        if (strcmp(arg[1].val.s,"")){
            add_module_mem(get_module_loader(arg[0].val.s), arg[0].val.s, arg[1].val.s);
        }
        else{
            add_module_mem(get_module_loader(arg[0].val.s), arg[0].val.s, NULL);
        }
    }
    return nil_Obj;
}


void init_memory(){
    MEMORY.values=malloc(sizeof(Object));
    MEMORY.keys=malloc(sizeof(char *));
    MEMORY.len=0;

    REFS = malloc(sizeof(ref_count));
    REFS_len = 1; // set to one to avoid realloc(x,0) when there is no lists
}

void init_libs(char*path){
    MEMORY = init_std(MEMORY, path);
    add_func(&MEMORY, "import", &import_func, ""); 
    add_object(&MEMORY, "_string", init_string(&MEMORY, path));
    add_object(&MEMORY, "_list", init_list(&MEMORY, path));
    add_object(&MEMORY, "_funccall", init_funccall(&MEMORY, path));
}

void add_loop_count(int index, int *loops_count, int **loops){
    (*loops_count)++;
    (*loops) = realloc(*loops, sizeof(int) * (*loops_count));
    (*loops)[*loops_count - 1] = index;
}

void remove_loop_count(int *loops_count, int **loops){
    (*loops_count)--;
    (*loops) = realloc(*loops, sizeof(int) * (*loops_count));
}

Object execute(Instruction* code, char* file_name, int len){
    int p = 0;
    int *loops = malloc(sizeof(int));
    int loops_count = 0; 
    current_instructions = code;
    current_index = &p;
    instruction_len = len;
    while(p < len){
        if(code[p].type == inst_pass_t){
            p++;
            continue;
        }
        
        if(code[p].type == inst_if_t){
            Object condition = eval_Ast(code[p].value.i->condition);
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
            continue;
        }
        if(code[p].type == inst_elif_t){
            Object condition = eval_Ast(code[p].value.el->condition);
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
            continue;
        }
        if(code[p].type == inst_else_t){
            p++;
            continue;
        }
        if(code[p].type == inst_endif){
            p = code[p].value.endifelse + 1;
            continue;

        }
        if(code[p].type == inst_endifelse){
            p++;
            continue;
        }
        if(code[p].type == inst_return_t){
            return eval_Ast(code[p].value.ret);
        }

        if(code[p].type == inst_expr_t){
            Object x = eval_Ast(code[p].value.expr);
            Obj_free_val(x);
            p++;
            continue;
        }
        if(code[p].type == inst_section_t){
            p++;
        }
        if(code[p].type == inst_goto_t){
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
        if(code[p].type == inst_for_t){
            add_loop_count(p, &loops_count, &loops);
            Object start = eval_Ast(code[p].value.fo->start);
            Object old_start = start;
            start = std_ount(&start, 1);
            Obj_free_val(old_start);

            Object end=eval_Ast(code[p].value.fo->end);
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
            for(int i = 0; i < MEMORY.len; i++){
                if(!strcmp(MEMORY.keys[i], code[p].value.fo->var_name)){
                    n = i;
                    break;
                }
            }
            if(n == -1){
                add_object(&MEMORY, code[p].value.fo->var_name,start);
            }
            else{
                Obj_free_val(MEMORY.values[n]);
                MEMORY.values[n] = start;

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
        if(code[p].type == inst_endfor_t){
            int for_p = code[p].value.endfor;

            Object start = eval_Ast(code[for_p].value.fo->start);
            Object old_start = start;
            start = std_ount(&start, 1);
            Obj_free_val(old_start);

            Object end=eval_Ast(code[for_p].value.fo->end);
            Object old_end = end;
            end=std_ount(&end, 1);
            Obj_free_val(old_end);


            //positive for 
            if(start.val.i < end.val.i){
                int n = -1;
                for(int i = 0; i < MEMORY.len; i++){
                    if(!strcmp(MEMORY.keys[i],code[for_p].value.fo->var_name)){
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
                Object old_obj = MEMORY.values[n];
                MEMORY.values[n] = std_ount(&MEMORY.values[n], 1);
                Obj_free_val(old_obj);

                if(MEMORY.values[n].type==Obj_nil_t){
                    printf("ERROR in for cant convert loop var '%s' to ount\n",code[for_p].value.fo->var_name);
                    exit(1);
                }
                MEMORY.values[n].val.i++;

                if(end.val.i > MEMORY.values[n].val.i){
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
                for(int i = 0; i < MEMORY.len; i++){
                    if(!strcmp(MEMORY.keys[i],code[for_p].value.fo->var_name)){
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
                Object old_obj = MEMORY.values[n];
                MEMORY.values[n] = std_ount(&MEMORY.values[n], 1);
                Obj_free_val(old_obj);

                if(MEMORY.values[n].type==Obj_nil_t){
                    printf("ERROR in for cant convert loop var '%s' to ount\n",code[for_p].value.fo->var_name);
                    exit(1);
                }
                MEMORY.values[n].val.i--;

                if(end.val.i < MEMORY.values[n].val.i){
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
        if(code[p].type == inst_while_t){
            add_loop_count(p, &loops_count, &loops);
            Object condition = eval_Ast(code[p].value.wh->condition);
            Object c = std_bool(&condition, 1);
            Obj_free_val(condition);
            if(c.val.b){
                Obj_free_val(c);
                p++;
                continue;
            }
            else{
                Obj_free_val(c);
                p = code[p].value.wh->endwhile + 1;
                remove_loop_count(&loops_count, &loops);
                continue;
            }
        }
        if(code[p].type == inst_endwhile_t){
            int while_p = code[p].value.endwhile;
            Object condition = eval_Ast(code[while_p].value.wh->condition);
            Object c=std_bool(&condition,1);
            Obj_free_val(condition);
            if(c.val.b){
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
            if(n == -1){
                Object f;
                f.type = Obj_funcid_t;
                f.val.funcid = malloc(sizeof(Funcdef));
                f.val.funcid->code = code[p].value.fc->code;
                f.val.funcid->code_len = code[p].value.fc->code_len;
                f.val.funcid->is_builtin = 0;
                f.val.funcid->arg_names = code[p].value.fc->args;
                f.val.funcid->nbr_of_args = code[p].value.fc->args_len;
                add_object(&MEMORY, code[p].value.fc->info.name, f);
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
    free(loops);
    return nil_Obj;
}