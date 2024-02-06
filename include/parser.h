#ifndef PARSER_H
#define PARSER_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "token_class.h"
#include "memlib.h"
#include "Ast.h"
#include "instruction.h"



void check_syntax(Ast*x);

typedef struct ast_and_len{
    Ast*value;
    int len;
}ast_and_len;

ast_and_len tok_to_Ast(Token*tok,int start,int end);

Ast*make_ast(Ast*e,int len);

int search_rrbrack(Token*t,int start);

int search_rpar(Token*t,int start);

int cond_parse(int start,int end,int len,int p);

Instruction *parse_next_inst(Token* tok, int start, int end, Instruction* inst, int* n_inst, int* p, int len, int *result);

Instruction*parse(Token*tok,int start,int end,Instruction*inst,int*n_inst);

#endif