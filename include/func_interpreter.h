#ifndef FUNC_INTERPRETER_H
#define FUNC_INTERPRETER_H

#include "memlib.h"
#include "instruction.h"
extern stack STACK;

void init_stack();

Object func_execute(Instruction*code,char*file_name,int len, int create_stack);

#endif