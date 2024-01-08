#include "../include/make_funcdef.h"
#include "../include/memlib.h"
#include "../include/token_class.h"
#include "../include/instruction.h"
#include <stdio.h>
#include <stdlib.h>
#include "../include/parser.h"
#include <string.h>
#include "../include/utilities.h"


extern int search_rpar(Token*t,int start);

Instruction *make_funcdef(Token* tok, int start, int end, Instruction* inst, int* n_inst, int* p, int len){
	if (token_is(tok[*p], "def")){
		if (*p + 1 < len && tok[*p + 1].type == identifier) {
			char *name =  tok[*p + 1].value.s;	
			if (*p + 2 < len && token_is(tok[*p + 2], "(")) {
				int par_l = *p + 2;
				int par_r = search_rpar(tok, *p + 2);
				if (par_r == -1) {
					printf("ERROR missing closing ')' on line %d after def\n",tok[*p + 1].line);
					exit(1);
				}
				//check param (id / , / $)
				for (int i = par_l + 1; i < par_r; i++) {
					if (tok[i].type == identifier)
						continue;
					if (token_is(tok[i], ","))
						continue;
					if (token_is(tok[i], "$"))
						continue;
					printf("ERROR in parameter onf function definition on line %d unexpected token\n", tok[i].line);
				}
				if (par_r + 1 < len && token_is(tok[par_r + 1], "{")) {
					int br_l = par_r + 1;
					int br_r = search_rrbrack(tok, br_l);
					if (par_r == -1) {
						printf("ERROR missing closing '}' on line %d after def\n",tok[*p + 1].line);
						exit(1);
					}
					int inst_len = 0;
					Instruction *f_inst = parse(tok, br_l + 1, br_r, NULL, &inst_len);
    				f_inst = make_jmp_links(f_inst, inst_len);
					if (par_l + 1 == par_r) {
						Instruction func;
						func.line = tok[*p].line;
						func.type = inst_funcdef_t;
						func.value.fc = malloc(sizeof(Funcdef_code));
						func.value.fc->args = NULL;
						func.value.fc->args_len = 0;
						func.value.fc->args_mod = 'o';
						func.value.fc->code = f_inst;
						func.value.fc->code_len = inst_len;
						func.value.fc->info.description = NULL;
						func.value.fc->info.name = uti_strdup(name);
						(*n_inst)++;
						inst = realloc(inst, sizeof(Instruction) * (*n_inst));
						inst[*n_inst - 1] = func;
						*p = br_r + 1;
						return inst;
					}
					else {
						//only last arg can be packed arg ($args)
						int n_arg = 0;
						char **args;
						char mod = 0;
						int pakced_n = 0;
						for (int i = par_l + 1; i < par_r; i++)
							if (token_is(tok[i], "$")) mod++;
						if (mod == 0) mod = 'o';
						else if (mod == 1) mod = '+';
						else {
							printf("ERROR packed argument only accepted as last parameter on line %d\n", tok[par_r - 2].line);
							exit(1);
						}
						for (int i = par_l + 1; i < par_r; i++)
							if (tok[i].type == identifier) n_arg++;
						if (mod == '+') {
							if (!token_is(tok[par_r - 2], "$")) {
								printf("ERROR packed argument only accepted as last parameter on line %d\n", tok[par_r - 2].line);
								exit(1);
							}
						}
						args = malloc(sizeof(char *) * n_arg);
						int args_ptr = -1;
						for(int i = par_l + 1; i < par_r; i++) {
							if (tok[i].type == identifier)
								args[++args_ptr] = uti_strdup(tok[i].value.s);
						}
						Instruction func;
						func.line = tok[*p].line;
						func.type = inst_funcdef_t;
						func.value.fc = malloc(sizeof(Funcdef_code));
						func.value.fc->args = args;
						func.value.fc->args_len = n_arg;
						func.value.fc->args_mod = mod;
						func.value.fc->code = f_inst;
						func.value.fc->code_len = inst_len;
						func.value.fc->info.description = NULL;
						func.value.fc->info.name = uti_strdup(name);
						(*n_inst)++;
						inst = realloc(inst, sizeof(Instruction) * (*n_inst));
						inst[*n_inst - 1] = func;
						*p = br_r + 1;
						return inst;
					}
				}
				else {
					printf("ERROR FUNDEF#1\n");
					exit(1);
				}
			}
			else {
				printf("ERROR FUNDEF#2\n");
				exit(1);
			}
		}
		else {
			printf("ERROR FUNDEF#3\n");
			exit(1);
		}
	}
	return inst;
}


