#include "bytecode/objects.h"
#include "bytecode/interpreter.h"
#include "bytecode/memory.h"
#include "string.h"
#include "stdlib.h"
#include "bytecode/operations.h"

int bc_div_ount() {
	switch (BC_STACK_TOP.type) {
		case bc_obj_ount_t:
			if (BC_STACK_TOP.val.i == 0)
				return bc_error_div_zero_t;
			BC_STACK_TOP2.type = bc_obj_floap_t;
			BC_STACK_TOP2.val.f = (bc_arch_float)BC_STACK_TOP2.val.i / (bc_arch_float)BC_STACK_TOP.val.i;
			BC_STACK_LEN--;
			return bc_error_none_t;
		case bc_obj_bool_t:
			if (BC_STACK_TOP.val.b == 0)
				return bc_error_div_zero_t;
			//BC_STACK_TOP2.val.i /= (int)BC_STACK_TOP.val.b;
			// divide by 1 lol
			BC_STACK_TOP2.type = bc_obj_floap_t;
			BC_STACK_TOP2.val.f = (bc_arch_float)BC_STACK_TOP2.val.i;
			BC_STACK_LEN--;
			return bc_error_none_t;
		case bc_obj_floap_t:
			if (BC_STACK_TOP.val.f == 0)
				return bc_error_div_zero_t;
			BC_STACK_TOP2.type = bc_obj_floap_t;
			BC_STACK_TOP2.val.f = (bc_arch_float)BC_STACK_TOP2.val.i;
			BC_STACK_TOP2.val.f /= BC_STACK_TOP.val.f;
			BC_STACK_LEN--;
			return bc_error_none_t;
		case bc_obj_complex_t:
			if (BC_STACK_TOP.val.c[0] == 0 && BC_STACK_TOP.val.c[1] == 0)
				return bc_error_none_t;
			{
				bc_object o = {.ref_count = NULL, .type = Obj_complex_t};
				bc_arch_float bottom = BC_STACK_TOP.val.c[0] * BC_STACK_TOP.val.c[0] + BC_STACK_TOP.val.c[1] * BC_STACK_TOP.val.c[1];
				o.val.c[0] = BC_STACK_TOP2.val.i * BC_STACK_TOP.val.c[0] / bottom;
				o.val.c[1] = BC_STACK_TOP2.val.i * -BC_STACK_TOP.val.c[1] / bottom;
				BC_STACK_TOP2 = o;
			}
			BC_STACK_LEN--;
			return bc_error_none_t;
		case bc_obj_custom_t:
			return bc_div_custom();
		default:
			break;
	}
	return bc_error_op_type_t;
}

int bc_div_bool() {
	switch (BC_STACK_TOP.type) {
		case bc_obj_ount_t:
			if (BC_STACK_TOP.val.i == 0)
				return bc_error_div_zero_t;
			BC_STACK_TOP2.type = bc_obj_floap_t;
			BC_STACK_TOP2.val.f = (bc_arch_float)((bc_arch_int)BC_STACK_TOP2.val.b / (bc_arch_float)BC_STACK_TOP.val.i);
			BC_STACK_LEN--;
			return bc_error_none_t;
		case bc_obj_bool_t:
			if (BC_STACK_TOP.val.b == 0)
				return bc_error_div_zero_t;
			BC_STACK_TOP2.type = bc_obj_floap_t;
			BC_STACK_TOP2.val.f = (bc_arch_float)(bc_arch_int)BC_STACK_TOP2.val.b;
			BC_STACK_LEN--;
			return bc_error_none_t;
		case bc_obj_floap_t:
			if (BC_STACK_TOP.val.f == 0)
				return bc_error_div_zero_t;
			BC_STACK_TOP2.type = bc_obj_floap_t;
			BC_STACK_TOP2.val.f = (bc_arch_float)(bc_arch_int)BC_STACK_TOP2.val.b;
			BC_STACK_TOP2.val.f /= BC_STACK_TOP.val.f;
			BC_STACK_LEN--;
			return bc_error_none_t;
		case bc_obj_complex_t:
			if (BC_STACK_TOP.val.c[0] == 0 && BC_STACK_TOP.val.c[1] == 0)
				return bc_error_div_zero_t;
			{
				bc_object o = {.ref_count = NULL, .type = Obj_complex_t};
				bc_arch_float bottom = BC_STACK_TOP.val.c[0] * BC_STACK_TOP.val.c[0] + BC_STACK_TOP.val.c[1] * BC_STACK_TOP.val.c[1];
				o.val.c[0] = ((bc_arch_int)BC_STACK_TOP2.val.b) * BC_STACK_TOP.val.c[0] / bottom;
				o.val.c[1] = ((bc_arch_int)BC_STACK_TOP2.val.b) * -BC_STACK_TOP.val.c[1] / bottom;
				BC_STACK_TOP2 = o;
			}
			BC_STACK_LEN--;
			return bc_error_none_t;
		case bc_obj_custom_t:
			return bc_div_custom();
		default:
			break;
	}
	return bc_error_op_type_t;
}

int bc_div_string() {
	switch (BC_STACK_TOP.type) {
		case bc_obj_custom_t:
			return bc_div_custom();
		default:
			break;
	}
	return bc_error_op_type_t;
}

int bc_div_floap() {
	switch (BC_STACK_TOP.type) {
		case bc_obj_custom_t:
			return bc_div_custom();
		default:
			break;
	}
	return bc_error_op_type_t;
}

int bc_div_complex() {
	switch (BC_STACK_TOP.type) {
		case bc_obj_custom_t:
			return bc_div_custom();
		default:
			break;
	}
	return bc_error_op_type_t;
}

int bc_div_list() {
	switch (BC_STACK_TOP.type) {
		case bc_obj_custom_t:
			return bc_div_custom();
		default:
			break;
	}
	return bc_error_op_type_t;
}

int bc_div_funcdef() {
	switch (BC_STACK_TOP.type) {
		case bc_obj_custom_t:
			return bc_div_custom();
		default:
			break;
	}
	return bc_error_op_type_t;
}

int bc_div_module() {
	switch (BC_STACK_TOP.type) {
		case bc_obj_custom_t:
			return bc_div_custom();
		default:
			break;
	}
	return bc_error_op_type_t;
}

int bc_div_custom() {
	switch (BC_STACK_TOP.type) {
		default:
			break;
	}
	return bc_error_op_type_t;
}

