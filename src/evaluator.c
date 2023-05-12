
#include "../include/memlib.h"
#include "../include/Ast.h"
#include "../include/operation.h"
#include "../sulfur_libs/blt_libs/std.h"
#include "../sulfur_libs/blt_libs/string_su.h"
#include "../sulfur_libs/blt_libs/funccall_su.h"

extern Object execute(Instruction*code,char*file_name,int len);

extern memory MEMORY;

Object eval_Ast(Ast*x){
    if(x->type==Ast_funccall_t){
        Object func = get_Obj_mem(MEMORY, x->root.fun->name);
        if(func.type == Obj_not_found_t){
            printf("ERROR function '%s' not found",x->root.fun->name);
            exit(1);
        }
        if(func.val.funcid->is_builtin){
            Object*a=malloc(sizeof(Object)*x->root.fun->nbr_arg);
            for(int i=0;i<x->root.fun->nbr_arg;i++){
                a[i]=eval_Ast(&x->root.fun->args[i]);

            }
            Object res = (*func.val.funcid->func_p)(a,x->root.fun->nbr_arg);
            for(int i = 0; i < x->root.fun->nbr_arg; i++){
                Obj_free_val(a[i]);
            }
            free(a);
            return res;
        }
        else{
            return execute(func.val.funcid->code,"",func.val.funcid->code_len);
        }
    }
    if(x->type == Ast_anonym_func_t){
        return execute(x->root.ano_func->code,"",x->root.ano_func->code_len);
    }
    if(x->left==NULL && x->right==NULL&& !x->isAst){
        if(x->type == Ast_varcall_t){
            Object val = get_Obj_mem(MEMORY, x->root.varcall);
            if(val.type == Obj_not_found_t){
                printf("ERROR var '%s' not found");
                exit(1);
            }
            return Obj_cpy(val);
        }
        if(x->type == Ast_object_t){
            return Obj_cpy(*x->root.obj);
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
                            Obj_free_val(in_what);
                            return old;
                        }
                    }
                    Obj_free_val(in_what);
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
                Object old_index = index_obj;

                index_obj = std_ount(&index_obj,1);
                Obj_free_val(old_index);

                if(index_obj.type == Obj_nil_t){
                    printf("ERROR in assign index not ount");
                    exit(1);
                }
                int index = *index_obj.val.i;
                if(!( -1 <= index && index < *(in_what.val.li->elements[0].val.i))){
                    printf("ERROR cannot assign out of range");
                    exit(1);
                }
                Object old = in_what.val.li->elements[index + 1];
                in_what.val.li->elements[index + 1] = right;

                Obj_free_val(index_obj);
                Obj_free_val(in_what);
                return old;

            }
            else{
                printf("ERROR cannot assign");
                exit(1);
            }
        }
        if(x->type == Ast_dot_t){
            Object a = eval_Ast(x->left);

            if(a.type == obj_module_t){
                Object func = get_Obj_mem(*a.val.module->MEM, x->root.fun->name);
                if(func.type == Obj_not_found_t){
                    printf("ERROR function '%s' not found in module '%s'", x->root.fun->name, *a.val.module->filename);
                    exit(1);
                }
                if(func.val.funcid->is_builtin){
                    Object*arg = malloc(sizeof(Object) * x->root.fun->nbr_arg);
                    for(int i=0;i<x->root.fun->nbr_arg;i++){
                        arg[i]=eval_Ast(&x->root.fun->args[i]);

                    }
                    Object res = (*func.val.funcid->func_p)(arg, x->root.fun->nbr_arg);
                    for(int i = 0; i < x->root.fun->nbr_arg; i++){
                        Obj_free_val(arg[i]);
                    }
                    free(arg);
                    Obj_free_val(a);
                    return res;
                }
                else{
                    return execute(func.val.funcid->code,"",func.val.funcid->code_len);
                }
            }
            if(a.type == Obj_string_t){
                if (x->right->type == Ast_funccall_t){
                    Object func = get_Obj_mem(*string_module.MEM, x->right->root.fun->name);
                    if (func.type == Obj_not_found_t){
                        printf("ERROR function '%s' not exist in methods of string",x->right->root.fun->name);
                        exit(1);
                    }
                    if(func.val.funcid->is_builtin){
                        if(x->right->root.fun->nbr_arg){
                            Object*arg=malloc(sizeof(Object) * (1 + x->right->root.fun->nbr_arg));
                            for(int i=0; i < x->right->root.fun->nbr_arg; i++){
                                arg[i + 1] = eval_Ast(&x->right->root.fun->args[i]);

                            }
                            arg[0] = a;
                            Object res = (*func.val.funcid->func_p)(arg,x->right->root.fun->nbr_arg +1 );
                            for(int i=0; i < 1 + x->right->root.fun->nbr_arg; i++){
                                Obj_free_val(arg[i]);
                            }
                            free(arg);
                        }
                        else{
                            Object res = (*func.val.funcid->func_p)(&a,1);
                            Obj_free_val(a);
                            return res;
                        }
                    }

                }
                
            }
            if(a.type == Obj_funcid_t){
                if (x->right->type == Ast_funccall_t){
                    Object func = get_Obj_mem(*funccall_module.MEM, x->right->root.fun->name);
                    if (func.type == Obj_not_found_t){
                        printf("ERROR function '%s' not exist in methods of funccall",x->right->root.fun->name);
                        exit(1);
                    }
                    if(func.val.funcid->is_builtin){
                        if(x->right->root.fun->nbr_arg){
                            Object*arg=malloc(sizeof(Object) * (1 + x->right->root.fun->nbr_arg));
                            for(int i=0; i < x->right->root.fun->nbr_arg; i++){
                                arg[i + 1] = eval_Ast(&x->right->root.fun->args[i]);

                            }
                            arg[0] = a;
                            Object res = (*func.val.funcid->func_p)(arg,x->right->root.fun->nbr_arg +1 );
                            for(int i=0; i < 1 + x->right->root.fun->nbr_arg; i++){
                                Obj_free_val(arg[i]);
                            }
                            free(arg);
                        }
                        else{
                            Object res = (*func.val.funcid->func_p)(&a,1);
                            Obj_free_val(a);
                            return res;
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
            Object a = eval_Ast(x->left);
            if (a.type == Obj_list_t){
                Object b=eval_Ast(x->right);
                Object old_b = b;
                b = std_ount(&b, 1);

                Obj_free_val(old_b);
                if(b.type == Obj_nil_t){
                    printf("ERROR ':' only take ount convetible");
                    exit(1);
                }
                int index = *b.val.i;
                if(!(-1 <= index && index < *a.val.li->elements[0].val.i)){
                    printf("ERROR list out of range on ':'");
                    exit(1);
                }
                Object res = Obj_cpy(a.val.li->elements[1 + *b.val.i]);
                Obj_free_val(a);
                return res;
            }
            else{
                printf("cannot use ':' ");
                exit(1);
            }
        }
    }
}
