#ifndef INTERPRETER_H
#define INTERPRETER_H
#include "parser.h"
#include "memlib.h"


extern memory MEMORY;

extern stack STACK;


typedef struct ref_counter{
    void*pointer;
    long long int count;
    char type;
}ref_counter;



typedef struct ASSIGN_HELPER{
    memory* MEM;
    char*name;
}ASSIGN_HELPER; 

extern ref_counter*REF_COUNTS;
extern long long int REF_COUNT_len;


void init_memory();

void init_stack();

void init_libs(char*path);

void init_garbage_collect();

Object eval_Ast(Ast*x);

int add_ref(Object o);

int remove_ref(Object o);

int update_ref();


Object execute(Instruction*code,char*file_name,int len);


#endif