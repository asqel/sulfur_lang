#include "../include/make_requested_vars.h"
#include "../include/sulfur.h"
#include "../include/instruction.h"
#include "../include/utilities.h"
#include "../include/Ast.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int add_requested_var(char *var) {
	int len = 0;
	while (CTX.requested_vars[len] != NULL) {
		if (!strcmp(CTX.requested_vars[len], var))
			return len;
		len++;
	}
	CTX.requested_vars = realloc(CTX.requested_vars, sizeof(char *) * (len + 2));
	CTX.requested_vars[len] = strdup(var);
	CTX.requested_vars[len + 1] = NULL;
	return len;
}

Instruction *make_requested_vars(Instruction *inst, int len) {
	for (int i = 0; i < len; i++)  {
		make_req_vars_inst(inst[i]);
	}
	return inst;
}


void make_req_vars_inst(Instruction inst) {
	switch (inst.type) {
		case inst_while_t:
			make_req_vars_ast(inst.value.wh->condition);
			break;
		case inst_for_t:
			make_req_vars_ast(inst.value.fo->end);
			make_req_vars_ast(inst.value.fo->start);
			break;	

		case inst_if_t:
			make_req_vars_ast(inst.value.i->condition);
			break;

		case inst_elif_t:
			make_req_vars_ast(inst.value.el->condition);
			break;

		case inst_expr_t:
			make_req_vars_ast(inst.value.expr);
			break;

		case inst_return_t:
			make_req_vars_ast(inst.value.ret);
			break;

		default:
		break;
	}
}

void make_req_vars_ast(Ast *x) {
	if (x->type == Ast_varcall_t) {
		int v = add_requested_var(x->root.varcall);
		free(x->root.varcall);
		x->root.var_idx = v;
		x->type = Ast_varcall_idx_t;
		return ;
	}
	if (x->type == Ast_dot_t) {
		make_req_vars_ast(x->left);
		return ;
	}
	if (x->left != NULL)
		make_req_vars_ast(x->left);
	if (x->right != NULL)
		make_req_vars_ast(x->right);
	if (x->type == Ast_anonym_func_t) {
		make_requested_vars(x->root.ano_func->code, x->root.ano_func->code_len);
		return ;
	}
	if (x->type == Ast_funccall_t) {
		
	}
}