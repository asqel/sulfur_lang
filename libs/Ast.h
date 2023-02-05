#ifndef Ast_H
#define Ast_H

#include "memlib.h"
#include "token_class.h"


struct Ast;

typedef struct funccall{
    char*name;
    struct Ast*args;
    int nbr_arg;
}funccall;

typedef struct varcall{
    char*name;
}varcall;

typedef struct Ast{
    int type;
    union{
        Object*obj;
        funccall*fun;
        varcall*var;
        Token*tok;
    }root;
    struct Ast*left;
    struct Ast*right;
}Ast;

enum Ast_types{
    Ast_token_t,
    Ast_varcall_t,
    Ast_funccall_t,
};

//check if an Ast has a right side 
//if not  it return 0
//if it has not a right side it will have a null pointer
int Ast_has_r(Ast x);

#endif