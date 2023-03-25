#include <stdlib.h>
#include <stdio.h>
#include "include/lexer.h"
#include "include/memlib.h"
#include "include/parser.h"
#include "include/interpreter.h"
#include "sulfur_libs/blt_libs/std.h"


int main(int argc,char **argv){
    int show_mem=0;
    for(int i=1;i<argc;i++){
        if(!strcmp(argv[i],"-m")){
            show_mem=1;
        }
    }
    char*path0=abs_path();
    back_slash_to_path(path0);
    char *d=dirname(path0);
    free(path0);
    char *filepath=str_cat_new(d,"/main.su");
    free(d);
    char *text=read_file(filepath);
    Token*l=lexe(text);
    int len=token_len(l); 

    int*instruction_len=malloc(sizeof(int));
    *instruction_len=0;

    Instruction*code=parse(l,-1,-1,NULL,instruction_len);

    //parser copy values of tokens so
    //you can free tokens after parsing 
    for(int i=0;i<=len-1;i++){
        free_tok_val(l[i]);
    }
    free(l);
    init_memory();
    init_stack(); 
    init_libs();  
    

    execute(code,filepath,*instruction_len);
    if(show_mem){
        printf("\n \nMEMORY:%d\n",MEMORY.len);
        for(int i=0;i<MEMORY.len;i++){
            printf("    %s: ",MEMORY.keys[i]);
            println_prompt(&MEMORY.values[i],1);
        }
    }
    return 0;
}