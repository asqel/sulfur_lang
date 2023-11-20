#ifndef EVALUATOR_H
#define EVALUATOR_H
#include "memlib.h"
#include "Ast.h"

Object eval_Ast(Ast*x);

Object *eval_args(Ast *args, int len, int *ret_len);

Object eval_func(Object *argv, int argc, Funcdef func);

Object eval_func(Object *argv, int argc, Funcdef func);

#endif