#include <stdlib.h>
#include <stdio.h>
#include "include/lexer.h"
#include "include/memlib.h"
#include "include/parser.h"
#include "include/interpreter.h"
#include "include/func_interpreter.h"
#include "sulfur_libs/blt_libs/std.h"

/*
args:
    [path]
    [path] -m
    -m
    nothing
*/
int main(int argc,char **argv){
    back_slash_to_path(argv[0]);
    int show_mem=0;
    char*filepath=NULL;
    
    if (argc==2){
        if(!strcmp(argv[1],"-m")){
            show_mem=1;
        }
        else{
            filepath=argv[1];
            back_slash_to_path(filepath);
        }
    }
    if (argc==3){
        if(!strcmp(argv[1],"-m")){
            show_mem=1;
            filepath=argv[2];
            back_slash_to_path(filepath);
        }
        else{
            filepath=argv[1];
            back_slash_to_path(filepath);
            if(!strcmp(argv[2],"-m")){
                show_mem=1;
            }
        }
    }
    if(argc>3){
        printf("ERROR too many arguments");
        exit(1);
    }
    if (!filepath){
        char*d=dirname(argv[0]);
        filepath=str_cat_new(d,"/main.su");
        free(d);
    }
    

    char*text=read_file(filepath);
    
    Token*l=lexe(text);
    int len=token_len(l); 

    int*instruction_len=malloc(sizeof(int));
    *instruction_len=0;
    Instruction*code=parse(l,-1,-1,NULL,instruction_len);

    //parser copy values of tokens so
    //you can free tokens after parsing 
    
    init_memory();
    init_stack(); 
    init_libs(filepath);  

    execute(code,filepath,*instruction_len);

    precision =base_precision;
    if(show_mem){
        printf("\n \nMEMORY:%d\n",MEMORY.len);
        for(int i=0;i<MEMORY.len;i++){
            printf("    %s: ",MEMORY.keys[i]);
            println_prompt(&MEMORY.values[i],1);
        }
        printf("Press enter to quit\n");
        getchar();

    }
    return 0;
}

