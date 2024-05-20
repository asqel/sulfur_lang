#ifndef INSTRUCTION_H
#define INSTRUCTION_H
#include "token_class.h"
#include "memlib.h"
#include "Ast.h"




struct Instruction;

typedef struct varset{
    char*name;
    char*type;
    struct Ast*val;

}varset;



typedef struct  Elif{
    struct Ast*condition;
    int endif_p;
}Elif;

typedef struct If{
    struct Ast*condition;
    int endif_p;
}If;


typedef struct For{
    int var_name;
    struct Ast*start;
    struct Ast*end;
    S_sulfur_int endfor;
}For;

typedef struct While{
    struct Ast*condition;
    int endwhile;
}While;



typedef struct Func_info{
    char* description;//can be shown with help()
    char* name;
}Func_info;


typedef struct Funcdef_code{
    Func_info info;

    char** args;
    int args_len;
    char args_mod;  // 'o' = only args_len,  '+' = args_len or more

    struct Instruction* code;
    int code_len;
}Funcdef_code;


typedef struct Instruction{
    short int type;
    union {
        varset *vs;
        Funcdef_code *fc;
        If *i;
        For *fo;
        While *wh;
        int endwhile;
        int endfor;
        struct Ast *ret;
        struct Ast *expr;
        int endif;
        int endifelse;
        Elif *el;
        char *section;
        char *goto_sec;
        int jmp;
        Object *obj;
        int jmp_length;
        int op;
        S_ount_t ount;
        double floap;
    }value;
    int line;
    char facultative; // used by expression / pass
}Instruction;

enum instruction_type{
    inst_varset_t,
    inst_new_varset_t,//use .vs without setting type
    inst_endif,
    inst_endifelse,
    inst_if_t,
    inst_elif_t,
    inst_else_t,
    inst_expr_t,
    inst_endfor_t,
    inst_endwhile_t,
    inst_for_t,
    inst_while_t,
    inst_return_t,
    inst_swap_t,
    inst_section_t,
    inst_goto_t,
    inst_pass_t,
    inst_funcdef_t,
    inst_proceed_t,
    inst_stop_t,
    inst_jmp_t, // jump to index of instruction
    // instructions of bytecode
    inst_S_op_t,
    inst_S_push_t,
    inst_S_push_1b_t,
    inst_S_push_0b_t,
    inst_S_push_nil_t,
    inst_S_push_i_t,
    inst_S_push_ount_t,
    inst_S_push_floap_t,
    inst_S_pop_t,
    inst_S_op_add_t,
    inst_S_op_sub_t,
    inst_S_op_mul_t,
    inst_S_op_div_t,
    inst_S_op_fldiv_t,
    inst_S_op_modulo_t
};

enum instruction_op_stack {
    instOp_add,
    instOp_sub,
    instOp_mul,
    instOp_div,
    instOp_fl_div,
    instOp_negate,
    instOp_not,
    instOp_pow,
    
};

//to acces an element
//just name.element
//if name==" " just element
typedef struct Lib{
    char*name;

    Funcdef*funcs;
    int nbr_funcs;

    Object*vars;
    char**vars_name;
    int nbr_vars;
}Lib;

int inst_to_str(Instruction*i);



extern Funcdef*FUNCDEFS;
extern long long int FUNCDEFS_len;

extern Lib*LIBS;
extern long long int LIBS_len;


void instruction_free_array(Instruction *code, int len);

void instruction_free(Instruction code);
#endif