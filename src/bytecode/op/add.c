#include "bytecode/objects.h"
#include "bytecode/interpreter.h"
#include "bytecode/memory.h"
#include "bytecode/operations.h"
#include "string.h"
#include "stdlib.h"

int bc_add_ount() {
	switch (BC_STACK_TOP.type) {
		case bc_obj_ount_t:
			BC_STACK_TOP2.val.i += BC_STACK_TOP.val.i;
			BC_STACK_LEN--;
			return bc_error_none_t;
		case bc_obj_bool_t:
			BC_STACK_TOP2.val.i += BC_STACK_TOP.val.b;
			BC_STACK_LEN--;
			return bc_error_none_t;
		case bc_obj_floap_t:
			BC_STACK_TOP2.type = bc_obj_floap_t;
			BC_STACK_TOP2.val.f = (bc_arch_float)BC_STACK_TOP2.val.i;
			BC_STACK_TOP2.val.f += BC_STACK_TOP.val.f;
			BC_STACK_LEN--;
			return bc_error_none_t;
		case bc_obj_complex_t:
			BC_STACK_TOP.val.c[0] += BC_STACK_TOP2.val.i;
			BC_STACK_TOP2.type = bc_obj_complex_t;
			BC_STACK_TOP2.val.c[0] = BC_STACK_TOP.val.c[0];
			BC_STACK_TOP2.val.c[1] = BC_STACK_TOP.val.c[1];
			BC_STACK_LEN--;
			return bc_error_none_t;
		case bc_obj_custom_t:
			return bc_add_custom();
		default:
			break;
	}
	return bc_error_op_type_t;
}

int bc_add_bool() {
	switch (BC_STACK_TOP.type) {
		case bc_obj_ount_t:
			BC_STACK_TOP2.type = bc_obj_ount_t;
			BC_STACK_TOP2.val.i = (bc_arch_int)BC_STACK_TOP2.val.b + BC_STACK_TOP.val.i;
			BC_STACK_LEN--;
			return bc_error_none_t;
		case bc_obj_bool_t:
			BC_STACK_TOP2.type = bc_obj_ount_t;
			BC_STACK_TOP2.val.i = (bc_arch_int) BC_STACK_TOP2.val.b + BC_STACK_TOP.val.b;
			BC_STACK_LEN--;
			return bc_error_none_t;
		case bc_obj_string_t:
			return bc_error_op_type_t;
		case bc_obj_floap_t:
			BC_STACK_TOP2.type = bc_obj_floap_t;
			BC_STACK_TOP2.val.f = (bc_arch_float)(bc_arch_int)BC_STACK_TOP2.val.b;
			BC_STACK_TOP2.val.f += BC_STACK_TOP.val.f;
			BC_STACK_LEN--;
			return bc_error_none_t;
		case bc_obj_complex_t:
			BC_STACK_TOP.val.c[0] += BC_STACK_TOP2.val.b;
			BC_STACK_TOP2.type = bc_obj_complex_t;
			BC_STACK_TOP2.val.c[0] = BC_STACK_TOP.val.c[0];
			BC_STACK_TOP2.val.c[1] = BC_STACK_TOP.val.c[1];
			BC_STACK_LEN--;
			return bc_error_none_t;
		case bc_obj_custom_t:
			return bc_add_custom();
		default:
			break;
	}
	return bc_error_op_type_t;
}

int bc_add_string() {
	switch (BC_STACK_TOP.type) {
		case bc_obj_string_t:
			int len1 = strlen(BC_STACK_TOP2.val.s);
			int len2 = strlen(BC_STACK_TOP.val.s);
			char *res_str = malloc(sizeof(char) * (1 + len1 + len2));
			res_str[0] = '\0';
			strcat(res_str, BC_STACK_TOP2.val.s);
			strcat(res_str, BC_STACK_TOP.val.s);
			bc_object res = bc_new_string(res_str);
			bc_remove_ref(BC_STACK_TOP);
			bc_remove_ref(BC_STACK_TOP2);
			BC_STACK_LEN--;
			BC_STACK_TOP = res;
			return bc_error_none_t;
		case bc_obj_custom_t:
			return bc_add_custom();
		default:
			break;
	}
	return bc_error_op_type_t;
}

