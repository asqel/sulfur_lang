#include "../../include/bytecode.h"
#include "../../include/bytecode/objects.h"
#include "../../include/bytecode/memory.h"
#include <stdint.h>

void bc_destroy_obj(bc_object x) {
	int len;
	switch (x.type) {
		case bc_obj_list_t:
			len = x.val.li->len;
			for (int i = 0; i < len; i++)
				bc_remove_ref(x.val.li->elements[i]);
			free(x.val.li->elements);
			free(x.val.li);
			free(x.ref_count);
			break;
		case bc_obj_module_t:
			len = x.val.mod->len;
			for (int i = 0; i < len; i++)
				bc_remove_ref(x.val.mod->members[i]);
			free(x.val.mod->members);
			free(x.val.mod);
			free(x.ref_count);
			break;
		case bc_obj_funcdef_t:
			// TODO : free funcdef
			free(x.val.fc);
			free(x.ref_count);
			break;
		case bc_obj_custom_t:
			(*x.val.oc->free_obj)(x.val.oc);
			if (x.ref_count != NULL)
				free(x.ref_count);
			break;
		default:
			break;
	}
}

void bc_remove_ref(bc_object x) {
	switch (x.type) {
		case bc_obj_floap_t:
		case bc_obj_bool_t:
		case bc_obj_complex_t:
		case bc_obj_ount_t:
		case bc_obj_nil_t:
			return ;
		case bc_obj_string_t:
			free(x.val.s);
			return ;
		case bc_obj_module_t:
		case bc_obj_funcdef_t:
		case bc_obj_list_t:
			(*x.ref_count)--;
			if (*x.ref_count == 0)
				bc_destroy_obj(x);
			return ;
		case bc_obj_custom_t:
			if (x.ref_count == NULL) {
				bc_destroy_obj(x);
			}
			else {
				(*x.ref_count)--;
				if (*x.ref_count == 0)
					bc_destroy_obj(x);
			}
			return ;
		default:
			printf("Warning remove ref:\n    unsuported type %d\n", x.type);
			return ;
	}

}

bc_object bc_get_surface_copy(bc_object x) {
	bc_object res;
	switch (x.type) {
		case bc_obj_floap_t:
		case bc_obj_bool_t:
		case bc_obj_ount_t:
		case bc_obj_nil_t:
			return x;
		case bc_obj_string_t:
			res.ref_count = NULL;
			res.type = bc_obj_string_t;
			res.val.s = uti_strdup(x.val.s);
			return res;
		case bc_obj_module_t:
		case bc_obj_funcdef_t:
		case bc_obj_list_t:
			(*x.ref_count)++;
			return x;
		case bc_obj_complex_t:
			res.type = bc_obj_complex_t;
			res.ref_count = NULL;
			res.val.c[0] = x.val.c[0];
			res.val.c[1] = x.val.c[1];
			return res;
		case bc_obj_custom_t:
			if (x.ref_count == NULL) {
				res.type = bc_obj_custom_t;
				res.ref_count = NULL;
				res.val.oc = (*x.val.oc->copy_obj)(x.val.oc);
				return res;
			}
			(*x.ref_count)++;
			return x;
		default:
			PRINT_ERR("Warning add ref:\n    unsuported type %d\n", x.type);
			return x;
	}
}

bc_object bc_new_ount(bc_arch_int x) {
	bc_object res;
	res.ref_count = NULL;
	res.type = bc_obj_ount_t;
	res.val.i = x;
	return res;
}

bc_object bc_new_bool(bc_arch_int x) {
	bc_object res;
	res.ref_count = NULL;
	res.type = bc_obj_bool_t;
	res.val.b = x;
	return res;
}

bc_object bc_new_floap(bc_arch_float x) {
	bc_object res;
	res.ref_count = NULL;
	res.type = bc_obj_floap_t;
	res.val.f = x;
	return res;
}

// will not allocate a copy of str
bc_object bc_new_string(char *str) {
	bc_object res;
	res.ref_count = NULL;
	res.type = bc_obj_string_t;
	res.val.s = str;
	return res;
}

bc_object bc_new_complex(bc_arch_float re, bc_arch_float im) {
	bc_object res;
	res.ref_count = NULL;
	res.type = bc_obj_complex_t;
	res.val.c[0] = re;
	res.val.c[1] = im;
	return res;
}
