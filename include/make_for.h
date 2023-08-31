#ifndef MAKE_FOR_H
#define MAKE_FOR_H

#include "../include/token_class.h"
#include "../include/Ast.h"

Instruction* make_for(Token* tok, int start, int end, Instruction* inst, int* n_inst, int* p, int len);

#endif