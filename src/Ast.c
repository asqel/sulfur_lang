
#include "../include/Ast.h"
#include "../sulfur_libs/blt_libs/std.h"


int Ast_has_r(Ast x){
    return x.right!=NULL;
}

int Ast_has_l(Ast x){
    return x.left!=NULL;
}


int Ast_len(Ast*x){
    int n=0;
    for(int i=0;x[i].type!=Ast_end_t;i++){
        n++;
    }
    return n;
}

int op_tok_to_op_ast(int v,int type){
    if(type==Ast_syntax_t){
        if(v==dot){
            return Ast_dot_t;
        }
        if(v == colon){
            return Ast_colon_t;
        }
    }
    if(type==Ast_op_t){
        switch (v){
            case OP_PLUS: return Ast_add_t;
            case OP_MULTIPLY: return Ast_mul_t;
            case OP_MINUS: return Ast_sub_t;
            case OP_DIVIDE: return Ast_div_t;
            case OP_EXPONENT: return Ast_pow_t;
            case OP_MODULUS: return Ast_mod_t;
            case OP_FLOOR_DIVIDE: return Ast_fldiv_t;
            case OP_OR: return Ast_or_t;
            case OP_AND: return Ast_and_t;
            case OP_NOT: return Ast_not_t;
            case OP_EQUAL: return Ast_eq_t;
            case OP_NOT_EQUAL: return Ast_noteq_t;
            case OP_LESS_EQUAL: return Ast_leq_t;
            case OP_GREATER_EQUAL: return Ast_geq_t;
            case OP_LESS: return Ast_le_t;
            case OP_GREATER: return Ast_ge_t;
            case OP_ASSIGN : return Ast_assign_t;
        }
    }
    return -1;
}

char* get_op_as_str(int type){
    switch (type){
        case Ast_add_t:
            return "+";
        case Ast_mul_t:
            return "*";
        case Ast_sub_t:
            return "-";
        case Ast_div_t:
            return "/";
        case Ast_pow_t:
            return "^";
        case Ast_mod_t:
            return "%";
        case Ast_fldiv_t:
            return "\\";
        case Ast_or_t:
            return "|";
        case Ast_and_t:
            return "&";
        case Ast_not_t:
            return "!";
        case Ast_eq_t:
            return "==";
        case Ast_noteq_t:
            return "!=";
        case Ast_leq_t:
            return "<=";
        case Ast_geq_t:
            return ">=";
        case Ast_le_t:
            return "<";
        case Ast_ge_t:
            return ">";
        case Ast_assign_t:
            return "=";
        case Ast_dot_t:
            return ".";
            
    }
}

int print_ast(Ast x){
    switch (x.type){
        case Ast_add_t:
        case Ast_mul_t:
        case Ast_sub_t:
        case Ast_div_t:
        case Ast_pow_t:
        case Ast_mod_t:
        case Ast_fldiv_t:
        case Ast_or_t:
        case Ast_and_t:
        case Ast_not_t:
        case Ast_eq_t:
        case Ast_noteq_t:
        case Ast_leq_t:
        case Ast_geq_t:
        case Ast_le_t:
        case Ast_ge_t:
        case Ast_assign_t:
        case Ast_dot_t:
            if(x.left!=NULL){
                printf("{");
                print_ast(*x.left);
            }
            printf("op[%s] ",get_op_as_str(x.type));
            if(x.right!=NULL){
                print_ast(*x.right);
                printf("}");
            }
            return 0;
        case Ast_keyword_t:
            printf("kw[%d] ",x.root.kw);
            return 0;
        case Ast_object_t:
            printf("obj[");
            print_prompt(x.root.obj,1);
            printf("] ");
            return 0;
        case Ast_varcall_t:
            printf("Vc[%s] ",x.root.varcall);
            return 0;
        case Ast_funccall_t:
            printf("Fc:[%s(",x.root.fun->name);
            for(int i=0;i<x.root.fun->nbr_arg;i++){
                print_ast(x.root.fun->args[i]);
            }
            printf(")] ");
            return 0;

    }
}