int bc_add_floap() {
	switch (BC_STACK_TOP.type) {
		case bc_obj_ount_t:
			BC_STACK_TOP2.val.f += BC_STACK_TOP.val.i;
			BC_STACK_LEN--;
			return bc_error_none_t;
		case bc_obj_bool_t:
			BC_STACK_TOP2.val.f += (int)BC_STACK_TOP.val.b;
			BC_STACK_LEN--;
			return bc_error_none_t;
		case bc_obj_floap_t:
			BC_STACK_TOP2.val.f += BC_STACK_TOP.val.f;
			BC_STACK_LEN--;
			return bc_error_none_t;
		case bc_obj_complex_t:
			BC_STACK_TOP.val.c[0] += BC_STACK_TOP2.val.f;
			BC_STACK_TOP2.type = bc_obj_complex_t;
			BC_STACK_TOP2.val.c[0] = BC_STACK_TOP.val.c[0];
			BC_STACK_TOP2.val.c[1] = BC_STACK_TOP.val.c[1];
			BC_STACK_LEN--;
			return bc_error_none_t;
		case bc_obj_custom_t:
			return bc_add_custom();
		default:
			break;
	}
	return bc_error_op_type_t;
}

int bc_add_complex() {
	switch (BC_STACK_TOP.type) {
		case bc_obj_ount_t:
			BC_STACK_TOP2.val.c[0] += BC_STACK_TOP.val.i;
			BC_STACK_LEN--;
			return bc_error_none_t;
		case bc_obj_bool_t:
			BC_STACK_TOP2.val.c[0] += (int)BC_STACK_TOP.val.b;
			BC_STACK_LEN--;
			return bc_error_none_t;
		case bc_obj_floap_t:
			BC_STACK_TOP2.val.c[0] += BC_STACK_TOP.val.f;
			BC_STACK_LEN--;
			return bc_error_none_t;
		case bc_obj_complex_t:
			BC_STACK_TOP2.val.c[0] += BC_STACK_TOP.val.c[0];
			BC_STACK_TOP2.val.c[1] += BC_STACK_TOP.val.c[1];
			BC_STACK_LEN--;
			return bc_error_none_t;
		case bc_obj_custom_t:
			return bc_add_custom();
		default:
			break;
	}
	return bc_error_op_type_t;
}

int bc_add_list() {
	switch (BC_STACK_TOP.type) {
		case bc_obj_list_t:
			bc_object res;
			res.ref_count = malloc(sizeof(int));
			*res.ref_count = 1;
			res.type = bc_obj_list_t;
			res.val.li = malloc(sizeof(bc_obj_list));
			res.val.li->len = 0;
			res.val.li->elements = malloc(sizeof(bc_object) * (BC_STACK_TOP2.val.li->len + BC_STACK_TOP.val.li->len));
			for (int i = 0; i < BC_STACK_TOP2.val.li->len; i++) {
				res.val.li->elements[res.val.li->len] = bc_get_surface_copy(BC_STACK_TOP2.val.li->elements[i]);
				res.val.li->len++;
			}
			for (int i = 0; i < BC_STACK_TOP.val.li->len; i++) {
				res.val.li->elements[res.val.li->len] = bc_get_surface_copy(BC_STACK_TOP.val.li->elements[i]);
				res.val.li->len++;
			}
			bc_remove_ref(BC_STACK_TOP);
			bc_remove_ref(BC_STACK_TOP2);
			BC_STACK_LEN--;
			BC_STACK_TOP = res;
			return bc_error_none_t;
		case bc_obj_custom_t:
			return bc_add_custom();
		default:
			break;
	}
	return bc_error_op_type_t;
}

int bc_add_funcdef() {
	switch (BC_STACK_TOP.type) {
		case bc_obj_custom_t:
			return bc_add_custom();
		default:
			break;
	}
	return bc_error_op_type_t;
}

int bc_add_module() {
	switch (BC_STACK_TOP.type) {
		case bc_obj_custom_t:
			return bc_add_custom();
		default:
			break;
	}
	return bc_error_op_type_t;
}

int bc_add_custom() {
	switch (BC_STACK_TOP.type) {
		default:
			break;
	}
	return bc_error_op_type_t;
}

