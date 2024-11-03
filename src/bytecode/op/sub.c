#include "bytecode/objects.h"
#include "bytecode/interpreter.h"
#include "bytecode/memory.h"
#include "string.h"
#include "stdlib.h"
#include "bytecode/operations.h"

int bc_sub_ount() {
	switch (BC_STACK_TOP.type) {
		case bc_obj_ount_t:
			BC_STACK_TOP2.val.i -= BC_STACK_TOP.val.i;
			BC_STACK_LEN--;
			return bc_error_none_t;
		case bc_obj_bool_t:
			BC_STACK_TOP2.val.i -= BC_STACK_TOP.val.b;
			BC_STACK_LEN--;
			return bc_error_none_t;
		case bc_obj_floap_t:
			BC_STACK_TOP2.type = bc_obj_floap_t;
			BC_STACK_TOP2.val.f = (bc_arch_float)BC_STACK_TOP2.val.i;
			BC_STACK_TOP2.val.f -= BC_STACK_TOP.val.f;
			BC_STACK_LEN--;
			return bc_error_none_t;
		case bc_obj_complex_t:
			{
				int x = BC_STACK_TOP2.val.i;
				BC_STACK_TOP2.type = bc_obj_complex_t;
				BC_STACK_TOP2.val.c[0] = x - BC_STACK_TOP.val.c[0];
				BC_STACK_TOP2.val.c[1] = 0 - BC_STACK_TOP.val.c[1];
			}
			BC_STACK_LEN--;
			return bc_error_none_t;
		case bc_obj_custom_t:
			return bc_sub_custom();
		default:
			break;
	}
	return bc_error_op_type_t;
}

int bc_sub_bool() {
	switch (BC_STACK_TOP.type) {
		case bc_obj_ount_t:
			BC_STACK_TOP2.type = bc_obj_ount_t;
			BC_STACK_TOP2.val.i = (bc_arch_int)BC_STACK_TOP2.val.b - BC_STACK_TOP.val.i;
			BC_STACK_LEN--;
			return bc_error_none_t;
		case bc_obj_bool_t:
			BC_STACK_TOP2.type = bc_obj_ount_t;
			BC_STACK_TOP2.val.i = (bc_arch_int) BC_STACK_TOP2.val.b - BC_STACK_TOP.val.b;
			BC_STACK_LEN--;
			return bc_error_none_t;
		case bc_obj_string_t:
			return bc_error_op_type_t;
		case bc_obj_floap_t:
			BC_STACK_TOP2.type = bc_obj_floap_t;
			BC_STACK_TOP2.val.f = (bc_arch_float)(bc_arch_int)BC_STACK_TOP2.val.b;
			BC_STACK_TOP2.val.f -= BC_STACK_TOP.val.f;
			BC_STACK_LEN--;
			return bc_error_none_t;
		case bc_obj_complex_t:
			{
				int x = BC_STACK_TOP2.val.b;
				BC_STACK_TOP2.type = bc_obj_complex_t;
				BC_STACK_TOP2.val.c[0] = x - BC_STACK_TOP.val.c[0];
				BC_STACK_TOP2.val.c[1] = 0 - BC_STACK_TOP.val.c[1];
			}
			BC_STACK_LEN--;
			return bc_error_none_t;
		case bc_obj_custom_t:
			return bc_sub_custom();
		default:
			break;
	}
	return bc_error_op_type_t;
}

int bc_sub_string() {
	switch (BC_STACK_TOP.type) {
		case bc_obj_custom_t:
			return bc_sub_custom();
		default:
			break;
	}
	return bc_error_op_type_t;
}

int bc_sub_floap() {
	switch (BC_STACK_TOP.type) {
		case bc_obj_ount_t:
			BC_STACK_TOP2.val.f -= BC_STACK_TOP.val.i;
			BC_STACK_LEN--;
			return bc_error_none_t;
		case bc_obj_bool_t:
			BC_STACK_TOP2.val.f -= (bc_arch_int)BC_STACK_TOP.val.b;
			BC_STACK_LEN--;
			return bc_error_none_t;
		case bc_obj_floap_t:
			BC_STACK_TOP2.val.f -= BC_STACK_TOP.val.f;
			BC_STACK_LEN--;
			return bc_error_none_t;
		case bc_obj_complex_t:
			{
				bc_arch_float x = BC_STACK_TOP2.val.f;
				BC_STACK_TOP2.type = bc_obj_complex_t;
				BC_STACK_TOP2.val.c[0] = x - BC_STACK_TOP.val.c[0];
				BC_STACK_TOP2.val.c[1] = 0 - BC_STACK_TOP.val.c[1];
			}
			BC_STACK_LEN--;
			return bc_error_none_t;
		case bc_obj_custom_t:
			return bc_sub_custom();
		default:
			break;
	}
	return bc_error_op_type_t;
}

int bc_sub_complex() {
	switch (BC_STACK_TOP.type) {
		case bc_obj_ount_t:
			BC_STACK_TOP2.val.c[0] -= BC_STACK_TOP.val.i;
			BC_STACK_LEN--;
			return bc_error_none_t;
		case bc_obj_bool_t:
			BC_STACK_TOP2.val.c[0] -= (int)BC_STACK_TOP.val.b;
			BC_STACK_LEN--;
			return bc_error_none_t;
		case bc_obj_floap_t:
			BC_STACK_TOP2.val.c[0] -= BC_STACK_TOP.val.f;
			BC_STACK_LEN--;
			return bc_error_none_t;
		case bc_obj_complex_t:
			BC_STACK_TOP2.val.c[0] -= BC_STACK_TOP.val.c[0];
			BC_STACK_TOP2.val.c[1] -= BC_STACK_TOP.val.c[1];
			BC_STACK_LEN--;
			return bc_error_none_t;
		case bc_obj_custom_t:
			return bc_sub_custom();
		default:
			break;
	}
	return bc_error_op_type_t;
}

int bc_sub_list() {
	switch (BC_STACK_TOP.type) {
		case bc_obj_custom_t:
			return bc_sub_custom();
		default:
			break;
	}
	return bc_error_op_type_t;
}

int bc_sub_funcdef() {
	switch (BC_STACK_TOP.type) {
		case bc_obj_custom_t:
			return bc_sub_custom();
		default:
			break;
	}
	return bc_error_op_type_t;
}

int bc_sub_module() {
	switch (BC_STACK_TOP.type) {
		case bc_obj_custom_t:
			return bc_sub_custom();
		default:
			break;
	}
	return bc_error_op_type_t;
}

int bc_sub_custom() {
	switch (BC_STACK_TOP.type) {
		default:
			break;
	}
	return bc_error_op_type_t;
}

