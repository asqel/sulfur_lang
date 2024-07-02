#include "../include/make_requested_vars.h"
#include "../include/sulfur.h"
#include "../include/instruction.h"
#include "../include/utilities.h"
#include "../include/Ast.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int add_requested_var(char *var) {
	if (var[0] == '#')
		var = &(var[1]);
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

int add_requested_var_right(char *var) {
	if (var[0] == '#')
		var = &(var[1]);
	int len = 0;
	while (CTX.requested_vars_right[len] != NULL) {
		if (!strcmp(CTX.requested_vars_right[len], var))
			return len;
		len++;
	}
	CTX.requested_vars_right = realloc(CTX.requested_vars_right, sizeof(char *) * (len + 2));
	CTX.requested_vars_right[len] = strdup(var);
	CTX.requested_vars_right[len + 1] = NULL;
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
/*
globals are negative - 1
*/
void make_req_vars_ast(Ast *x) {
	if (x->type == Ast_varcall_t) {
		char is_global = (x->root.varcall[0] == '#');
		int v = add_requested_var(x->root.varcall);
		free(x->root.varcall);
		x->root.var_idx = is_global ? -v - 1 : v;
		x->type = Ast_varcall_idx_t;
		return ;
	}
	if (x->type == Ast_dot_t) {
		make_req_vars_ast(x->left);
		if (x->right->type != Ast_varcall_t && x->right->type != Ast_funccall_t ) {
			PRINT_ERR("ERROR dot operator accept only identifier as right operend\n");
			exit(1);
		}
		if (x->right->type == Ast_varcall_t && x->right->root.varcall[0] == '#') {
			PRINT_ERR("ERROR dot operator accept only identifier as right operend (no #)\n");
			exit(1);
		}
		if (x->right->type == Ast_varcall_t) {
			char *old = x->right->root.varcall;
			x->right->root.var_idx = add_requested_var_right(x->right->root.varcall);
			x->right->type = Ast_varcall_idx_t;
			free(old);
			return ;
		}
		x->right->root.fun->name_idx = add_requested_var_right(x->right->root.fun->name);
		for (int i = 0; i < x->right->root.fun->nbr_arg; i++)
			make_req_vars_ast(&(x->right->root.fun->args[i]));
		return ;
	}
	if (x->type == Ast_anonym_func_t) {
		make_requested_vars(x->root.ano_func->code, x->root.ano_func->code_len);
		return ;
	}
	if (x->type == Ast_funccall_t) {
		x->root.fun->name_idx = add_requested_var(x->root.fun->name);
		for (int i = 0; i < x->root.fun->nbr_arg; i++)
			make_req_vars_ast(&(x->root.fun->args[i]));
		return;
	}
	if (x->type == Ast_anonym_func_t) {
		make_requested_vars(x->root.ano_func->code, x->root.ano_func->code_len);
		return;
	}
	if (x->left != NULL)
		make_req_vars_ast(x->left);
	if (x->right != NULL)
		make_req_vars_ast(x->right);
}