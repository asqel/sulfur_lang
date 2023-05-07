
#include "../include/memlib.h"
#include "../include/Ast.h"
#include "../include/operation.h"

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
        /*
        if typ==assigne
        left has to be either varcall or list gettting '[ ...]'
        but if its varcall technicaly its not here but ok
        oh and also with dot operator 
        but the parser doesnt parse '[]' i think 
        else no
        and return nil_obj;
        */
        if(x->type == Ast_assign_t){
            Object val=eval_Ast(x->right);
            Ast *left=eval_left_assign(x->left);
        }
        if(x->type == Ast_dot_t){
            Object a=eval_Ast(x->left);
            temp_ref(a);
            if(a.type != obj_module_t){
                printf("ERROR dot operator on non module Object");
                exit(1);
            }
            if (x->right->type == Ast_varcall_t || x->right->type == Ast_funccall_t){
                memory global=MEMORY;
                MEMORY=*a.val.module->MEM;
                Object o=eval_Ast(x->right);
                MEMORY=global;
                return o;
            }
            else{
                printf("ERROR on dot operator");
                exit(1);
            }
        }
    }
}
