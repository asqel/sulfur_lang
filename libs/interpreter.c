#include "interpreter.h"




Object*MEMORY;//array of Objec
char**MEMORY_key;//array of st
Object**STACK;//end with an ar
char***STACK_KEY;//end with an

long long int MEMORY_len;
long long int STACK_len;
long long int*sub_STACK_len;//

Instruction**FUNCDEFS;
char**FUNCDEFS_NAME;
long long int FUNCDEFS_len;

CLASSDEF*CLASSDEFS;
long long int CLASSDEF_len;

ref_counter*REF_COUNTS;
long long int REF_COUNT_len;






void init_memory(){
    MEMORY=malloc(sizeof(Object));
    MEMORY_key=malloc(sizeof(char *));
    MEMORY_len=0;
}


void init_stack(){
    STACK=malloc(sizeof(Object*));
    STACK_KEY=malloc(sizeof(char**));
    STACK_len=0;
   *sub_STACK_len=0;
}

void init_funcdefs(){
    FUNCDEFS=malloc(sizeof(Instruction*));
    FUNCDEFS_NAME=malloc(sizeof(char*));
    FUNCDEFS_len=0;
}

void init_classdefs(){
    CLASSDEF_len=0;
    CLASSDEFS=malloc(sizeof(CLASSDEF));
}

void init_garbage_collect(){
    REF_COUNT_len=0;
    REF_COUNTS=malloc(sizeof(ref_counter));
}

int execute(Instruction*code,char*file_name){

    return 0;
}