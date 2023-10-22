#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/token_class.h"
#include "../include/Ast.h"
#include "../include/utilities.h"
#include "../include/parser.h"
#include "../include/make_if.h"

extern int count_elseelif(Token*tok,int p);
extern int search_rpar(Token*t,int start);

Instruction* make_if(Token* tok, int start, int end, Instruction* inst, int* n_inst, int* p, int len){
    if(tok[*p].type == keyword && *tok[*p].value.t == if_t){
        if(*p + 1 < len && tok[*p + 1].type == syntax && *tok[*p + 1].value.t == par_L){
            int opening_par = *p + 1;
            int closing_par = search_rpar(tok, *p + 1);
            if (closing_par == -1){
                printf("ERROR missing closing ')' on line %d after if\n",tok[*p + 1].line);
                exit(1);
            }
            *p = closing_par+1;
            if(*p < len&&tok[*p].type == syntax && *tok[*p].value.t == r_brack_L){
                int opening_rbrack = *p;
                int closing_rback = search_rrbrack(tok, *p);
                if(closing_rback == -1){
                    printf("ERROR missing closing '}' on line %d after if\n",tok[*p + 1].line);
                    exit(1);
                }
                (*n_inst)++;
                inst = realloc(inst, sizeof(Instruction) * (*n_inst));
                inst[*n_inst - 1].type = inst_if_t;
                inst[*n_inst - 1].value.i = malloc(sizeof(If));
                ast_and_len val = tok_to_Ast(tok,opening_par + 1, closing_par);
                inst[*n_inst - 1].value.i->condition = make_ast(val.value, val.len);
                int if_index = *n_inst - 1;
                
                inst = parse(tok, opening_rbrack + 1, closing_rback, inst, n_inst);
                (*n_inst)++;
                inst = realloc(inst, sizeof(Instruction) * (*n_inst));
                inst[*n_inst - 1].type = inst_endif;
                inst[if_index].value.i->endif_p = *n_inst - 1;
                int* endifelse_ps = malloc(sizeof(int) * (1 + count_elseelif(tok, closing_rback + 1)));//list des endif ou on doit leur rajouter le pointer vers un endifelse
                int endif_n = 1;
                endifelse_ps[0] = *n_inst - 1;
                *p = closing_rback + 1;
                while(*p < len && tok[*p].type == keyword && *tok[*p].value.t == elif_t){
                    if(*p + 1 < len && tok[*p + 1].type == syntax && *tok[*p + 1].value.t == par_L){
                        opening_par = *p + 1;
                        closing_par = search_rpar(tok, opening_par);
                        if(closing_par == -1){
                            printf("ERROR missing closing ')' on line %d after elif\n", tok[opening_par].line);
                        }
                        *p = closing_par+1;
                        if(*p < len && tok[*p].type == syntax && *tok[*p].value.t == r_brack_L){
                            int opening_rbrack = *p;
                            int closing_rback = search_rrbrack(tok, *p);
                            if(closing_rback == -1){
                                printf("ERROR missing closing '}' on line %d after elif\n",tok[*p + 1].line);
                                exit(1);
                            }
                            (*n_inst)++;
                            inst = realloc(inst, sizeof(Instruction) * (*n_inst));
                            inst[*n_inst - 1].type = inst_elif_t;
                            inst[*n_inst - 1].value.el = malloc(sizeof(Elif));
                            ast_and_len val = tok_to_Ast(tok,opening_par + 1, closing_par);
                            inst[*n_inst - 1].value.el->condition = make_ast(val.value, val.len);
                            int elif_index = *n_inst - 1;
                            
                            inst = parse(tok, opening_rbrack + 1, closing_rback, inst, n_inst);
        
                            (*n_inst)++;
                            inst = realloc(inst,sizeof(Instruction) * (*n_inst));
                            inst[*n_inst - 1].type = inst_endif;
                            inst[elif_index].value.el->endif_p = *n_inst - 1;
                            *p = closing_rback + 1;
                            endif_n++;
                            endifelse_ps[endif_n - 1] = *n_inst - 1;//ffaut faire ca en bas aussi avec le else oublie pas hein !
                        }
                        else{
                            printf("ERROR missing opening '{' on line %d after elif\n",tok[*p-1].line);
                            exit(1);
                        }
                    }
                    else{
                        printf("ERROR missing opening '(' on line %d after elif\n",tok[*p].line);
                        exit(1);
                    }
                }
                if(*p < len && tok[*p].type == keyword && *tok[*p].value.t == else_t){
                    if(*p + 1 < len && tok[*p + 1].type == syntax && *tok[*p + 1].value.t == r_brack_L){
                        opening_rbrack = *p + 1;
                        closing_rback = search_rrbrack(tok, opening_rbrack);
                        if(closing_rback == -1){
                            printf("ERROR missing closing '}' on line %d after else\n", tok[*p + 1].line);
                            exit(1);
                        }
                        (*n_inst)++;
                        inst = realloc(inst, sizeof(Instruction) * (*n_inst));
                        inst[*n_inst - 1].type = inst_else_t;
                        int else_index = *n_inst - 1;
                        inst = parse(tok, opening_rbrack + 1, closing_rback, inst, n_inst);
                        (*n_inst)++;
                        inst = realloc(inst, sizeof(Instruction) * (*n_inst));
                        inst[*n_inst - 1].type = inst_endif;
                        inst[else_index].value.endif = *n_inst - 1;
                        
                        *p = closing_rback + 1;
                        endif_n++;
                        endifelse_ps[endif_n - 1] = *n_inst - 1;
                    }
                    else{
                        printf("ERROR missing opening '{' on line %d after else\n",tok[*p].line);
                        exit(1);
                    }
                }
                (*n_inst)++;
                inst = realloc(inst,sizeof(Instruction) * (*n_inst));
                inst[*n_inst - 1].type = inst_endifelse;
                inst[*n_inst - 1].value.endifelse = if_index;
                for(int i = 0; i < endif_n; i++){
                    inst[endifelse_ps[i]].value.endifelse = *n_inst - 1;
                }
                free(endifelse_ps);
                return inst;
            }
        }
        else{
            printf("ERROR on if missing opening '(' line %d\n",tok[*p].line);
            exit(1);
        }
    }
    return inst;
}