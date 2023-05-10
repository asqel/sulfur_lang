
#include "../include/memlib.h"
#include "../include/Ast.h"
#include "../include/operation.h"
#include "../sulfur_libs/blt_libs/std.h"
#include "../sulfur_libs/blt_libs/string_su.h"
#include "../sulfur_libs/blt_libs/funccall_su.h"

extern Object execute(Instruction*code,char*file_name,int len);

extern memory MEMORY;


Ast *eval_left_assign(Ast *x){
    if(x->type == Ast_dot_t){
        if (x->left->type == Ast_varcall_t){
            if(x->right->type == Ast_varcall_t){
            // TODO finish that i dont even know
            }
        }
    }
    else{
        printf("ERROR cannot assign value");
        exit(1);
    }
}

Object eval_Ast(Ast*x){
    if(x->type==Ast_funccall_t){
        int n=-1;
        
        for(int i=0;i<MEMORY.len;i++){

            if(!strcmp(MEMORY.keys[i],x->root.fun->name)&& MEMORY.values[i].type==Obj_funcid_t){
                n=i;
            }
        }
        if(n==-1){
            printf("function '%s' doesnt exit",x->root.fun->name);
            exit(1);
        }
        
        if(MEMORY.values[n].val.funcid->is_builtin){
            Object*a=malloc(sizeof(Object)*x->root.fun->nbr_arg);
            for(int i=0;i<x->root.fun->nbr_arg;i++){
                a[i]=eval_Ast(&x->root.fun->args[i]);

            }
            return (*MEMORY.values[n].val.funcid->func_p)(a,x->root.fun->nbr_arg);
        }
        else{
            return execute(MEMORY.values[n].val.funcid->code,"",MEMORY.values[n].val.funcid->code_len);
        }
    }
    if(x->type == Ast_anonym_func_t){
        return execute(x->root.ano_func->code,"",x->root.ano_func->code_len);
    }
    if(x->type == Ast_list_comprehension_t){
        Object start = eval_Ast(x->root.li->start);
        start = std_ount(&start, 1);
        Object end = eval_Ast(x->root.li->end);
        end = std_ount(&end, 1);

        if(start.type == Obj_nil_t){
            printf("ERROR in list comprehension cannot convert start to ount");
            exit(1);
        }
        if(end.type == Obj_nil_t){
            printf("ERROR in list comprehension cannot convert end to ount");
            exit(1);
        }
        int n = -1;
        for(int i = 0; i < MEMORY.len; i++){
            if(!strcmp(MEMORY.keys[i], x->root.li->varname)){
                n = i;
                break;
            }
        }
        if(n == -1){
            MEMORY = *add_object(&MEMORY, x->root.li->varname, new_ount(*start.val.i));
            n = MEMORY.len - 1;
        }
        else{
            MEMORY.values[n].type = Obj_ount_t;
            *MEMORY.values[n].val.i = *start.val.i;
        }

        if(*start.val.i < *end.val.i){
            Object* objects = malloc(sizeof(Object));
            int obj_len = 0;
            while(*MEMORY.values[n].val.i < *end.val.i){
                obj_len++;
                objects = realloc(objects, sizeof(Object));
                objects[obj_len -1] = eval_Ast(x->root.li->expr);

                MEMORY.values[n] = std_ount(&MEMORY.values[n],1);
                if(MEMORY.values[n].type == Obj_nil_t){
                    printf("ERROR in for in list comprehension");
                    exit(1);
                }
                (*MEMORY.values[n].val.i)++;
            }
            return std_list(objects, obj_len);
        }
        else if(*start.val.i > *end.val.i){

        }
        else{
            return std_list(NULL, 0);
        }

    }
    if(x->left==NULL && x->right==NULL&& !x->isAst){
        if(x->type == Ast_varcall_t){
            int n=-1;
            for(int i=0;i<MEMORY.len;i++){
                if(!strcmp(MEMORY.keys[i],x->root.varcall)){
                    n=i;
                }
            }
            if(n==-1){
                printf("ERROR var '%s' doesnt exist",x->root.varcall);
            }
            return Obj_cpy(MEMORY.values[n]);
        }
        if(x->type == Ast_object_t){
            return Obj_cpy(*(x->root.obj));
        }
        else{
            printf("ERROR in Ast");
            printf(" %d ",x->type);
            exit(-1);
        }
    }
    if(x->isAst){
        if(x->type==Ast_add_t){
            if(x->left!=NULL && x->right!=NULL){
                Object a=eval_Ast(x->left);
                Object b=eval_Ast(x->right);
                Object o=add(a,b);
                Obj_free_val(a);
                Obj_free_val(b);
                return o;
            }
        }
        if(x->type==Ast_le_t){
            Object a=eval_Ast(x->left);
            Object b=eval_Ast(x->right);
            Object o=less(a,b);
            Obj_free_val(a);
            Obj_free_val(b);
            return o;
            
        }
        if(x->type==Ast_ge_t){
            Object a=eval_Ast(x->left);
            Object b=eval_Ast(x->right);
            Object o=greater(a,b);
            Obj_free_val(a);
            Obj_free_val(b);
            return o;
            
        }
        if(x->type==Ast_sub_t){
            if(x->left!=NULL && x->right!=NULL){
                Object a=eval_Ast(x->left);
                Object b=eval_Ast(x->right);
                Object o=sub(a,b);
                Obj_free_val(a);
                Obj_free_val(b);
                return o;
            }
            if(x->left==NULL && x->right!=NULL){
                Object b=eval_Ast(x->right);
                Object o= negate(b);
                Obj_free_val(b);
                return o;
            }
        }
        if(x->type==Ast_pow_t){
            Object a=eval_Ast(x->left);
            Object b=eval_Ast(x->right);
            Object o=_pow(a,b);
            Obj_free_val(a);
            Obj_free_val(b);
            return o;
            
        }
        if(x->type==Ast_eq_t){
            Object a=eval_Ast(x->left);
            Object b=eval_Ast(x->right);
            Object o=eq(a,b);
            Obj_free_val(a);
            Obj_free_val(b);
            return o;
            
        }
        if(x->type==Ast_div_t){
            Object a=eval_Ast(x->left);
            Object b=eval_Ast(x->right);
            Object o=_div(a,b);
            Obj_free_val(a);
            Obj_free_val(b);
            return o;
            
        }
        if(x->type==Ast_mul_t){
            Object a=eval_Ast(x->left);
            Object b=eval_Ast(x->right);
            Object o=mul(a,b);
            Obj_free_val(a);
            Obj_free_val(b);
            return o;
        
        }
        if(x->type==Ast_mod_t){
            Object a=eval_Ast(x->left);
            Object b=eval_Ast(x->right);
            Object o=mod(a,b);
            Obj_free_val(a);
            Obj_free_val(b);
            return o;
        }
        if(x->type == Ast_assign_t){
            Object right = eval_Ast(x->right);
            if(x->left->type == Ast_varcall_t){
                for(int i = 0; i<MEMORY.len; i++){
                    if(!strcmp(x->left->root.varcall, MEMORY.keys[i])){
                        Object old = MEMORY.values[i];
                        MEMORY.values[i] = right;
                        return old;
                    }
                }
                add_object(&MEMORY, x->left->root.varcall, right);
                return nil_Obj;
            }
            if(x->left->type != Ast_dot_t && x->left->type != Ast_colon_t){
                printf("ERROR in expression cannot assign %d ",x->left->type);
                exit(1);
            }
            Ast* left = x->left;

            Object in_what = eval_Ast(left->left);
            if(in_what.type == obj_module_t){
                if(x->left->type != Ast_dot_t){
                    printf("ERROR in assign with module");
                    exit(1);
                }
                if(left->right->type == Ast_varcall_t){
                    char* name = left->right->root.varcall;
                    for(int i=0; i<in_what.val.module->MEM->len; i++){
                        if(!strcmp(name, in_what.val.module->MEM->keys[i])){
                            Object old =in_what.val.module->MEM->values[i];
                            in_what.val.module->MEM->values[i] = right;
                            return old;
                        }
                    }
                    add_Object_Module(in_what, name, right);
                    return nil_Obj;
                }   
                else{
                    printf("ERROR cannot assign");
                }
            }
            if(in_what.type == Obj_list_t){
                if(x->left->type != Ast_colon_t){
                    printf("ERROR in assign with module");
                    exit(1);
                }
                Object index_obj = eval_Ast(left->right);
                index_obj = std_ount(&index_obj,1);
                if(index_obj.type == Obj_nil_t){
                    printf("ERROR in assign index not ount");
                    exit(1);
                }
                int index = *index_obj.val.i;
                if(!( -1 < index && index < *(in_what.val.li->elements[0].val.i))){
                    printf("ERROR cannot assign out of range");
                    exit(1);
                }
                Object old = in_what.val.li->elements[index + 1];
                in_what.val.li->elements[index + 1] = right;
                return old;

            }
            else{
                printf("ERROR cannot assign");
                exit(1);
            }
        }
        if(x->type == Ast_dot_t){
            Object a=eval_Ast(x->left);
            temp_ref(a);
            if(a.type == obj_module_t){
                if (x->right->type == Ast_varcall_t){
                    for(int i=0; i<a.val.module->MEM->len; i++){
                        if(!strcmp(x->right->root.varcall, a.val.module->MEM->keys[i])){
                            return a.val.module->MEM->values[i];
                        }
                    }
                    printf("ERROR var not exit in mmodule");
                    exit(1);
                }
                else if (x->right->type == Ast_funccall_t){
                    Object j = nil_Obj;
                    for(int i=0; i<a.val.module->MEM->len; i++){
                        if(!strcmp(x->right->root.fun->name, a.val.module->MEM->keys[i])){
                            j = a.val.module->MEM->values[i];
                        }
                    }
                    if (j.type == Obj_nil_t){
                        printf("ERROR func not exist in module");
                        exit(1);
                    }
                    if(j.val.funcid->is_builtin){
                        if(x->right->root.fun->nbr_arg){
                            Object*arg=malloc(sizeof(Object)*x->right->root.fun->nbr_arg);
                            for(int i=0;i<x->right->root.fun->nbr_arg;i++){
                                arg[i]=eval_Ast(&x->right->root.fun->args[i]);

                            }
                            return (*j.val.funcid->func_p)(arg,x->right->root.fun->nbr_arg);
                        }
                        else{
                            return (*j.val.funcid->func_p)(NULL,0);
                        }
                    }

                }
            }
            if(a.type == Obj_string_t){
                if (x->right->type == Ast_funccall_t){
                    Object j = nil_Obj;
                    for(int i=0; i < string_module.MEM->len; i++){
                        if(!strcmp(x->right->root.fun->name, string_module.MEM->keys[i])){
                            j = string_module.MEM->values[i];
                        }
                    }
                    if (j.type == Obj_nil_t){
                        printf("ERROR func not exist in methods of string");
                        exit(1);
                    }
                    if(j.val.funcid->is_builtin){
                        if(x->right->root.fun->nbr_arg){
                            Object*arg=malloc(sizeof(Object) * (1 + x->right->root.fun->nbr_arg));
                            for(int i=1;i<x->right->root.fun->nbr_arg + 1;i++){
                                arg[i]=eval_Ast(&x->right->root.fun->args[i-1]);

                            }
                            arg[0] = a;
                            return (*j.val.funcid->func_p)(arg,x->right->root.fun->nbr_arg);
                        }
                        else{
                            return (*j.val.funcid->func_p)(&a,1);
                        }
                    }

                }
                
            }
            if(a.type == Obj_funcid_t){
                if (x->right->type == Ast_funccall_t){
                    Object j = nil_Obj;
                    for(int i=0; i < funccall_module.MEM->len; i++){
                        if(!strcmp(x->right->root.fun->name, funccall_module.MEM->keys[i])){
                            j = funccall_module.MEM->values[i];
                        }
                    }
                    if (j.type == Obj_nil_t){
                        printf("ERROR func not exist in methods of funccall");
                        exit(1);
                    }
                    if(j.val.funcid->is_builtin){
                        if(x->right->root.fun->nbr_arg){
                            Object*arg=malloc(sizeof(Object) * (1 + x->right->root.fun->nbr_arg));
                            for(int i=1;i<x->right->root.fun->nbr_arg + 1;i++){
                                arg[i]=eval_Ast(&x->right->root.fun->args[i-1]);

                            }
                            arg[0] = a;
                            return (*j.val.funcid->func_p)(arg,x->right->root.fun->nbr_arg);
                        }
                        else{
                            return (*j.val.funcid->func_p)(&a,1);
                        }
                    }

                }
            }
            else{
                printf("ERROR on dot operator");
                exit(1);
            }
        }
        if(x->type == Ast_colon_t){
            Object a=eval_Ast(x->left);
            temp_ref(a);
            if (a.type == Obj_list_t){
                Object b=eval_Ast(x->right);
                if(b.type != Obj_ount_t){
                    printf("error dot list nort int");
                    exit(1);
                }
                return Obj_cpy(a.val.li->elements[1 + *b.val.i]);
            }
        }
    }
}


/*


*/