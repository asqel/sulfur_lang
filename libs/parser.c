#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "token_class.h"


//take a math expression and see if it ok
//return nothing 
//print error and exit(1)
void check_syntax(Ast*x){


}
Ast*make_list_funccal_varcal(Ast*x){

}

//start included
//end not included
//there no check if end and start are out of range
Ast*make_ast(Token*tok,int start,int end){
    Ast*e=malloc(sizeof(Ast)*(end-start));
    for(int i=start;i<end;i++){
        e=realloc(e,sizeof(Token)*(i-start+1));
        e[i-start].type=Ast_token_t;
        e[i-start].left=NULL;
        e[i-start].right=NULL;
        e[i-start].root.tok->type=tok[i].type;
        if(tok[i].type==comp){
            e[i-start].root.tok->value.c=malloc(sizeof(long double)*2);
            e[i-start].root.tok->value.c[0]=tok[i].value.c[0];
            e[i-start].root.tok->value.c[1]=tok[i].value.c[1];
        }
    }//e contains only tokens from start to end 
    check_syntax(e);
    e=make_list_funccal_varcal(e);
    



}


int find_semicol(Token*tok,int p){
    int len=token_len(tok);
    for(int i=0;i+p<len;i++){
        if(tok[i+p].type==syntax&&*tok[i+p].value.t==semicolon){
            return i+p;
        }
    }
    return -1;
}


Instruction*parse(Token*tok){

    int len=token_len(tok);
    int p=0;
    Instruction*inst=malloc(sizeof(Instruction));
    int n_inst=1;
    while(p<len){
        if(tok[p].type==identifier){
            if(p+2<len){
                if(tok[p+1].type==identifier) {
                    int n=find_semicol(tok,p);
                    if(tok[p+2].type==op&&*tok[p+2].value.t==OP_ASSIGN&&n!=-1){
                        n_inst++;
                        inst=realloc(inst,sizeof(Instruction)*n_inst);
                        inst[n_inst-1].type=inst_varset_t;

                        inst[n_inst-1].value.vs=malloc(sizeof(varset));
                        inst[n_inst-1].value.vs->name=malloc(sizeof(char)*(strlen(tok[p].value.s))+1);
                        strcpy(inst[n_inst-1].value.vs->name,tok[p].value.s);//set name
           
                        inst[n_inst-1].value.vs->type=malloc(sizeof(char)*(strlen(tok[p+1].value.s))+1);
                        strcpy(inst[n_inst-1].value.vs->type,tok[p+1].value.s);//set type
                        Ast*v=make_ast(tok,p+3,n);
                        inst[n_inst-1].value.vs->val=v;

                    }
                    else{
                        if (n==-1){
                            printf("ERROR : missing semicollon ';' on line %d after Token ",tok[p+2].line);
                            token_print(tok[p+2],"\n");
                            exit(1);
                        }
                        if(tok[p+2].type!=op||*tok[p+2].value.t!=OP_ASSIGN){
                            printf("ERROR : missing assignement '=' on line %d after token",tok[p+2].line);
                            token_print(tok[p+2],"\n");
                            exit(1);
                        }
                    }
                }
            }

        }
        else{
            p++;
        }
    }


}