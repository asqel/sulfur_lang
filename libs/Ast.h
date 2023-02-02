#ifndef Ast_H
#define Ast_H

#include "memlib.h"
#include "token_class.h"



typedef struct funccall{

}funccall;

typedef struct varcall{

}varcall;

typedef union Ast_value{
    Object*obj;
    funccall*fun;
    varcall*var;
    Token*tok;

}Ast_value;

typedef struct Ast{
    int type;
    Ast_value root;
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