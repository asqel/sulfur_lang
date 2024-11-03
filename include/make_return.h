#ifndef MAKE_RETURN_H
#define MAKE_RETURN_H

#include "token_class.h"
#include "Ast.h"

Instruction* make_return(Token* tok, int start, int end, Instruction* inst, int* n_inst, int* p, int len);

#endif