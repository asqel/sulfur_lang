#include "../include/func_evaluator.h"
#include "../include/memlib.h"
#include "../include/Ast.h"
#include "../include/operation.h"
#include "../sulfur_libs/blt_libs/std.h"
#include "../sulfur_libs/blt_libs/string_su.h"
#include "../sulfur_libs/blt_libs/funccall_su.h"
#include "../sulfur_libs/blt_libs/list_su.h"


extern Object execute(Instruction*code,char*file_name,int len);
extern Object func_execute(Funcdef* func, Object* argv, int argc, int create_stack);

extern memory MEMORY;
extern stack STACK;

Object func_eval_Ast(Ast*x){
    if(x->type==Ast_funccall_t){
        Object func = get_Obj_mem(STACK.MEM[STACK.len - 1], x->root.fun->name);
        
        if(func.type == Obj_not_found_t){
            func = get_Obj_mem(MEMORY, x->root.fun->name);
            if(func.type == Obj_not_found_t){
                printf("ERROR function '%s' not found\n",x->root.fun->name);
                exit(1);
            }
        }
        if(func.val.funcid->is_builtin){
            Object*a=malloc(sizeof(Object)*x->root.fun->nbr_arg);
            for(int i=0;i<x->root.fun->nbr_arg;i++){
                a[i]=func_eval_Ast(&x->root.fun->args[i]);

            }
            Object res = (*func.val.funcid->func_p)(a,x->root.fun->nbr_arg);
            Obj_free_array(a, x->root.fun->nbr_arg);
            return res;
        }
        else{
            Object*a=malloc(sizeof(Object)*x->root.fun->nbr_arg);

            for(int i = 0; i < x->root.fun->nbr_arg; i++){
                a[i] = func_eval_Ast(&x->root.fun->args[i]);

            }
            Object res = func_execute(func.val.funcid, a, x->root.fun->nbr_arg, 1);
            Obj_free_array(a, x->root.fun->nbr_arg);
            return res;
        }
    }
    if(x->type == Ast_anonym_func_t){
        Funcdef f;
        f.arg_names = NULL;
        f.nbr_of_args = 0;
        f.code = x->root.ano_func->code;
        f.code_len = x->root.ano_func->code_len;
        f.description = "";
        f.func_p = NULL;
        f.is_builtin = 0;
        return func_execute(&f, NULL, 0, 0);
    }
    if(x->left==NULL && x->right==NULL&& !x->isAst){
        if(x->type == Ast_varcall_t){
            Object val = get_Obj_mem(STACK.MEM[STACK.len - 1], x->root.varcall);
            if(val.type == Obj_not_found_t){
                val = get_Obj_mem(MEMORY, x->root.varcall);
                if(val.type == Obj_not_found_t){
                    printf("ERROR var '%s' not found\n");
                    exit(1);
                }
            }
            Object res = Obj_cpy(val);
            return res;
        }
        if(x->type == Ast_object_t){
            return Obj_cpy(*x->root.obj);
        }
        else{
            printf("ERROR in Ast type %d\n",x->type);
            exit(1);
        }
    }
    if(x->isAst){
        if(x->type==Ast_add_t){
            if(x->left!=NULL && x->right!=NULL){
                Object a=func_eval_Ast(x->left);
                Object b=func_eval_Ast(x->right);
                Object o=add(a,b);
                Obj_free_val(a);
                Obj_free_val(b);
                return o;
            }
        }
        if(x->type==Ast_le_t){
            Object a=func_eval_Ast(x->left);
            Object b=func_eval_Ast(x->right);
            Object o=less(a,b);
            Obj_free_val(a);
            Obj_free_val(b);
            return o;
            
        }
        if(x->type==Ast_ge_t){
            Object a=func_eval_Ast(x->left);
            Object b=func_eval_Ast(x->right);
            Object o=greater(a,b);
            Obj_free_val(a);
            Obj_free_val(b);
            return o;
            
        }
        if(x->type==Ast_plus_assign_t){
            Ast ast;
            ast.type = Ast_assign_t;
            ast.left = x->left;
            ast.right = malloc(sizeof(Ast));
            ast.right->type = Ast_add_t;
            ast.right->left = x->left;
            ast.right->right = x->right;
            Object o = func_eval_Ast(&ast);
            free(ast.right);
            return o;
            
        }
        if(x->type == Ast_minus_assign_t){
            Ast ast;
            ast.type = Ast_assign_t;
            ast.left = x->left;
            ast.right = malloc(sizeof(Ast));
            ast.right->type = Ast_sub_t;
            ast.right->left = x->left;
            ast.right->right = x->right;
            Object o = func_eval_Ast(&ast);
            free(ast.right);
            return o;
            
        }
        if(x->type==Ast_leq_t){
            Object a=func_eval_Ast(x->left);
            Object b=func_eval_Ast(x->right);
            Object o=less_eq(a,b);
            Obj_free_val(a);
            Obj_free_val(b);
            return o;
            
        }
        if(x->type==Ast_fldiv_t){
            Object a=func_eval_Ast(x->left);
            Object b=func_eval_Ast(x->right);
            Object o=fl_div(a,b);
            Obj_free_val(a);
            Obj_free_val(b);
            return o;
            
        }
        if(x->type==Ast_or_t){
            Object a=func_eval_Ast(x->left);
            Object b=func_eval_Ast(x->right);
            Object o=or(a,b);
            Obj_free_val(a);
            Obj_free_val(b);
            return o;
            
        }
        if(x->type==Ast_sub_t){
            if(x->left!=NULL && x->right!=NULL){
                Object a=func_eval_Ast(x->left);
                Object b=func_eval_Ast(x->right);
                Object o=sub(a,b);
                Obj_free_val(a);
                Obj_free_val(b);
                return o;
            }
            if(x->left==NULL && x->right!=NULL){
                Object b=func_eval_Ast(x->right);
                Object o= negate(b);
                Obj_free_val(b);
                return o;
            }
        }
        if(x->type==Ast_pow_t){
            Object a=func_eval_Ast(x->left);
            Object b=func_eval_Ast(x->right);
            Object o=_pow(a,b);
            Obj_free_val(a);
            Obj_free_val(b);
            return o;
            
        }
        if(x->type==Ast_eq_t){
            Object a=func_eval_Ast(x->left);
            Object b=func_eval_Ast(x->right);
            Object o=eq(a,b);
            Obj_free_val(a);
            Obj_free_val(b);
            return o;
            
        }
        if(x->type==Ast_noteq_t){
            Object a=func_eval_Ast(x->left);
            Object b=func_eval_Ast(x->right);
            Object o = not_eq(a,b);
            Obj_free_val(a);
            Obj_free_val(b);
            return o;
            
        }
        if(x->type==Ast_div_t){
            Object a=func_eval_Ast(x->left);
            Object b=func_eval_Ast(x->right);
            Object o=_div(a,b);
            Obj_free_val(a);
            Obj_free_val(b);
            return o;
            
        }
        if(x->type==Ast_mul_t){
            Object a=func_eval_Ast(x->left);
            Object b=func_eval_Ast(x->right);
            Object o=mul(a,b);
            Obj_free_val(a);
            Obj_free_val(b);
            return o;
        
        }
        if(x->type==Ast_geq_t){
            Object a=func_eval_Ast(x->left);
            Object b=func_eval_Ast(x->right);
            Object o=greater_eq(a,b);
            Obj_free_val(a);
            Obj_free_val(b);
            return o;
        
        }
        if(x->type==Ast_mod_t){
            Object a=func_eval_Ast(x->left);
            Object b=func_eval_Ast(x->right);
            Object o=mod(a,b);
            Obj_free_val(a);
            Obj_free_val(b);
            return o;
        }
        if(x->type==Ast_and_t){
            Object a=func_eval_Ast(x->left);
            Object b=func_eval_Ast(x->right);
            Object o=and(a,b);
            Obj_free_val(a);
            Obj_free_val(b);
            return o;
        }
        if(x->type==Ast_rshift_t){
            Object a=func_eval_Ast(x->left);
            Object b=func_eval_Ast(x->right);
            Object o=rshift(a,b);
            Obj_free_val(a);
            Obj_free_val(b);
            return o;
        }
        if(x->type==Ast_lshift_t){
            Object a=func_eval_Ast(x->left);
            Object b=func_eval_Ast(x->right);
            Object o=lshift(a,b);
            Obj_free_val(a);
            Obj_free_val(b);
            return o;
        }
        if(x->type==Ast_not_t){
            Object b=func_eval_Ast(x->right);
            Object o=not(b);
            Obj_free_val(b);
            return o;
        }
        if(x->type == Ast_assign_t){
            Object right = func_eval_Ast(x->right);
            if(x->left->type == Ast_varcall_t){
                for(int i = 0; i<STACK.MEM[STACK.len - 1].len; i++){
                    if(!strcmp(x->left->root.varcall, STACK.MEM[STACK.len - 1].keys[i])){
                        Object old = STACK.MEM[STACK.len - 1].values[i];
                        STACK.MEM[STACK.len - 1].values[i] = right;
                        return old;
                    }
                }
                for(int i = 0; i<MEMORY.len; i++){
                    if(!strcmp(x->left->root.varcall, MEMORY.keys[i])){
                        Object old = MEMORY.values[i];
                        MEMORY.values[i] = right;
                        return old;
                    }
                }
                add_object(&STACK.MEM[STACK.len - 1], x->left->root.varcall, right);
                return nil_Obj;
            }
            if(x->left->type != Ast_dot_t && x->left->type != Ast_colon_t){
                printf("ERROR in expression cannot assign %d\n",x->left->type);
                exit(1);
            }
            Ast* left = x->left;

            Object in_what = func_eval_Ast(left->left);
            if(in_what.type == obj_module_t){
                if(x->left->type != Ast_dot_t){
                    printf("ERROR in assign with module\n");
                    exit(1);
                }
                if(left->right->type == Ast_varcall_t){
                    char* name = left->right->root.varcall;
                    for(int i = 0; i < in_what.val.module->MEM->len; i++){
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
                    printf("ERROR cannot assign\n");
                }
            }
            if(in_what.type == Obj_list_t){
                if(x->left->type != Ast_colon_t){
                    printf("ERROR in assign with module\n");
                    exit(1);
                }
                Object index_obj = func_eval_Ast(left->right);
                Object old_index = index_obj;

                index_obj = std_ount(&index_obj,1);
                Obj_free_val(old_index);

                if(index_obj.type == Obj_nil_t){
                    printf("ERROR in assign index not ount\n");
                    exit(1);
                }
                int index = index_obj.val.i;
                if(!( -1 <= index && index < (in_what.val.li->elements[0].val.i))){
                    printf("ERROR cannot assign out of range\n");
                    exit(1);
                }
                Object old = in_what.val.li->elements[index + 1];
                in_what.val.li->elements[index + 1] = right;

                Obj_free_val(index_obj);
                Obj_free_val(in_what);
                return old;

            }
            else{
                printf("ERROR cannot assign\n");
                exit(1);
            }
        }
        if(x->type == Ast_dot_t){
            Object a = func_eval_Ast(x->left);

            if(a.type == obj_module_t){
                Object func = get_Obj_mem(*a.val.module->MEM, x->root.fun->name);
                if(func.type == Obj_not_found_t){
                    printf("ERROR function '%s' not found in module '%s'\n", x->root.fun->name, *a.val.module->filename);
                    exit(1);
                }
                if(func.val.funcid->is_builtin){
                    Object*arg = malloc(sizeof(Object) * x->root.fun->nbr_arg);
                    for(int i=0;i<x->root.fun->nbr_arg;i++){
                        arg[i]=func_eval_Ast(&x->root.fun->args[i]);

                    }
                    Object res = (*func.val.funcid->func_p)(arg, x->root.fun->nbr_arg);
                    Obj_free_array(arg, x->right->root.fun->nbr_arg);
                    Obj_free_val(a);
                    return res;
                }
                else{
                    Object*a=malloc(sizeof(Object)*x->root.fun->nbr_arg);
                    for(int i = 0; i < x->root.fun->nbr_arg; i++){
                        a[i] = func_eval_Ast(&x->root.fun->args[i]);

                    }
                    Object res = func_execute(func.val.funcid, a, x->root.fun->nbr_arg, 1);
                    Obj_free_array(a, x->root.fun->nbr_arg);
                    return res;
                }
            }
            if(a.type == Obj_string_t){
                if (x->right->type == Ast_funccall_t){
                    Object func = get_Obj_mem(*string_module.MEM, x->right->root.fun->name);
                    if (func.type == Obj_not_found_t){
                        printf("ERROR function '%s' not exist in methods of string\n",x->right->root.fun->name);
                        exit(1);
                    }
                    if(func.val.funcid->is_builtin){
                        if(x->right->root.fun->nbr_arg){
                            Object*arg=malloc(sizeof(Object) * (1 + x->right->root.fun->nbr_arg));
                            for(int i=0; i < x->right->root.fun->nbr_arg; i++){
                                arg[i + 1] = func_eval_Ast(&x->right->root.fun->args[i]);

                            }
                            arg[0] = Obj_cpy(a);
                            Object res = (*func.val.funcid->func_p)(arg,x->right->root.fun->nbr_arg +1 );
                            Obj_free_array(arg, 1 + x->right->root.fun->nbr_arg);

                            return res;
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
                        printf("ERROR function '%s' not exist in methods of funccall\n",x->right->root.fun->name);
                        exit(1);
                    }
                    if(func.val.funcid->is_builtin){
                        if(x->right->root.fun->nbr_arg){
                            Object*arg=malloc(sizeof(Object) * (1 + x->right->root.fun->nbr_arg));
                            for(int i=0; i < x->right->root.fun->nbr_arg; i++){
                                arg[i + 1] = func_eval_Ast(&x->right->root.fun->args[i]);

                            }
                            arg[0] = Obj_cpy(a);
                            Object res = (*func.val.funcid->func_p)(arg,x->right->root.fun->nbr_arg +1 );
                            Obj_free_array(arg, 1 + x->right->root.fun->nbr_arg);
                            return res;
                        }
                        else{
                            Object res = (*func.val.funcid->func_p)(&a,1);
                            Obj_free_val(a);
                            return res;
                        }
                    }

                }
            }
            if(a.type == Obj_list_t){
                if (x->right->type == Ast_funccall_t){
                    Object func = get_Obj_mem(*list_module.MEM, x->right->root.fun->name);
                    if (func.type == Obj_not_found_t){
                        printf("ERROR function '%s' not exist in methods of list\n",x->right->root.fun->name);
                        exit(1);
                    }
                    if(func.val.funcid->is_builtin){
                        if(x->right->root.fun->nbr_arg){
                            Object*arg = malloc(sizeof(Object) * (1 + x->right->root.fun->nbr_arg));
                            for(int i=0; i < x->right->root.fun->nbr_arg; i++){
                                arg[i + 1] = func_eval_Ast(&x->right->root.fun->args[i]);

                            }
                            arg[0] = a;
                            Object res = (*func.val.funcid->func_p)(arg,x->right->root.fun->nbr_arg +1 );
                            Obj_free_array(arg, 1 + x->right->root.fun->nbr_arg);
                            return res;
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
                printf("ERROR on dot operator\n");
                exit(1);
            }
        }
        if(x->type == Ast_colon_t){
            Object a = func_eval_Ast(x->left);
            if (a.type == Obj_list_t){
                Object b=func_eval_Ast(x->right);
                Object old_b = b;
                b = std_ount(&b, 1);

                Obj_free_val(old_b);
                if(b.type == Obj_nil_t){
                    printf("ERROR ':' only take ount convetible\n");
                    exit(1);
                }
                int index = b.val.i;
                if(!(-1 <= index && index < a.val.li->elements[0].val.i)){
                    printf("ERROR list out of range on ':'\n");
                    exit(1);
                }
                Object res = Obj_cpy(a.val.li->elements[1 + b.val.i]);
                Obj_free_val(a);
                Obj_free_val(b);
                return res;
            }
            if(a.type == Obj_string_t){
                Object b=func_eval_Ast(x->right);
                Object old_b = b;
                b = std_ount(&b, 1);

                Obj_free_val(old_b);
                if(b.type == Obj_nil_t){
                    printf("ERROR ':' only take ount convetible\n");
                    exit(1);
                }
                int index = b.val.i;
                int len = strlen(a.val.s);
                if(!(-1 <= index && index < len)){
                    printf("ERROR string out of range on ':'\n");
                    exit(1);
                }
                Object res;
                if(index != -1){
                    char s[2] = {a.val.s[index],'\0'};
                    res = new_string(s);
                }
                else{
                    res = new_ount(len);
                }
                Obj_free_val(a);
                Obj_free_val(b);
                return res;
            }
            else{
                printf("ERROR cannot use ':'\n");
                exit(1);
            }
        }
    }
}
