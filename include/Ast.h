#ifndef Ast_H
#define Ast_H

#include "memlib.h"
#include "token_class.h"
#include "instruction.h"


struct Ast;

typedef struct funccall{
    char*name;
    struct Ast*args;
    int nbr_arg;
    int name_idx;
}funccall;

typedef struct expr_funccall{
    struct Ast* expr;
    struct Ast*args;
    int nbr_arg;
}expr_funccall;

typedef struct anonym_func{
    int code_len;
    struct Instruction* code;
}anonym_func;

typedef struct list_comprehension{
    struct Ast* expr;
    struct Ast* start;
    struct Ast* end;
    char* varname;
}list_comprehension;


typedef struct tempexpr{
    struct Ast*val;
}tempexpr;

typedef struct Ast{
    int type;
    char isAst;
    union{
        Object*obj;
        funccall*fun;
        Token*tok;
        char*varcall;
        int var_idx;
        short int op;
        short int sy;
        short int kw;
        anonym_func* ano_func;
        list_comprehension* li;
        expr_funccall* ex;
        long long int constant[2];
    }root;
    struct Ast*left;
    struct Ast*right;
}Ast;

//unary - are store like an ast with root  - , left NULL , and right the value
//same for unary +
//same for unary !
enum Ast_types{
    Ast_end_t,
    Ast_object_t,
    Ast_token_t,
    Ast_varcall_t,//stored 
    Ast_varcall_idx_t,
    Ast_funccall_t,
    Ast_tempexpr_t,//.obj that start with an object int that contains the length
    Ast_add_t,
    Ast_sub_t,
    Ast_mul_t,
    Ast_div_t,
    Ast_fldiv_t,
    Ast_mod_t,
    Ast_pow_t,
    Ast_or_t,
    Ast_and_t,
    Ast_not_t,
    Ast_eq_t,
    Ast_noteq_t,
    Ast_geq_t,
    Ast_leq_t,
    Ast_le_t,
    Ast_ge_t,
    Ast_dot_t,
    Ast_op_t,
    Ast_syntax_t,
    Ast_keyword_t,
    Ast_rshift_t,
    Ast_lshift_t,
    Ast_assign_t,
    Ast_plus_assign_t,
    Ast_minus_assign_t,
    Ast_anonym_func_t,
    Ast_list_comprehension_t,
    Ast_colon_t,
    Ast_expr_funccall_t, //function called like this : (...)(a,b) NOT SUPPORTED YET
    Ast_unpack_t,
    Ast_bit_xor_t,
	ast_nil_t

};

//check if an Ast has a right side 
//if not  it return 0
//if it has not a right side it will have a null pointer
int Ast_has_r(Ast x);

int op_tok_to_op_ast(int v,int type);

int Ast_len(Ast*x);

int print_ast(Ast x);

void free_ast(Ast x);

char *get_op_str(Ast x);

#endif