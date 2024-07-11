#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/token_class.h"
#include "../include/Ast.h"
#include "../include/utilities.h"
#include "../include/parser.h"
#include "../include/make_while.h"

extern int search_rpar(Token*t,int start);

Instruction *make_while(Token* tok, int start, int end, Instruction* inst, int* n_inst, int* p, int len){
    if(tok[*p].type == keyword && *tok[*p].value.t == while_t){
        if(!(*p + 1 < len && tok[*p + 1].type == syntax && *tok[*p + 1].value.t == par_L)){
            PRINT_ERR("ERROR expected '('  after while on line %d\n", tok[*p].line);
            exit(1);
        }
        int opening_par = *p + 1;
        int closing_par = search_rpar(tok, *p + 1);
        if (closing_par == -1){
            PRINT_ERR("ERROR missing closing ')' on line %d after while\n",tok[*p + 1].line);
            exit(1);
        }
        *p = closing_par + 1;
        if(*p < len && tok[*p].type == syntax && *tok[*p].value.t == r_brack_L){
            int opening_rbrack = *p;
            int closing_rback = search_rrbrack(tok, *p);
            if(closing_rback == -1){
                PRINT_ERR("ERROR missing closing '}' on line %d after while\n",tok[*p + 1].line);
                exit(1);

            }
            (*n_inst)++;
            inst = realloc(inst, sizeof(Instruction) * (*n_inst));
            inst[*n_inst - 1].type = inst_while_t;
			inst[*n_inst - 1].facultative = 0;
            inst[*n_inst - 1].value.wh = malloc(sizeof(While));
			inst[*n_inst - 1].line = tok[*p].line;
            ast_and_len val = tok_to_Ast(tok, opening_par + 1, closing_par);
            inst[*n_inst - 1].value.i->condition = make_ast(val.value, val.len);
            int while_index = *n_inst - 1;
            
            inst = parse(tok, opening_rbrack + 1, closing_rback, inst, n_inst);

            (*n_inst)++;
            inst = realloc(inst, sizeof(Instruction) * (*n_inst));
            inst[*n_inst - 1].type = inst_endwhile_t;
            inst[*n_inst - 1].value.endwhile = while_index;
		    inst[*n_inst - 1].facultative = 0;
            inst[while_index].value.wh->endwhile = *n_inst - 1;
            *p = closing_rback + 1;
            return inst;
        }
        else {
            (*n_inst)++;
            inst = realloc(inst, sizeof(Instruction) * (*n_inst));
            inst[*n_inst - 1].type = inst_while_t;
			inst[*n_inst - 1].facultative = 0;
			inst[*n_inst - 1].line = tok[*p].line;
            inst[*n_inst - 1].value.wh = malloc(sizeof(While));
            ast_and_len val = tok_to_Ast(tok, opening_par + 1, closing_par);
            inst[*n_inst - 1].value.i->condition = make_ast(val.value, val.len);
            int while_index = *n_inst - 1;
            
            int result = 0;
            inst = parse_next_inst(tok, start, end, inst, n_inst, p, len, &result);

            (*n_inst)++;
            inst = realloc(inst, sizeof(Instruction) * (*n_inst));
            inst[*n_inst - 1].type = inst_endwhile_t;
            inst[*n_inst - 1].value.endwhile = while_index;
		    inst[*n_inst - 1].facultative = 0;
            inst[while_index].value.wh->endwhile = *n_inst - 1;
            return inst;
        }
    }
	return inst;
}