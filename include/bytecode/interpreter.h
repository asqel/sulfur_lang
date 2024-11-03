#ifndef BC_INTERPRETER_H
#define BC_INTERPRETER_H

#include "objects.h"

extern bc_object BC_STACK[0xffff * 5];
extern int BC_STACK_LEN;

extern bc_scope_t *bc_scopes;
extern int bc_scopes_len;
extern int current_scope;

enum BC_ERROR_TYPES{
	bc_error_none_t,
	bc_error_local_var_t,
	bc_error_global_var_t,
	bc_error_op_type_t,
	bc_error_div_zero_t,
	bc_error_mod_zero_t,
	bc_error_stack_size_t
};

#define BC_STACK_TOP BC_STACK[BC_STACK_LEN - 1]
#define BC_STACK_TOP2 BC_STACK[BC_STACK_LEN - 2]

#endif