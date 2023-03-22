#include <stdlib.h>
#include <stdio.h>
#include "include/lexer.h"
#include "include/memlib.h"
#include "include/parser.h"
#include "include/interpreter.h"
    #include "sulfur_libs/blt_libs/std.h"



int main(int argc,char **argv){
    back_slash_to_path(argv[0]);
    char *d=dirname(argv[0]);
    char *filepath=str_cat_new(d,"/main.su");
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
    


    //set a var __path__ to current .su file path executed
    MEMORY.len++;
    //MEMORY.values=realloc(MEMORY.values,sizeof(Object)*MEMORY.len);
    //utiliser le realloc modifier des valeurs 
    Object*e=malloc(sizeof(Object)*MEMORY.len);
    for(int i=0;i<MEMORY.len-1;i++){
        e[i]=MEMORY.values[i];
    }
    free(MEMORY.values);
    MEMORY.values=e;

    
    MEMORY.keys=realloc_c(MEMORY.keys,sizeof(char*)*(MEMORY.len-1),sizeof(char*)*MEMORY.len);
    
    MEMORY.keys[MEMORY.len-1]=malloc(sizeof(char)*(strlen("__path__")+1));
    strcpy(MEMORY.keys[MEMORY.len-1],"__path__");
    
    

    MEMORY.values[MEMORY.len-1].type=Obj_string_t;
    MEMORY.values[MEMORY.len-1].val.s=malloc(sizeof(char)*(1+strlen(filepath)));
    strcpy(MEMORY.values[MEMORY.len-1].val.s,filepath);

    execute(code,filepath,*instruction_len);
    return 0;
}