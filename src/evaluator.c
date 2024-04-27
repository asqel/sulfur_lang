
#include "../include/memlib.h"
#include "../include/evaluator.h"
#include "../include/Ast.h"
#include "../include/operation.h"
#include "../include/func_interpreter.h"
#include "../sulfur_libs/blt_libs/std.h"
#include "../sulfur_libs/blt_libs/string_su.h"
#include "../sulfur_libs/blt_libs/funccall_su.h"
#include "../sulfur_libs/blt_libs/list_su.h"
#include "../sulfur_libs/blt_libs/complex.h"

extern Object execute(Instruction*code,char*file_name,int len);

extern memory MEMORY;

extern void remove_one_stack();

Object eval_Ast(Ast*x){
    if(x->type==Ast_funccall_t){
        Object func = get_Obj_mem(MEMORY, x->root.fun->name);

        if(func.type == Obj_not_found_t){
            printf("ERROR function '%s' not found\n",x->root.fun->name);
            exit(1);
        }
        if(func.type != Obj_funcid_t){
            if (func.type == obj_module_t) {
                func = get_Obj_mem(*func.val.module->MEM, "__call");
                if(func.type == Obj_not_found_t){
                    printf("ERROR function '%s' not found\n",x->root.fun->name);
                    exit(1);
                }
            }
            else {
                printf("ERROR can't call a non function Object '%s'\n",x->root.fun->name);
                exit(1);
            }
        }
        if(func.val.funcid->is_builtin){
            int args_len = 0;
            
            Object *args = eval_args(x->root.fun->args, x->root.fun->nbr_arg, &args_len);
            Object res = (*func.val.funcid->func_p)(args, args_len);
            Obj_free_array(args, args_len);
            return res;
        }
        else{
            int args_len = 0;
            Object *args = eval_args(x->root.fun->args, x->root.fun->nbr_arg, &args_len);
            Object res = eval_func(args, args_len, *func.val.funcid);

            if (res.type == Obj_list_t)
                add_protected(res.val.li);
            if (res.type == Obj_funcid_t) 
                add_protected(res.val.funcid);
            if (res.type == obj_module_t) 
                add_protected(res.val.module);
            remove_one_stack();
            Obj_free_array(args, args_len);
            return res;
        }
    }

    if(x->type == Ast_anonym_func_t){
        return execute(x->root.ano_func->code,"",x->root.ano_func->code_len);
    }

    if(x->left==NULL && x->right==NULL&& !x->isAst){
        if(x->type == Ast_varcall_t){
            Object val = get_Obj_mem(MEMORY, x->root.varcall);
            if(val.type == Obj_not_found_t){
                printf("ERROR var '%s' not found\n", x->root.varcall);
                exit(1);
            }
            return val;
        }
        if(x->type == Ast_object_t){
            return *x->root.obj;
        }
        else{
            printf("ERROR in Ast\n");
            exit(1);
        }
    }
    if(x->isAst){
        if(x->type==Ast_add_t){
            if(x->left!=NULL && x->right!=NULL){
                Object a = Obj_cpy(eval_Ast(x->left));
                Object b = Obj_cpy(eval_Ast(x->right));
                Object o = add(a,b);
                Obj_free_val(a);
                Obj_free_val(b);
                return o;
            }
        }
        if(x->type==Ast_le_t){
            Object a = Obj_cpy(eval_Ast(x->left));
            Object b = Obj_cpy(eval_Ast(x->right));
            Object o = less(a,b);
            Obj_free_val(a);
            Obj_free_val(b);
            return o;
            
        }
        if(x->type==Ast_ge_t){
            Object a = Obj_cpy(eval_Ast(x->left));
            Object b = Obj_cpy(eval_Ast(x->right));
            Object o = greater(a,b);
            Obj_free_val(a);
            Obj_free_val(b);
            return o;
            
        }
        if(x->type==Ast_plus_assign_t){
            Ast ast;
            ast.isAst = 1;
            ast.type = Ast_assign_t;
            ast.left = x->left;
            ast.right = malloc(sizeof(Ast));
            ast.right->type = Ast_add_t;
            ast.right->left = x->left;
            ast.right->right = x->right;
            ast.right->isAst = 1;
            Object o = Obj_cpy(eval_Ast(&ast));
            free(ast.right);
            return o;
            
        }
        if(x->type == Ast_minus_assign_t){
            Ast ast;
            ast.isAst = 1;
            ast.type = Ast_assign_t;
            ast.left = x->left;
            ast.right = malloc(sizeof(Ast));
            ast.right->type = Ast_sub_t;
            ast.right->left = x->left;
            ast.right->right = x->right;
            Object o = Obj_cpy(eval_Ast(&ast));
            free(ast.right);
            return o;
        }
        if(x->type==Ast_leq_t){
            Object a = Obj_cpy(eval_Ast(x->left));
            Object b = Obj_cpy(eval_Ast(x->right));
            Object o = less_eq(a,b);
            Obj_free_val(a);
            Obj_free_val(b);
            return o;
            
        }
        if(x->type==Ast_fldiv_t){
            Object a = Obj_cpy(eval_Ast(x->left));
            Object b = Obj_cpy(eval_Ast(x->right));
            Object o = fl_div(a,b);
            Obj_free_val(a);
            Obj_free_val(b);
            return o;
            
        }
        if(x->type==Ast_or_t){
            Object a = Obj_cpy(eval_Ast(x->left));
            Object new_a = std_bool(&a, 1);
            if (new_a.val.b) {
                Obj_free_val(a);
                return new_boolean(1);
            }
            Object b = Obj_cpy(eval_Ast(x->right));
            Object new_b = std_bool(&b, 1);
            if (new_b.val.b) {
                Obj_free_val(b);
                return new_boolean(1);
            }
            
            return new_boolean(0);
            
        }
        if(x->type==Ast_sub_t){
            if(x->left!=NULL && x->right!=NULL){
                Object a = Obj_cpy(eval_Ast(x->left));
                Object b = Obj_cpy(eval_Ast(x->right));
                Object o = sub(a,b);
                Obj_free_val(a);
                Obj_free_val(b);
                return o;
            }
            if(x->left==NULL && x->right!=NULL){
                Object b = Obj_cpy(eval_Ast(x->right));
                Object o = negate(b);
                Obj_free_val(b);
                return o;
            }
        }
        if(x->type==Ast_pow_t){
            Object a = Obj_cpy(eval_Ast(x->left));
            Object b = Obj_cpy(eval_Ast(x->right));
            Object o = _pow(a,b);
            Obj_free_val(a);
            Obj_free_val(b);
            return o;
            
        }
        if(x->type==Ast_eq_t){
            Object a = Obj_cpy(eval_Ast(x->left));
            Object b = Obj_cpy(eval_Ast(x->right));
            Object o = eq(a,b);
            Obj_free_val(a);
            Obj_free_val(b);
            return o;
            
        }
        if(x->type==Ast_noteq_t){
            Object a = Obj_cpy(eval_Ast(x->left));
            Object b = Obj_cpy(eval_Ast(x->right));
            Object o = not_eq(a,b);
            Obj_free_val(a);
            Obj_free_val(b);
            return o;
            
        }
        if(x->type==Ast_div_t){
            Object a = Obj_cpy(eval_Ast(x->left));
            Object b = Obj_cpy(eval_Ast(x->right));
            Object o = _div(a,b);
            Obj_free_val(a);
            Obj_free_val(b);
            return o;
            
        }
        if(x->type==Ast_mul_t){
            Object a = Obj_cpy(eval_Ast(x->left));
            Object b = Obj_cpy(eval_Ast(x->right));
            Object o = mul(a,b);
            Obj_free_val(a);
            Obj_free_val(b);
            return o;
        
        }
        if(x->type==Ast_geq_t){
            Object a = Obj_cpy(eval_Ast(x->left));
            Object b = Obj_cpy(eval_Ast(x->right));
            Object o = greater_eq(a,b);
            Obj_free_val(a);
            Obj_free_val(b);
            return o;
        
        }
        if(x->type==Ast_mod_t){
            Object a = Obj_cpy(eval_Ast(x->left));
            Object b = Obj_cpy(eval_Ast(x->right));
            Object o = mod(a,b);
            Obj_free_val(a);
            Obj_free_val(b);
            return o;
        }
        if(x->type==Ast_and_t){
            Object a = Obj_cpy(eval_Ast(x->left));
            Object new_a = std_bool(&a, 1);
            if (!new_a.val.b) {
                Obj_free_val(a);
                return new_boolean(0);
            }
            Object b = Obj_cpy(eval_Ast(x->right));
            Object new_b = std_bool(&b, 1);
            if (!new_b.val.b) {
                Obj_free_val(b);
                return new_boolean(0);
            }
            
            return new_boolean(1);
        }
        if(x->type==Ast_rshift_t){
            Object a = Obj_cpy(eval_Ast(x->left));
            Object b = Obj_cpy(eval_Ast(x->right));
            Object o = rshift(a,b);
            Obj_free_val(a);
            Obj_free_val(b);
            return o;
        }
        if(x->type==Ast_lshift_t){
            Object a = Obj_cpy(eval_Ast(x->left));
            Object b = Obj_cpy(eval_Ast(x->right));
            Object o = lshift(a,b);
            Obj_free_val(a);
            Obj_free_val(b);
            return o;
        }
        if(x->type==Ast_bit_xor_t){
            Object a = Obj_cpy(eval_Ast(x->left));
            Object b = Obj_cpy(eval_Ast(x->right));
            Object o = xor_op(a,b);
            Obj_free_val(a);
            Obj_free_val(b);
            return o;
        }
        if(x->type==Ast_not_t){
            Object b = Obj_cpy(eval_Ast(x->right));
            Object o = not(b);
            Obj_free_val(b);
            return o;
        }
        if(x->type == Ast_unpack_t){
            printf("ERROR cannot unpack here\n");
            exit(1);
        }
        if(x->type == Ast_assign_t){
            Object right = eval_Ast(x->right);
            if(x->left->type == Ast_varcall_t){
                for(int i = 0; i<MEMORY.len; i++){
                    if(!strcmp(x->left->root.varcall, MEMORY.keys[i])){
                        Object old = MEMORY.values[i];
                        MEMORY.values[i] = Obj_cpy(right);
                        return old;
                    }
                }
                add_object_cpy(&MEMORY, x->left->root.varcall, right);
                return nil_Obj;
            }
            if(x->left->type != Ast_dot_t && x->left->type != Ast_colon_t){
                printf("ERROR in expression cannot assign %d\n",x->left->type);
                exit(1);
            }
            Ast* left = x->left;

            Object in_what = Obj_cpy(eval_Ast(left->left));
            if(in_what.type == obj_module_t){
                if(x->left->type != Ast_dot_t){// check a.b = right
                    printf("ERROR in assign with module\n");
                    exit(1);
                }
                if(left->right->type == Ast_varcall_t){
                    char* name = left->right->root.varcall;
                    for(int i=0; i<in_what.val.module->MEM->len; i++){
                        if(!strcmp(name, in_what.val.module->MEM->keys[i])){
                            Object old =in_what.val.module->MEM->values[i];
                            in_what.val.module->MEM->values[i] = Obj_cpy(right);
                            Obj_free_val(in_what);
                            return old;
                        }
                    }
                    Obj_free_val(in_what);
                    add_Object_Module_cpy(in_what, name, right);
                    return nil_Obj;
                }   
                else{
                    printf("ERROR cannot assign\n");
                }
            }
            if(in_what.type == Obj_list_t){
                if(x->left->type != Ast_colon_t){
                    printf("ERROR in assign with list\n");
                    exit(1);
                }
                Object index_obj = Obj_cpy(eval_Ast(left->right));
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
                in_what.val.li->elements[index + 1] = Obj_cpy(right);

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
            Object a = eval_Ast(x->left);
            if(a.type == obj_module_t){
                if (x->right->type == Ast_funccall_t){
                    Object func = get_Obj_mem(*a.val.module->MEM, x->right->root.fun->name);
                    if (func.type == Obj_not_found_t){
                        printf("ERROR function '%s' not exist in methods of list\n",x->right->root.fun->name);
                        exit(1);
                    }
                    if (func.type != Obj_funcid_t) {
                        if (func.type == obj_module_t) {
                            func = get_Obj_mem(*func.val.module->MEM, "__call");
                            if(func.type == Obj_not_found_t){
                                printf("ERROR function '%s' not found\n",x->root.fun->name);
                                exit(1);
                            }
                        }
                        else {
                            printf("ERROR function '%s' not exist in methods of list\n",x->right->root.fun->name);
                            exit(1);
                        }
                    }
                    if(func.val.funcid->is_builtin){
                        if(x->right->root.fun->nbr_arg){
                            int args_len = 0;
                            Object *args = eval_args(x->right->root.fun->args, x->right->root.fun->nbr_arg, &args_len);
                            Object res = (*func.val.funcid->func_p)(args, args_len);
                            Obj_free_array(args, args_len);
                            return res;
                        }
                        else{
                            Object res = (*func.val.funcid->func_p)(NULL,0);
                            return res;
                        }
                    }
                    else{
                        int args_len = 0;
                        Object *args = eval_args(x->root.fun->args, x->root.fun->nbr_arg, &args_len);
                        Object res = eval_func(args, args_len, *func.val.funcid);
                        Obj_free_array(args, args_len);
                        return res;
                    }

                }
                else if(x->right->type == Ast_varcall_t){
                    Object var = get_Obj_mem(*a.val.module->MEM, x->right->root.varcall);
                    if (var.type == Obj_not_found_t){
                        printf("ERROR function '%s' not exist in methods of list",x->right->root.varcall);
                        exit(1);
                    }
                    return var;
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

                            int args_len = 0;
                            Object *args = eval_args(x->right->root.fun->args, x->right->root.fun->nbr_arg, &args_len);
                            args_len++;
                            args = realloc(args, sizeof(Object) * args_len);
                            for(int i = args_len - 1;  i > 0; i--)
                                args[i] = args[i - 1];
                            args[0] = Obj_cpy(a);
                            Object res = (*func.val.funcid->func_p)(args, args_len);
                            Obj_free_array(args, args_len);
                            return res;
                        }
                        else{
                            Object arg = Obj_cpy(a);
                            Object res = (*func.val.funcid->func_p)(&arg,1);
                            Obj_free_val(arg);
                            return res;
                        }
                    }
                    else {
                        int args_len = 0;
                        Object *args = eval_args(x->right->root.fun->args, x->right->root.fun->nbr_arg, &args_len);
                        args_len++;
                        args = realloc(args, sizeof(Object) * args_len);
                        for(int i = args_len - 1;  i > 0; i--)
                                args[i] = args[i - 1];
                        args[0] = Obj_cpy(a);
                        Object res = eval_func(args, args_len, *func.val.funcid);
                        Obj_free_array(args, args_len);
                        return res;
                    }

                }
                else if(x->right->type == Ast_varcall_t){
                    Object var = get_Obj_mem(*string_module.MEM, x->right->root.varcall);
                    if (var.type == Obj_not_found_t){
                        printf("ERROR function '%s' not exist in methods of list",x->right->root.varcall);
                        exit(1);
                    }
                    return var;
                }
                
            }
            if(a.type == Obj_complex_t){
                if (x->right->type == Ast_funccall_t){
                    Object func = get_Obj_mem(*complex_module.MEM, x->right->root.fun->name);
                    if (func.type == Obj_not_found_t){
                        printf("ERROR function '%s' not exist in methods of complex\n",x->right->root.fun->name);
                        exit(1);
                    }
                    if(func.val.funcid->is_builtin){
                        if(x->right->root.fun->nbr_arg){
                            int args_len = 0;
                            Object *args = eval_args(x->right->root.fun->args, x->right->root.fun->nbr_arg, &args_len);
                            args_len++;
                            args = realloc(args, sizeof(Object) * args_len);
                            for(int i = args_len - 1;  i > 0; i--)
                                args[i] = args[i - 1];
                            args[0] = Obj_cpy(a);
                            Object res = (*func.val.funcid->func_p)(args, args_len);
                            Obj_free_array(args, args_len);
                            return res;
                        }
                        else{
                            Object arg = Obj_cpy(a);
                            Object res = (*func.val.funcid->func_p)(&arg,1);
                            Obj_free_val(arg);
                            return res;
                        }
                    }
                    else {
                        int args_len = 0;
                        Object *args = eval_args(x->right->root.fun->args, x->right->root.fun->nbr_arg, &args_len);
                        args_len++;
                        args = realloc(args, sizeof(Object) * args_len);
                        for(int i = args_len - 1;  i > 0; i--)
                                args[i] = args[i - 1];
                        args[0] = Obj_cpy(a);
                        Object res = eval_func(args, args_len, *func.val.funcid);
                        Obj_free_array(args, args_len);
                        return res;
                    }

                }
                else if(x->right->type == Ast_varcall_t){
                    Object var = get_Obj_mem(*complex_module.MEM, x->right->root.varcall);
                    if (var.type == Obj_not_found_t){
                        printf("ERROR function '%s' not exist in methods of complex\n",x->right->root.varcall);
                        exit(1);
                    }
                    return var;
                }
                
            }
            if(a.type == Obj_funcid_t){
                if (x->right->type == Ast_funccall_t){
                    Object func = get_Obj_mem(*funccall_module.MEM, x->right->root.fun->name);
                    if (func.type == Obj_not_found_t){
                        printf("ERROR function '%s' not exist in methods of funcid\n",x->right->root.fun->name);
                        exit(1);
                    }
                    if(func.val.funcid->is_builtin){
                        if(x->right->root.fun->nbr_arg){
                            int args_len = 0;
                            Object *args = eval_args(x->right->root.fun->args, x->right->root.fun->nbr_arg, &args_len);
                            args_len++;
                            args = realloc(args, sizeof(Object) * args_len);
                            for(int i = args_len - 1;  i > 0; i--)
                                args[i] = args[i - 1];
                            args[0] = Obj_cpy(a);
                            Object res = (*func.val.funcid->func_p)(args, args_len);
                            Obj_free_array(args, args_len);
                        }
                        else{
                            Object arg = Obj_cpy(a);
                            Object res = (*func.val.funcid->func_p)(&arg,1);
                            Obj_free_val(arg);
                            return res;
                        }
                    }
                    else {
                        int args_len = 0;
                        Object *args = eval_args(x->right->root.fun->args, x->right->root.fun->nbr_arg, &args_len);
                        args_len++;
                        args = realloc(args, sizeof(Object) * args_len);
                        for(int i = args_len - 1;  i > 0; i--)
                                args[i] = args[i - 1];
                        args[0] = Obj_cpy(a);
                        Object res = eval_func(args, args_len, *func.val.funcid);
                        Obj_free_array(args, args_len);
                        return res;
                    }

                }
                else if(x->right->type == Ast_varcall_t){
                    Object var = get_Obj_mem(*funccall_module.MEM, x->right->root.varcall);
                    if (var.type == Obj_not_found_t){
                        printf("ERROR function '%s' not exist in methods of function",x->right->root.varcall);
                        exit(1);
                    }
                    return var;
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
                            int args_len = 0;
                            Object *args = eval_args(x->right->root.fun->args, x->right->root.fun->nbr_arg, &args_len);
                            args_len++;
                            args = realloc(args, sizeof(Object) * args_len);
                            for(int i = args_len - 1;  i > 0; i--)
                                args[i] = args[i - 1];
                            args[0] = Obj_cpy(a);
                            Object res = (*func.val.funcid->func_p)(args, args_len);
                            Obj_free_array(args, args_len);
                            return res;
                        }
                        else{
                            Object arg = Obj_cpy(a);
                            Object res = (*func.val.funcid->func_p)(&arg,1);
                            Obj_free_val(arg);
                            return res;
                        }
                    }
                    else {
                        int args_len = 0;
                        Object *args = eval_args(x->right->root.fun->args, x->right->root.fun->nbr_arg, &args_len);
                        args_len++;
                        args = realloc(args, sizeof(Object) * args_len);
                        for(int i = args_len - 1;  i > 0; i--)
                                args[i] = args[i - 1];
                        args[0] = Obj_cpy(a);
                        Object res = eval_func(args, args_len, *func.val.funcid);
                        Obj_free_array(args, args_len);
                        return res;
                    }

                }
                else if(x->right->type == Ast_varcall_t){
                    Object var = get_Obj_mem(*list_module.MEM, x->right->root.varcall);
                    if (var.type == Obj_not_found_t){
                        printf("ERROR function '%s' not exist in methods of list",x->right->root.varcall);
                        exit(1);
                    }
                    return var;
                }
            }
            else{
                printf("ERROR on dot operator\n");
                exit(1);
            }
        }
        if(x->type == Ast_colon_t){
            Object a = Obj_cpy(eval_Ast(x->left));
            if (a.type == Obj_list_t){
                Object b = Obj_cpy(eval_Ast(x->right));
                Object old_b = b;
                b = std_ount(&old_b, 1);
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
                Object res = a.val.li->elements[1 + index];
                Obj_free_val(a);
                Obj_free_val(b);
                return res;
            }
            if(a.type == Obj_string_t){
                Object b = Obj_cpy(eval_Ast(x->right));
                Object old_b = b;
                b = std_ount(&old_b, 1);
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
                    char s[2] = {a.val.s[index], '\0'};
                    res = new_string(s);
                }
                else{
                    res = new_ount(len);
                }
                Obj_free_val(a);
                Obj_free_val(b);
                return res;
            }
            if(a.type == Obj_complex_t){
                int index = -1;
                if (x->right->type == Ast_varcall_t) {
                    if (!strcmp("re", x->right->root.varcall) || !strcmp("Re", x->right->root.varcall))
                        index = 0;
                    else if (!strcmp("RE", x->right->root.varcall) || !strcmp("rE", x->right->root.varcall))
                        index = 0;
                    else if (!strcmp("im", x->right->root.varcall) || !strcmp("Im", x->right->root.varcall))
                        index = 1;
                    else if (!strcmp("IM", x->right->root.varcall) || !strcmp("iM", x->right->root.varcall))
                        index = 1;
                }
                else {
                    Object b = eval_Ast(x->right);
                    if (b.type == Obj_ount_t && (b.val.i == 1 || b.val.i == 0))
                        index = b.val.i;
                    else if (b.type == Obj_boolean_t && (b.val.b == 1 || b.val.b == 0))
                        index = (int)b.val.i;
                    else if (b.type == Obj_floap_t && ((int)b.val.f == 1 || (int)b.val.f == 0))
                        index = (int)b.val.f;
                    else if (b.type == Obj_string_t) {
                        if (!strcmp("re", b.val.s) || !strcmp("Re", b.val.s))
                            index = 0;
                        else if (!strcmp("RE", b.val.s) || !strcmp("rE", b.val.s))
                            index = 0;
                        else if (!strcmp("im", b.val.s) || !strcmp("Im", b.val.s))
                            index = 1;
                        else if (!strcmp("IM", b.val.s) || !strcmp("iM", b.val.s))
                            index = 1;
                    }
                }

                if (index == -1) {
                    printf("ERROR complex wrong operand for ':'\n");
                    exit(1);
                }
                return new_ount(a.val.c[index]);
            }
            else{
                printf("cannot use ':'\n");
                exit(1);
            }
        }
    }
    return nil_Obj;
}



