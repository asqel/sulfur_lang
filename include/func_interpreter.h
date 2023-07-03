#ifndef FUNC_INTERPRETER_H
#define FUNC_INTERPRETER_H

#include "memlib.h"
#include "instruction.h"
extern stack STACK;

void init_stack();

Object func_execute(Funcdef* func, Object* argv, int argc, int create_stack);

#endif