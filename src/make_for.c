#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/token_class.h"
#include "../include/Ast.h"
#include "../include/utilities.h"
#include "../include/parser.h"
#include "../include/make_for.h"

extern int search_rpar(Token*t,int start);

Instruction *make_for(Token* tok, int start, int end, Instruction* inst, int* n_inst, int* p, int len){
	if(tok[*p].type == keyword && *tok[*p].value.t == for_t){
        if(*p + 1 < len && tok[*p + 1].type == syntax && *tok[*p+1].value.t == par_L){
            int n = search_rpar(tok, *p + 1);
            if(n == -1){
                printf("ERROR missing closing ')' on line %d after for\n",tok[*p + 1].line);
                exit(1);
            }
            if(n == *p + 2){
                printf("ERROR empty for-statement on line %d after for\nCorrect for-statement: for(i from a to b){\n",tok[*p + 1].line);
                exit(1);
            }
            if(*p + 2 < len && tok[*p + 2].type == identifier){
                int id_idx = *p + 2;
                if(*p + 3 < len && tok[*p + 3].type == keyword && *tok[*p + 3].value.t == from_t){
                    int n2 = -1;
                    for(int i = *p + 4; i < len; i++){
                        if(tok[i].type == keyword && *tok[i].value.t == to_t){
                            n2 = i;
                            break;
                        }
                    }
                    if(n2 == -1){
                        printf("ERROR missing 'to' in for-statement on line %d after for\nCorrect for-statement: for(i from a to b){\n",tok[*p + 1].line);
                        exit(1);
                    }
                    if(n + 1 < len && tok[n + 1].type == syntax && *tok[n + 1].value.t == r_brack_L){
                        int k = search_rrbrack(tok, n + 1);
                        if(k == -1){
                            printf("ERROR missing closing '}' after for-statement on line %d after for\nCorrect for-statement: for(i from a to b){\n",tok[n + 1].line);
                            exit(1);
                        }
                        ast_and_len val = tok_to_Ast(tok, *p + 4, n2);
                        Ast *x = make_ast(val.value, val.len);
                        val = tok_to_Ast(tok, n2 + 1, n);
                        Ast *x2 = make_ast(val.value, val.len);

                        (*n_inst)++;
                        inst = realloc(inst, sizeof(Instruction) * (*n_inst));
                        inst[*n_inst - 1].type = inst_for_t;
                        inst[*n_inst - 1].value.fo = malloc(sizeof(For));
                        
                        inst[*n_inst - 1].value.fo->var_name = malloc(sizeof(char) * (1 + strlen(tok[id_idx].value.s)));
                        strcpy(inst[*n_inst - 1].value.fo->var_name, tok[id_idx].value.s);
                        
                        inst[*n_inst - 1].value.fo->start = x;
                        inst[*n_inst - 1].value.fo->end = x2;

                        int for_idx = *n_inst - 1;

                        inst = parse(tok, n + 2, k, inst, n_inst);

                        (*n_inst)++;
                        inst = realloc(inst, sizeof(Instruction) * (*n_inst));
                        inst[*n_inst - 1].type = inst_endfor_t;
                        inst[*n_inst - 1].value.endfor = for_idx;
                        inst[for_idx].value.fo->endfor = *n_inst - 1;
                        *p = k + 1;
						return inst;
                    }
                    else{
                        ast_and_len val = tok_to_Ast(tok, *p + 4, n2);
                        Ast *x = make_ast(val.value, val.len);
                        val = tok_to_Ast(tok, n2 + 1, n);
                        Ast *x2 = make_ast(val.value, val.len);

                        (*n_inst)++;
                        inst = realloc(inst, sizeof(Instruction) * (*n_inst));
                        inst[*n_inst - 1].type = inst_for_t;
                        inst[*n_inst - 1].value.fo = malloc(sizeof(For));
                        
                        inst[*n_inst - 1].value.fo->var_name = malloc(sizeof(char) * (1 + strlen(tok[id_idx].value.s)));
                        strcpy(inst[*n_inst - 1].value.fo->var_name, tok[id_idx].value.s);
                        
                        inst[*n_inst - 1].value.fo->start = x;
                        inst[*n_inst - 1].value.fo->end = x2;

                        int for_idx = *n_inst - 1;

                        int result = 0;
                        *p = n + 1;
                        inst = parse_next_inst(tok, start, end, inst, n_inst, p, len, &result);
                        (*n_inst)++;
                        inst = realloc(inst, sizeof(Instruction) * (*n_inst));
                        inst[*n_inst - 1].type = inst_endfor_t;
                        inst[*n_inst - 1].value.endfor = for_idx;
                        inst[for_idx].value.fo->endfor = *n_inst - 1;
						return inst;
                    }
                }

            }
            else{
                printf("ERROR in for-statement on line %d after for\nCorrect for-statement: for(i from a to b){\n",tok[*p + 1].line);
                exit(1);
            }
        }
    }
	return inst;
}