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



typedef struct  Elif{
    Ast*condition;
    int endif_p;
}Elif;

typedef struct If{
    Ast*condition;
    int endif_p;
}If;


typedef struct For{
    char*var_name;
    Ast*start;
    Ast*end;
    int endfor;
}For;

typedef struct While{
    Ast*condition;
    int endwhile;
}While;

typedef struct Swap{
    char**ids;
    int len;
}Swap;


typedef struct Instruction{
    short int type;
    union {
        varset*vs;
        Funcdef*fc;
        If*i;
        For*fo;
        While*wh;
        int endwhile;
        int endfor;
        Ast*ret;
        Ast*expr;
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
    inst_pass_t
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