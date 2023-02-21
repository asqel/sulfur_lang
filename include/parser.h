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

Ast*make_list_funccal_varcal(Ast*x);

Ast*make_ast(Token*tok,int start,int end);

int cond_parse(int start,int end,int len,int p);

Instruction*parse(Token*tok,int start,int end,Instruction*inst,int*n_inst);

#endif