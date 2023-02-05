#include "interpreter.h"


void init_memory(){
    Object*MEMORY=malloc(sizeof(Object));
    char*MEMORY_key=malloc(sizeof(char *));
    long long int MEMORY_len=0;
}


void init_stack(){
    Object**STACK=malloc(sizeof(Object*));
    char***STACK_KEY=malloc(sizeof(char**));
    long long int STACK_len=0;
    long long int sub_STACK_len=0;
}

void init_funcdefs(){
    Instruction**FUNCDEFS=malloc(sizeof(Instruction*));
    char**FUNCDEFS_NAME=malloc(sizeof(char*));
    long long int FUNCDEFS_len=0;
}

int execute(Instruction*code,char*file_name){

    return 0;
}