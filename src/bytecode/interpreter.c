#include "bytecode.h"
#include "bytecode/objects.h"
#include "bytecode/memory.h"
#include "bytecode/interpreter.h"
#include "bytecode/operations.h"
#include <stdint.h>

bc_object BC_STACK[0xffff * 5];
int BC_STACK_LEN = 0;

bc_scope_t *bc_scopes = NULL;
int bc_scopes_len = 0;
int current_scope = 0;

void init_scopes(int var_number) {
	bc_scopes = malloc(sizeof(bc_scope_t));
	bc_scopes_len = 1;
	bc_scopes[0].len = var_number;
	bc_scopes[0].values = malloc(sizeof(bc_object) * var_number);
	for (int i = 0; i < var_number; i++) {
		bc_scopes[0].values[i] = bc_unset;
	}
}


bc_object bc_get_local_var(bc_arch_uint index) {
	return bc_scopes[current_scope].values[index];
}

bc_object bc_get_global_var(bc_arch_uint index) {
	return bc_scopes[0].values[index];
}

bc_arch_int bc_get_inst_arg(unsigned char *code) {
	code++;
	return *((bc_arch_int *)code);
}

void bc_stack_to_bool() {
	switch (BC_STACK_TOP.type) {
		case bc_obj_nil_t:
			BC_STACK_TOP.type = bc_obj_bool_t;
			BC_STACK_TOP.val.b = 0;
			break;
		case bc_obj_ount_t:
			BC_STACK_TOP.type = bc_obj_bool_t;
			BC_STACK_TOP.val.b = BC_STACK_TOP.val.i != 0 ? 1 : 0;
			break;
		case bc_obj_bool_t:
			break;
		case bc_obj_string_t:
			{
				int is_empty = strlen(BC_STACK_TOP.val.s) > 0 ? 1 : 0;
				free(BC_STACK_TOP.val.s);
				BC_STACK_TOP.type = bc_obj_bool_t;
				BC_STACK_TOP.val.b = is_empty;
			}
			break;
		case bc_obj_floap_t:
			BC_STACK_TOP.type = bc_obj_bool_t;
			BC_STACK_TOP.val.b = BC_STACK_TOP.val.f != 0 ? 1 : 0;
			break;
		case bc_obj_complex_t:
			BC_STACK_TOP.type = bc_obj_bool_t;
			BC_STACK_TOP.val.b = BC_STACK_TOP.val.c[0] != 0 || BC_STACK_TOP.val.c[1] != 0 ? 1 : 0;
			break;
		case bc_obj_list_t:
			{
				int is_empty = BC_STACK_TOP.val.li->len > 0 ? 1 : 0;
				bc_remove_ref(BC_STACK_TOP);
				BC_STACK_TOP.type = bc_obj_bool_t;
				BC_STACK_TOP.val.b = is_empty;
			}
			break;
		default:
			BC_STACK[BC_STACK_LEN] = bc_new_bool(1);
			BC_STACK_LEN++;
			break;
	}
}

void bytecode_exec(bytecode_info bytecode) {
	unsigned char *code = bytecode.code;
	int type_error = 0;

	bc_object tmp;
	while (1) {
		switch (*code) {
			case OPC_nop:
				code++;
				break;
			case OPC_push_str:
				//BC_STACK[BC_STACK_LEN] = bc_new_string(uti_strdup((char *)bytecode.byte_code.vals + bc_get_inst_arg(code)));
				BC_STACK[BC_STACK_LEN] = bc_new_string(uti_strdup(""));
				BC_STACK_LEN++;
				code = code + 9;
				break;
			case OPC_push_var:
				tmp = bc_get_local_var(bc_get_inst_arg(code));
				if (tmp.type == bc_obj_unset_t) {
					type_error = bc_error_local_var_t;
					goto error;
				}
				BC_STACK[BC_STACK_LEN] = bc_get_surface_copy(tmp);
				BC_STACK_LEN++;
				code = code + 9;
				break;
			case OPC_push_global_var:
				BC_STACK[BC_STACK_LEN] = bc_get_surface_copy(bc_get_global_var(bc_get_inst_arg(code)));
				BC_STACK_LEN++;
				code = code + 9;
				break;
			case OPC_push_0b:
				BC_STACK[BC_STACK_LEN] = bc_new_bool(0);
				BC_STACK_LEN++;
				code++;
				break;
			case OPC_push_1b:
				BC_STACK[BC_STACK_LEN] = bc_new_bool(1);
				BC_STACK_LEN++;
				code++;
				break;
			case OPC_push_nil:
				BC_STACK[BC_STACK_LEN] = bc_nil;
				BC_STACK_LEN++;
				code++;
				break;
			case OPC_push_i:
				BC_STACK[BC_STACK_LEN] = bc_new_complex(0, 1);
				BC_STACK_LEN++;
				code++;
				break;
			case OPC_push_ount:
				BC_STACK[BC_STACK_LEN] = bc_new_ount(bc_get_inst_arg(code));
				BC_STACK_LEN++;
				code = code + 9;
				break;
			case OPC_push_floap:
				BC_STACK[BC_STACK_LEN] = bc_new_floap(*((double *)(code + 1)));
				BC_STACK_LEN++;
				code = code + 9;
				break;
			case OPC_pop:
				BC_STACK_LEN--;
				bc_remove_ref(BC_STACK[BC_STACK_LEN]);
				code++;
				break;
			case OPC_op_add:
				type_error = bc_doop_add();
				code++;
				break;
			case OPC_op_sub:
				type_error = bc_doop_sub();
				code++;
				break;
			// add all the other operations

			case OPC_rjmp:
				code += bc_get_inst_arg(code);
				break;
			case OPC_rjmpif:
				bc_stack_to_bool();
				if (BC_STACK_TOP.val.b != 0)
					code += bc_get_inst_arg(code);
				BC_STACK_LEN--;
				break;
			case OPC_rjmpifn:
				bc_stack_to_bool();
				if (BC_STACK_TOP.val.b == 0)
					code += bc_get_inst_arg(code);
				BC_STACK_LEN--;
				break;
			

		}
		if (type_error != bc_error_none_t)
		goto error;
	}
	return ;
	error:
		// TODO : free everything
		exit(1);
}
