#include <stdlib.h>
#include <stdio.h>
#include "include/lexer.h"
#include "include/memlib.h"
#include "include/parser.h"
#include "include/interpreter.h"




int main(int argc,char **argv){
    printf("salut2");
    back_slash_to_path(argv[0]);
    char *d=dirname(argv[0]);
    char *filepath=str_cat_new(d,"/build/main.su");
    char *text=read_file(filepath);
    Token*l=lexe(text);
    int len=token_len(l);
    int*instruction_len=malloc(sizeof(int));
    *instruction_len=0;
    printf("befor parse Ok\n");
    Instruction*code=parse(l,-1,-1,NULL,instruction_len);
    printf("after parse Ok\n");
    //parser copy values of tokens so
    //you can free tokens after parsing 
    //for(int i=0;i<=len;i++){
    //    token_print(l[i],"");
    //    printf("%d\n",i);
    //    free_tok_val(l[i]);
    //}
    //free(l);
    init_memory();
    init_stack();    
    init_funcdefs();
    

    //set a var __path__ to current .wh file path executed
    MEMORY_len++;
    MEMORY=realloc(MEMORY,sizeof(Object)*(MEMORY_len));
    MEMORY_key[MEMORY_len-1]=malloc(sizeof(char)*(strlen("__path__")+1));
    strcpy(MEMORY_key[MEMORY_len-1],"__path__");

    MEMORY[MEMORY_len-1].type=Obj_string_t;
    MEMORY[MEMORY_len-1].val.s=malloc(sizeof(char)*(1+strlen(filepath)));
    strcpy(MEMORY[0].val.s,filepath);
    printf("before exec Ok");
    execute(code,filepath,*instruction_len);
    printf("after exec ok");
    return 0;
}