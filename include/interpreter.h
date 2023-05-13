#ifndef INTERPRETER_H
#define INTERPRETER_H
#include "parser.h"
#include "memlib.h"


extern memory MEMORY;

void init_memory();

void init_libs(char*path);

void init_garbage_collect();

Object execute(Instruction*code,char*file_name,int len);


#endif