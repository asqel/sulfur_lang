#ifndef FUNC_EVALUATOR_H
#define FUNC_EVALUATOR_H

#include "../include/memlib.h"
#include "../include/Ast.h"


Object func_eval_Ast(Ast*x);

Object *func_eval_args(Ast *args, int len, int *ret_len);

#endif