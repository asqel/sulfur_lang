#ifndef MAKE_WHILE_H
#define MAKE_WHILE_H

#include "../include/token_class.h"
#include "../include/Ast.h"

Instruction* make_while(Token* tok, int start, int end, Instruction* inst, int* n_inst, int* p, int len);

#endif