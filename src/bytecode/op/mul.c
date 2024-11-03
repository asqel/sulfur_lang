#include "bytecode/objects.h"
#include "bytecode/interpreter.h"
#include "bytecode/memory.h"
#include "string.h"
#include "stdlib.h"
#include "bytecode/operations.h"

int bc_mul_ount() {
	switch (BC_STACK_TOP.type) {
		case bc_obj_ount_t:
			BC_STACK_TOP2.val.i *= BC_STACK_TOP.val.i;
			BC_STACK_LEN--;
			return bc_error_none_t;
		case bc_obj_bool_t:
			BC_STACK_TOP2.val.i *= (int)BC_STACK_TOP.val.b;
			BC_STACK_LEN--;
			return bc_error_none_t;
		case bc_obj_floap_t:
			BC_STACK_TOP2.type = bc_obj_floap_t;
			BC_STACK_TOP2.val.f = (bc_arch_float)BC_STACK_TOP2.val.i;
			BC_STACK_TOP2.val.f *= BC_STACK_TOP.val.f;
			BC_STACK_LEN--;
			return bc_error_none_t;
		case bc_obj_complex_t:
			{
				bc_object o = {.ref_count = NULL, .type = Obj_complex_t};
				o.val.c[0] = BC_STACK_TOP.val.c[0] * BC_STACK_TOP2.val.i;
				o.val.c[1] = BC_STACK_TOP.val.c[1] * BC_STACK_TOP2.val.i;
				BC_STACK_TOP2 = o;
			}
			BC_STACK_LEN--;
			return bc_error_none_t;
		case bc_obj_string_t:
			int len = strlen(BC_STACK_TOP.val.s);
			char *s = malloc(sizeof(char) * (len * BC_STACK_TOP2.val.i + 1));
			int end = 0;
			for (int i = 0; i < BC_STACK_TOP2.val.i; i++) {
				for (int k = 0; k < len; k++) {
					s[end] = BC_STACK_TOP.val.s[k];
					end++;
				}
			}
			free(BC_STACK_TOP.val.s);
			BC_STACK_TOP2.val.s = s;
			BC_STACK_TOP2.type = bc_obj_string_t;
			BC_STACK_LEN--;
			return bc_error_none_t;
		case bc_obj_custom_t:
			return bc_mul_custom();
		default:
			break;
	}
	return bc_error_op_type_t;
}

int bc_mul_bool() {
	switch (BC_STACK_TOP.type) {
		case bc_obj_ount_t:
			BC_STACK_TOP2.type = bc_obj_ount_t;
			BC_STACK_TOP2.val.i = (bc_arch_int)BC_STACK_TOP2.val.b * BC_STACK_TOP.val.i;
			BC_STACK_LEN--;
			return bc_error_none_t;
		case bc_obj_bool_t:
			BC_STACK_TOP2.type = bc_obj_ount_t;
			BC_STACK_TOP2.val.i = (bc_arch_int) BC_STACK_TOP2.val.b * (bc_arch_int) BC_STACK_TOP.val.b;
			BC_STACK_LEN--;
			return bc_error_none_t;
		case bc_obj_floap_t:
			BC_STACK_TOP2.type = bc_obj_floap_t;
			BC_STACK_TOP2.val.f = (bc_arch_float)(bc_arch_int)BC_STACK_TOP2.val.b;
			BC_STACK_TOP2.val.f *= BC_STACK_TOP.val.f;
			BC_STACK_LEN--;
			return bc_error_none_t;
		case bc_obj_complex_t:
			{
				bc_object o = {.ref_count = NULL, .type = Obj_complex_t};
				o.val.c[0] = BC_STACK_TOP.val.c[0] * (bc_arch_int)BC_STACK_TOP2.val.b;
				o.val.c[1] = BC_STACK_TOP.val.c[1] * (bc_arch_int)BC_STACK_TOP2.val.b;
				BC_STACK_TOP2 = o;
			}
			BC_STACK_LEN--;
			return bc_error_none_t;
		case bc_obj_custom_t:
			return bc_mul_custom();
		default:
			break;
	}
	return bc_error_op_type_t;
}

int bc_mul_string() {
	switch (BC_STACK_TOP.type) {
		case bc_obj_ount_t:
			int len = strlen(BC_STACK_TOP2.val.s);
			char *s = malloc(sizeof(char) * (len * BC_STACK_TOP.val.i + 1));
			int end = 0;
			for (int i = 0; i < BC_STACK_TOP.val.i; i++) {
				for (int k = 0; k < len; k++) {
					s[end] = BC_STACK_TOP2.val.s[k];
					end++;
				}
			}
			free(BC_STACK_TOP2.val.s);
			BC_STACK_TOP2.val.s = s;
			BC_STACK_LEN--;
			return bc_error_none_t;
		case bc_obj_custom_t:
			return bc_mul_custom();
		default:
			break;
	}
	return bc_error_op_type_t;
}

int bc_mul_floap() {
	switch (BC_STACK_TOP.type) {
		case bc_obj_ount_t:
			BC_STACK_TOP2.val.f *= BC_STACK_TOP.val.i;
			BC_STACK_LEN--;
			return bc_error_none_t;
		case bc_obj_bool_t:
			BC_STACK_TOP2.val.f *= (bc_arch_int)BC_STACK_TOP.val.b;
			BC_STACK_LEN--;
			return bc_error_none_t;
		case bc_obj_floap_t:
			BC_STACK_TOP2.val.f *= BC_STACK_TOP.val.f;
			BC_STACK_LEN--;
			return bc_error_none_t;
		case bc_obj_complex_t:
			{
				bc_object o = {.ref_count = NULL, .type = Obj_complex_t};
				o.val.c[0] = BC_STACK_TOP.val.c[0] * BC_STACK_TOP2.val.f;
				o.val.c[1] = BC_STACK_TOP.val.c[1] * BC_STACK_TOP2.val.f;
				BC_STACK_TOP2 = o;
			}
			BC_STACK_LEN--;
			return bc_error_none_t;
		case bc_obj_custom_t:
			return bc_mul_custom();
		default:
			break;
	}
	return bc_error_op_type_t;
}

int bc_mul_complex() {
	switch (BC_STACK_TOP.type) {
		case bc_obj_custom_t:
			return bc_mul_custom();
		default:
			break;
	}
	return bc_error_op_type_t;
}

int bc_mul_list() {
	switch (BC_STACK_TOP.type) {
		case bc_obj_custom_t:
			return bc_mul_custom();
		default:
			break;
	}
	return bc_error_op_type_t;
}

int bc_mul_funcdef() {
	switch (BC_STACK_TOP.type) {
		case bc_obj_custom_t:
			return bc_mul_custom();
		default:
			break;
	}
	return bc_error_op_type_t;
}

int bc_mul_module() {
	switch (BC_STACK_TOP.type) {
		case bc_obj_custom_t:
			return bc_mul_custom();
		default:
			break;
	}
	return bc_error_op_type_t;
}

int bc_mul_custom() {
	switch (BC_STACK_TOP.type) {
		default:
			break;
	}
	return bc_error_op_type_t;
}

