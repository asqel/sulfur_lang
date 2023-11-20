#ifndef MAKE_FUNCDEF_H
#define MAKE_FUNCDEF_H

#include "instruction.h"
#include "token_class.h"

Instruction *make_funcdef(Token* tok, int start, int end, Instruction* inst, int* n_inst, int* p, int len);

#endif