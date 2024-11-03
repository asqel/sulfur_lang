#include "../../include/bytecode/operations.h"
#include "../../include/bytecode/interpreter.h"



int bc_doop_add() {
	if (BC_STACK_LEN < 2)
		return bc_error_stack_size_t;
	switch (BC_STACK[BC_STACK_LEN - 2].type) {
			case bc_obj_nil_t:
				return bc_error_op_type_t;
			case bc_obj_ount_t:
				return bc_add_ount();
			case bc_obj_bool_t:
				return bc_add_bool();
			case bc_obj_string_t:
				return bc_add_string();
			case bc_obj_floap_t:
				return bc_add_floap();
			case bc_obj_complex_t:
				return bc_add_complex();
			case bc_obj_list_t:
				return bc_add_list();
			case bc_obj_funcdef_t:
				return bc_add_funcdef();
			case bc_obj_module_t:
				return bc_add_module();
			case bc_obj_custom_t:
				return bc_add_custom();
			default:
				break;
	}
	return bc_error_op_type_t;
}

int bc_doop_sub() {
	if (BC_STACK_LEN < 2)
		return bc_error_stack_size_t;
	switch (BC_STACK[BC_STACK_LEN - 2].type) {
			case bc_obj_nil_t:
				return bc_error_op_type_t;
			case bc_obj_ount_t:
				return bc_sub_ount();
			case bc_obj_bool_t:
				return bc_sub_bool();
			case bc_obj_string_t:
				return bc_sub_string();
			case bc_obj_floap_t:
				return bc_sub_floap();
			case bc_obj_complex_t:
				return bc_sub_complex();
			case bc_obj_list_t:
				return bc_sub_list();
			case bc_obj_funcdef_t:
				return bc_sub_funcdef();
			case bc_obj_module_t:
				return bc_sub_module();
			case bc_obj_custom_t:
				return bc_sub_custom();
			default:
				break;
	}
	return bc_error_op_type_t;
}
int bc_doop_mul() {
	if (BC_STACK_LEN < 2)
		return bc_error_stack_size_t;
	switch (BC_STACK[BC_STACK_LEN - 2].type) {
			case bc_obj_nil_t:
				return bc_error_op_type_t;
			case bc_obj_ount_t:
				return bc_mul_ount();
			case bc_obj_bool_t:
				return bc_mul_bool();
			case bc_obj_string_t:
				return bc_mul_string();
			case bc_obj_floap_t:
				return bc_mul_floap();
			case bc_obj_complex_t:
				return bc_mul_complex();
			case bc_obj_list_t:
				return bc_mul_list();
			case bc_obj_funcdef_t:
				return bc_mul_funcdef();
			case bc_obj_module_t:
				return bc_mul_module();
			case bc_obj_custom_t:
				return bc_mul_custom();
			default:
				break;
	}
	return bc_error_op_type_t;
}