#ifndef PARSER_H
#define PARSER_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "token_class.h"
#include "memlib.h"
#include "Ast.h"


typedef struct varset{
    char*name;
    char*type;
    Ast*val;

}varset;

typedef struct Instruction{
    short int type;
    union {
        varset*vs;

    }value;
}Instruction;


enum instruction_type{
    inst_varset_t,
    inst_new_varset_t,
};


Instruction*parse(Token*tok);

#endif