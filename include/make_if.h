#ifndef MAKE_IF_H
#define MAKE_IF_H

#include "token_class.h"
#include "Ast.h"

Instruction* make_if(Token* tok, int start, int end, Instruction* inst, int* n_inst, int* p, int len);

#endif