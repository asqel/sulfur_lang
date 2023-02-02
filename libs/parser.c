#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "token_class.h"


Instruction*parse(Token*tok){

    int len=token_len(tok);
    int p=0;
    Instruction*inst=malloc(sizeof(Instruction));
    int n_inst=1;
    while(p<len){
        if(tok[p].type==identifier){
            if(p+1<len){
                if(tok[p+1].type=str){
                    n_inst++;
                    inst=realloc(inst,sizeof(Instruction)*n_inst);
                    varset t;
                    t.name=malloc(sizeof(char)*(1+strlen(tok[p+1].value.s)));
                    strcpy(t.name,tok[p+1].value.s);
                    inst[n_inst-1].type=inst_varset_t;
                    inst[n_inst-1].value.vs=malloc(sizeof(varset));
                    *inst[n_inst-1].value.vs=t;

                }
            }

        }
    }


}