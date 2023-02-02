#include <stdlib.h>
#include <stdio.h>
#include "libs/lexer.h"
#include "libs/memlib.h"


Object*MEMORY;//array of Object ended with end_obj
char**MEMORY_key;//array of string ended with "-2.3" string
Object**STACK;//end with an array that start with 2 end_obj
char***STACK_KEY;//end with an array that start with a string "-1.2"


int main(int argc,char **argv){
    back_slash_to_path(argv[0]);
    char *d=dirname(argv[0]);
    char *filepath=str_cat_new(d,"/main.wh");
    char *text=read_file(filepath);

    Token*l=lexe(text);
    tokens_print(l,"\n");
    int len=token_len(l);
    for(int i=0;i<len;i++){
        free_tok_val(l[i]);
    }
    free(l);

    Object*MEMORY=malloc(sizeof(Object));//end with end_obj
    char **MEMORY_key=malloc(sizeof(char *));//end with "-2.3" string

    Object**STACK=malloc(sizeof(Object*));//end with an array that start with 2 end_obj
    char ***STACK_KEY=malloc(sizeof(char **));//end with an array of string that start with "-1.2"string

    MEMORY_key[0]="__path__";//pour ajouter une variable path qui contient le chemin du fichier executé
    MEMORY[0].type=Obj_string_t;//ici C:/../main.wh
    MEMORY[0].val.s=malloc(sizeof(char)*(1+strlen(filepath)));
    strcpy(MEMORY[0].val.s,filepath);

    //len of a list in the memory *(obj.val.o[0].i)
    //len of a list of memory  end with something written above 

    return 0;
}