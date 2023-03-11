
#include "../include/Ast.h"



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
        }
    }
    return -1;
}