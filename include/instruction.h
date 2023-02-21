#ifndef INSTRUCTION_H
#define INSTRUCTION_H
#include "token_class.h"
#include "memlib.h"
#include "Ast.h"




struct Instruction;

typedef struct varset{
    char*name;
    char*type;
    Ast*val;

}varset;


typedef struct funcdef{
    char*name;
    char*type;
    char*arg_types;
    char*arg_names;
    char*is_builtin;
    struct Instruction*code;
}funcdef;

typedef struct  Elif{
    Ast*condition;
    struct Instruction*endif_p;
}Elif;

typedef struct If{
    Ast*condition;
    struct Instruction*endif_p;
}If;


typedef struct For{
    char*var_name;
    Ast*start;
    Ast*end;
    struct Instruction*endfor;
}For;

typedef struct While{
    Ast*condition;
    struct Instruction*endwhile;
}While;



typedef struct Instruction{
    short int type;
    union {
        varset*vs;
        funcdef*fc;
        If*i;
        For*fo;
        While*wh;
        Ast*ret;
        Ast*expr;
        struct Instruction*endif;
        struct Instruction*endifelse;
        Elif*el;
    }value;
}Instruction;

typedef struct Fundef{
    Instruction*code;
}Funcdef;


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
    inst_return_t
};

#endif