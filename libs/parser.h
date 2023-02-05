#ifndef PARSER_H
#define PARSER_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
    struct Instruction*code;
}funcdef;

typedef struct  Elif{
    Ast*condition;
    struct Instruction*ifftrue;
}Elif;

typedef struct If{
    Ast*condition;
    Elif*ef;
    int elif_len;
    struct Instruction*iftrue;
    struct Instruction*iffalse;
}If;

typedef struct For{
    char*var_name;
    int start;
    int end;
    struct Instruction*code;
}For;

typedef struct While{
    Ast*condition;
    struct Instruction*iftrue;
}While;

typedef struct Return{
    Ast*value;
}Return;



typedef struct Instruction{
    short int type;
    union {
        varset*vs;
        funcdef*fc;
        If*i;
        For*fo;
        While*wh;
        Return*re;
    }value;
}Instruction;


enum instruction_type{
    inst_varset_t,
    inst_new_varset_t,
};

void check_syntax(Ast*x);

Ast*make_list_funccal_varcal(Ast*x);

Ast*make_ast(Token*tok,int start,int end);

Instruction*parse(Token*tok);

#endif