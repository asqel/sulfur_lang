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
    char*var_name;
    struct Ast*start;
    struct Ast*end;
    int endfor;
}For;

typedef struct While{
    struct Ast*condition;
    int endwhile;
}While;

typedef struct Swap{
    char**ids;
    int len;
}Swap;

typedef struct Funcdef_code{
    char**ret_type;// list of types that can be returned by the function
    int nbr_ret_type;//len of ret_type
    char**arg_types;
    char**arg_names;
    int nbr_of_args;
    int code_len;
    char is_builtin;
    struct Object (*func_p)(struct Object*,int);//pointer to the builtin function
    struct Instruction*code;
    char*description;//can be shown with help()
    char*name;
}Funcdef_code;

typedef struct Instruction{
    short int type;
    union {
        varset*vs;
        Funcdef_code*fc;
        If*i;
        For*fo;
        While*wh;
        int endwhile;
        int endfor;
        struct Ast*ret;
        struct Ast*expr;
        int endif;
        int endifelse;
        Elif*el;
        Swap*sw;
        char*section;
        char*goto_sec;
    }value;
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
    inst_funcdef_t
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


#endif