Object *eval_args(Ast *args, int len, int *ret_len) {
    int malloc_len = len;
    *ret_len = 0;
    Object *res = malloc(sizeof(Object) * malloc_len);
    for(int i = 0; i < len; i++) {
        if (args[i].type != Ast_unpack_t)
            res[(*ret_len)++] = Obj_cpy(eval_Ast(&args[i]));
        else{
            Object x = eval_Ast(args[i].right);
            if (x.type != Obj_list_t) {
                printf("ERROR cannot unpack object of type %s\n", Obj_type_as_str(x.type));
                exit(1);
            }
            if (x.val.li->elements[0].val.i == 0){
                x = Obj_cpy(x);
                Obj_free_val(x);
                continue;
            }
            int list_len = x.val.li->elements[0].val.i;
            res = realloc(res, sizeof(Object) * (malloc_len + list_len - 1));
            for(int k = 0; k < list_len; k++)
                res[(*ret_len)++] = Obj_cpy(x.val.li->elements[k + 1]); 
            malloc_len += list_len - 1;
            x = Obj_cpy(x);
            Obj_free_val(x);
        }
    }
    return res;
}


Object eval_func(Object *argv, int argc, Funcdef func){
    if (func.is_builtin)
        return (*func.func_p)(argv, argc);
    //search perfect match
    int n = -1;
    for (int i = 0; i < func.defs_len; i++) {
        if (func.defs[i].args_mod ==  'o' && argc == func.defs[i].args_len) {
            n = i;
            break;
        }
    }
    if (n == -1) {
        //search one that match with packed args
        for (int i = 0; i < func.defs[i].args_len; i++) {
            if (func.defs[i].args_mod == '+' && argc >= func.defs[i].args_len - 1) {
                n = i;
                break;
            }
        }
        if (n == -1) {
            printf("ERROR cannot call function %s with %d arguments\n", func.name, argc);
            exit(1);
        }
    }
    return func_execute(argv, argc, func.defs[n], func.name, 1);
}