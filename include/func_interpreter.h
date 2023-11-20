#ifndef FUNC_INTERPRETER_H
#define FUNC_INTERPRETER_H

#include "memlib.h"
#include "instruction.h"
extern stack STACK;

void init_stack();

Object func_execute(Object* argv, int argc, sulfur_func func, char *name, int add_stack);

#endif