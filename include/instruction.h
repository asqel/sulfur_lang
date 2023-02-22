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

/*every blt function should be definied like this
{
    name;
    return types;
    number of return types;
    types of args; (NULL if number of args is -1)
    NULL;
    number of args; (-1 if not specified)
    1;
    the pointer to the builtin function;
    NULL;
}
the builtin function written in c has to to take 2 argument: an array of Object and the length of this array
if the function number of args=0 a NULL pointer is passed
return type must be Object
*/
typedef struct Funcdef{
    char*name;//name of function
    char**ret_type;// list of types that can be returned by the function
    int nbr_ret_type;//len of ret_type
    char**arg_types;
    char**arg_names;
    int nbr_of_args;
    char is_builtin;
    Object (*func_p)(Object*,int);//pointer to the builtin function
    struct Instruction*code;
}Funcdef;

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
        Funcdef*fc;
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



extern Funcdef*FUNCDEFS;
extern long long int FUNCDEFS_len;

extern Lib*LIBS;
extern long long int LIBS_len;

void init_libs() __attribute__((constructor));;


#endif