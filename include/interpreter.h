#ifndef INTERPRETER_H
#define INTERPRETER_H
#include "parser.h"
#include "memlib.h"


extern memory MEMORY;

extern Object**STACK;
extern char***STACK_KEY;

extern long long int STACK_len;
extern long long int*sub_STACK_len;//list of int its len is stack_len



typedef struct ref_counter{
    void*pointer;
    long long int count;
    char type;
}ref_counter;

typedef struct CLASSDEF{
    char*id;
    char**types;
    char**names;
}CLASSDEF;


extern CLASSDEF*CLASSDEFS;
extern long long int CLASSDEF_len;

extern ref_counter*REF_COUNTS;
extern long long int REF_COUNT_len;

void check_libs();

void init_memory();

void init_stack();

void init_funcdefs();

void init_classdefs();

void init_libs(char*path);

void init_garbage_collect();

Object eval_Ast(Ast*x);

int add_ref(Object o);

int remove_ref(Object o);

int update_ref();


int execute(Instruction*code,char*file_name,int len);


#endif