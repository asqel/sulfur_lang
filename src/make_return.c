#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/token_class.h"
#include "../include/Ast.h"
#include "../include/utilities.h"
#include "../include/parser.h"
#include "../include/make_return.h"

extern int find_semicol(Token *tok, int start);

Instruction *make_return(Token *tok, int start, int end, Instruction *inst, int *n_inst, int *p, int len){
	if(tok[*p].type == keyword && *tok[*p].value.t == return_t){
        int n = find_semicol(tok, *p);
        if(n == -1){
            PRINT_ERR("ERROR missing ';' on line %d after return\n",tok[*p].line);
            exit(1);
        }
        if(n == *p + 1){
            //if its "return ;" then it is considered to be "return nil;"
            (*n_inst)++;
            inst = realloc(inst, sizeof(Instruction) * (*n_inst));
            inst[*n_inst - 1].type = inst_return_t;
            inst[*n_inst - 1].value.ret = malloc(sizeof(Ast));
            inst[*n_inst - 1].value.ret->left = NULL;
            inst[*n_inst - 1].value.ret->right = NULL;
			inst[*n_inst - 1].facultative = 0;

            inst[*n_inst - 1].value.ret->type = Ast_object_t;

            inst[*n_inst - 1].value.ret->root.obj = malloc(sizeof(Object));
            inst[*n_inst - 1].value.ret->root.obj->type = Obj_nil_t;
            *p = n + 1;
            return inst;
        }
        else{
            ast_and_len val = tok_to_Ast(tok, *p + 1, n);
            Ast *x = make_ast(val.value, val.len);
            (*n_inst)++;
            inst = realloc(inst, sizeof(Instruction) * (*n_inst));
            inst[*n_inst - 1].type = inst_return_t;
			inst[*n_inst - 1].facultative = 0;
            inst[*n_inst - 1].value.ret = x;
            
            *p = n + 1;
            return inst;
        }

    }
    return inst;
        
}