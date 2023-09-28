#ifndef INTERPRETER_H
#define INTERPRETER_H
#include "parser.h"
#include "memlib.h"

extern Instruction *current_instructions;
extern int *current_index;
extern int instruction_len;

extern memory MEMORY;

void init_memory();

void init_libs(char*path);

void init_garbage_collect();

Object execute(Instruction*code,char*file_name,int len);


#endif